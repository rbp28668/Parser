/*
// File:          PLANT.C
//
// Prefix:
//
// Description:   Low level code planting
//
*/

/* #define SHOW */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parser.h"
#include "prsif.h"
#include "iproc.h"
#include "expr.h"
#include "plant.h"
#include "error.h"
#include "malloc.h"

#if !defined NDEBUG && defined SHOW
#include "printf.h"
#endif


#define SEG_SIZE 4096 /* gives max size of procedure or fn */
#define MAX_SAVED 16  /* max no of nested code buffers */

typedef enum e_SaveT
  {
  SAVE_NOTHING,
  SAVE_INTEGER,
  SAVE_REAL,
  SAVE_ADDR
  } SaveT;

static SaveT Saved = SAVE_NOTHING;
static long SavedInt = 0;
static float SavedReal = 0;
static void* SavedAddr = NULL;

static Opcode* CurrentSegment = NULL;
static Opcode* SavedSegments[MAX_SAVED] = {NULL};
static int nSegCount = 0;

static int offset = 0;
static int SegTop = 0; /* 1 less than SEG_SIZE */
static int bad = FALSE;
static int runnable = FALSE;


#if!defined  NDEBUG && defined SHOW
/**********************************************************************
* ShowLiteral
* Displays a value which will be planted in the next opcode
**********************************************************************/
static void ShowLiteral(void)
{

  switch (Saved)
    {
    case SAVE_NOTHING:
      break;

    case SAVE_INTEGER:
      if(SavedInt >= -127 && SavedInt <= 127)
        PRSI_printf("        BYTE %ld\n",SavedInt);
      else if( SavedInt >= -32767 && SavedInt <= 32767)
        PRSI_printf("        WORD %ld\n",SavedInt);
      else
        PRSI_printf("        LONG %ld\n",SavedInt);
      break;

    case SAVE_REAL:
      PRSI_printf("        FLOAT %f\n",SavedReal);
      break;

    case SAVE_ADDR:
      PRSI_printf("        ADDR %p\n",SavedAddr);
      break;

    default:
      PRSI_printf("Unknown stored value\n");
      break;
    }

  return;
}
#endif

/**********************************************************************
* PlantActualAddr
* Plants an address on the current opcode
**********************************************************************/
static void PlantActualAddr(void* addr)
{
  assert(offset > 0);
  if(CurrentSegment != NULL && !bad)
    {
    if( offset + (int)sizeof(void*) < SegTop)
      {
      CurrentSegment[offset-1] |= IP_Long;
      *((void**)(CurrentSegment+offset)) = addr;
      offset += sizeof(void*);
      }
    else
      {
      PRS_Error(IDS_ERR_PLANT_TOO_COMPLEX);
      bad = TRUE;
      }
    }
  return;
}

/**********************************************************************
* PlantActualInteger
* This plants an integer onto the current opcode. The system will
* try and minimise the number of bytes needed.
**********************************************************************/
static void PlantActualInteger(long lval)
{
  assert(offset > 0);
  if(CurrentSegment != NULL && !bad)
    {
    if(offset + (int)sizeof(void*) < SegTop)
      {
      if(lval >= -127 && lval <= 127)
        {
        CurrentSegment[offset-1] |= IP_Byte;
        *(signed char*)(CurrentSegment+offset) = (signed char)lval;
        offset += sizeof(signed char);
        }
      else if( lval >= -32767 && lval <= 32767)
        {
        CurrentSegment[offset-1] |= IP_Word;
        *(int*)(CurrentSegment+offset) = (int)lval;
        offset += sizeof(int);
        }
      else
        {
        CurrentSegment[offset-1] |= IP_Long;
        *((long*)(CurrentSegment+offset)) = lval;
        offset += sizeof(long);
        }
      }
    else
      {
      PRS_Error(IDS_ERR_PLANT_TOO_COMPLEX);
      bad = TRUE;
      }
    }
  return;
}

/**********************************************************************
* PlantActualFloat
* This plants a float onto the current opcode
**********************************************************************/
static void PlantActualFloat(float fval)
{
  assert(offset > 0);
  if(CurrentSegment != NULL && !bad)
    {
    if(CurrentSegment != NULL && offset + (int)sizeof(float) < SegTop)
      {
      CurrentSegment[offset-1] |= IP_Long;
      *((float*)(CurrentSegment+offset)) = fval;
      offset += sizeof(float);
      }
    else
      {
      PRS_Error(IDS_ERR_PLANT_TOO_COMPLEX);
      bad = TRUE;
      }
    }
  return;
}

/**********************************************************************
* PRSI_Plant
* Plants an opcode
**********************************************************************/
void PRSI_Plant(Opcode op)
{
  if(CurrentSegment != NULL && !bad)
    {
    if(offset < SegTop)
      {
      #if !defined  NDEBUG && defined SHOW
      ShowLiteral();
      PRSI_printf("%6d: %s\n",offset,IP_TranslateOpcode(op));
      #endif

      CurrentSegment[offset++] = op;

      switch (Saved)
        {
        case SAVE_NOTHING:
          break;

        case SAVE_INTEGER:
          PlantActualInteger(SavedInt);
          break;

        case SAVE_REAL:
          PlantActualFloat(SavedReal);
          break;

        case SAVE_ADDR:
          PlantActualAddr(SavedAddr);
          break;
        }
      Saved = SAVE_NOTHING;
      }
    else
      {
      PRS_Error(IDS_ERR_PLANT_TOO_COMPLEX);
      bad = TRUE;
      }
    }
  return;
}

/**********************************************************************
* PRSI_PlantBranch
* Plants a branch opcode, reserves 2 bytes of space for the
* jump and returns the address of the branch opcode
**********************************************************************/
Opcode* PRSI_PlantBranch(Opcode op)
{
  Opcode* pos = NULL;

  if(CurrentSegment != NULL && !bad)
    {
    if(Saved)
      PRSI_Plant(NOP);
    pos = CurrentSegment+offset;
    PRSI_Plant(op);  /* should be BRA or ZBRA */
    CurrentSegment[offset-1] |= IP_Word;
    *(int*)(CurrentSegment+offset) = 0; /* use 0 as placeholder */
    offset += sizeof(int);
    }
  return pos;
}

/**********************************************************************
* PRSI_PlantSetBranch
* sets up a previously planted branch (using PlantBranch) to
* point to the branch destination.
**********************************************************************/
void PRSI_PlantSetBranch(Opcode* Target,Opcode* BrInst)
{ 
  int rel = 0;

  assert(Target != NULL);
  assert(BrInst != NULL);
  
  if(CurrentSegment != NULL && !bad)
    {
    assert(Target != NULL);
    assert(BrInst != NULL);
    rel = (int)(Target - (BrInst + sizeof(Opcode) + sizeof(int)));
    *(int*)(BrInst+sizeof(Opcode)) = rel;
    }
  return;
}

/**********************************************************************
* PRSI_PlantPosition
* Returns the address where the next opcode will go.
**********************************************************************/
Opcode* PRSI_PlantPosition(void)
{
  return CurrentSegment + offset;
}


/**********************************************************************
* PRSI_PlantAddr
* Plants an address on the current opcode
**********************************************************************/
void PRSI_PlantAddr(void* addr)
{
  if(CurrentSegment != NULL && !bad)
    {
    if(Saved)
      PRSI_Plant(NOP);
    SavedAddr = addr;
    Saved = SAVE_ADDR;
    }
}

/**********************************************************************
* PRSI_PlantInteger
* This plants an integer onto the current opcode. The system will
* try and minimise the number of bytes needed.
**********************************************************************/
void PRSI_PlantInteger(long lval)
{
  if(CurrentSegment != NULL && !bad)
    {
    if(Saved)
      PRSI_Plant(NOP);
    SavedInt = lval;
    Saved = SAVE_INTEGER;
    }
  return;
}

/**********************************************************************
* PRSI_PlantFloat
* This plants a float onto the current opcode
**********************************************************************/
void PRSI_PlantFloat(float fval)
{
  if(CurrentSegment != NULL && !bad)
    {
    if(Saved)
      PRSI_Plant(NOP);
    SavedReal = fval;
    Saved = SAVE_REAL;
    }
  return;
}

/**********************************************************************
* PRSI_PlantString
* plants a literal string in the code.
**********************************************************************/
void PRSI_PlantString(PSTRING pstr)
{ 
  assert(pstr != NULL);
  if(CurrentSegment != NULL && !bad)
    {
    PRSI_Plant(SLT); /* string literal opcode */
    if(pstr == NULL)
      {
      if(offset < SegTop)
        CurrentSegment[offset++] = 0;
      else
        bad = TRUE;
      }
    else /* pstr != NULL */
      {
      const char* str = PRS_StringContents(pstr);
      int len = strlen(str) + 1;
      if(offset + len < SegTop)
        {
        strcpy((char*)(CurrentSegment + offset),str);
        offset += len;
        }
      else
        bad = TRUE;
      }
        
    if(bad)
      {
      PRS_Error(IDS_ERR_PLANT_TOO_COMPLEX);
      }
    }

  
}

/**********************************************************************
* PRSI_PlantNewSegment
* Initialises the code segment for the next set of opcodes
**********************************************************************/
void PRSI_PlantNewSegment(void)
{
  offset = 0;
  bad = FALSE;
  runnable = FALSE;
  Saved = SAVE_NOTHING;

  #if !defined NDEBUG && defined SHOW
  PRSI_printf("New Code Segment\n");
  #endif
}


/**********************************************************************
* PRSI_PlantGetSegment
* Makes and returns a copy of the current segment. Used for procedures
* and functions to save the I-Code for the body.
**********************************************************************/
Opcode* PRSI_PlantGetSegment(void)
{
  Opcode* seg = NULL;
  PRSI_Plant(HALT);
  if(!bad)
    {
    seg = malloc(offset);
    if(seg != NULL)
      memcpy(seg,CurrentSegment,offset);
    }
  return seg;
}

/**********************************************************************
* Function:   PRSI_PlantSetRunnable
* This sets an internal flag which marks the current segment as
* runnable.  This should be set if parsing a simple statement from
* the command line.
* Parameters: NONE
*
* Returns:    NOTHING
**********************************************************************/
void PRSI_PlantSetRunnable(void)
{
  if(!bad && CurrentSegment != NULL)
    runnable = TRUE;
}

/**********************************************************************
* Function:   PRSI_PlantCurrentSegment
* This gets a copy of the current segment. It plants a HALT instruction
* first to ensure that program execution will not run past the end.
* Parameters: NONE
*
* Returns:    a pointer to the current segment or NULL if there
*             was a problem or the segment was not marked as runnable.
**********************************************************************/
Opcode* PRSI_PlantCurrentSegment(void)
{
  Opcode* seg = NULL;
  if(!bad && runnable)
    {
    PRSI_Plant(HALT);
    seg = CurrentSegment;
    }
  return seg;

}

/**********************************************************************
* PRSI_PlantInitialise
* Initialises the code planting
**********************************************************************/
int PRSI_PlantInitialise(void)
{
  int err = 0;
  if(CurrentSegment == NULL)
    {
    CurrentSegment = malloc(SEG_SIZE);
    SegTop = SEG_SIZE-1;
    offset = 0;
    bad = FALSE;
    }

  if(CurrentSegment == NULL)
    err = IDS_ERR_PLANT_ALLOCATE_CODE_BUFFER;
  return err;
}

/**********************************************************************
* Function:   PRSI_PlantTerminate
* Terminates the code planting and frees any resources used by this
* module.
* Parameters: NONE
*
* Returns:    NOTHING
**********************************************************************/
void PRSI_PlantTerminate(void)
{
  if(CurrentSegment != NULL)
    {
    free(CurrentSegment);
    CurrentSegment = NULL;
    }

  assert(nSegCount == 0);
  while(nSegCount > 0)
    {
    free(SavedSegments[--nSegCount]);
    SavedSegments[nSegCount] = NULL;
    }
}

/**********************************************************************
* Function:   PRSI_SaveCode
* This saves the current code segment and creates a new one.  This
* allows C call functions to read in (and have parsed and hence have
* code planted) code without over-writing the currently executing code
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
int PRSI_SaveCode(void)
{
  int err = 0;

  offset = 0;
  bad = FALSE;

  if(nSegCount >= MAX_SAVED)
    {
    err = IDS_ERR_PLANT_TOO_MANY_SAVE;
    PRS_Error(err);
    bad = TRUE;
    }
  else
    {
    SavedSegments[nSegCount++] = CurrentSegment;
    CurrentSegment = malloc(SEG_SIZE);
    if(CurrentSegment == NULL)
      {
      err = IDS_ERR_PLANT_SAVE_CODE_SEG;
      PRS_Error(err);
      bad = TRUE;
      }
    }

  return err;
}

/**********************************************************************
* Function:   PRSI_RestoreCode
* This restores the effect of PRSI_SaveCode
* Parameters: NONE
*
* Returns:    0 for success, non zero for failure
**********************************************************************/
int PRSI_RestoreCode(void)
{
  int err = 0;
  if(nSegCount == 0)
    {
    err = IDS_ERR_PLANT_INVALID_RESTORE;
    }
  else
    {
    if(CurrentSegment != NULL)
      free(CurrentSegment);
    CurrentSegment = SavedSegments[--nSegCount];
    SavedSegments[nSegCount] = 0;
    }
  return err;
}
