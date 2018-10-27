#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "prsif.h"
#include "pstring.h"
#include "pdrive.h"

static int blQuit = 0;

/* Forward declarations of functions for driver table */
static int PAS_LoadFile(void);
static int PAS_Forget(void);
static int PAS_Fence(void);
static int PAS_Version(void);
static int PAS_ShowIdentifiers(void);
static int PAS_WhatIs(void);
static int PAS_Quit(void);

/* Function table: this maps C functions to their pascal declarations */
static FunctionTableT FunctionTable[] =
{
  {PAS_LoadFile,  "procedure include(name : string)"},
  {PAS_Forget,    "procedure forget(name : string)"},
  {PAS_Fence,     "procedure fence(name : string)"},
  {PAS_Version,   "procedure version"},
  {PAS_ShowIdentifiers, "procedure list"},
  {PAS_WhatIs,    "procedure whatis(name : string)"},
  {PAS_Quit,      "procedure quit"},
  {NULL,NULL}
};

/**********************************************************************
* Function:   PAS_LoadFile
* procedure LoadFile(name : string)
* This is loads a pascal file into the interpreter
* Parameters: NONE
*
* Returns:    0 for success non-zero (fault) for failure
**********************************************************************/
static int PAS_LoadFile(void)
{
  StringT* str = NULL;
  int err = 0;
  str = *(StringT**)PRS_Parameters(sizeof(StringT*));
  assert(str != 0);
  err = PRS_LoadFile(PRS_StringContents(str));
  return err; /* non-zero will cause a fault */
}


/**********************************************************************
* Function:   PAS_Forget
* Forget(name : string)
* This unloads the given identifier and all identifier which were
* defined after it, from the symbol table.
* Parameters: NONE
*
* Returns:    0 for success non-zero (fault) for failure
**********************************************************************/
static int PAS_Forget(void)
{
  StringT* str = *(StringT**)PRS_Parameters(sizeof(StringT*));
  assert(str != NULL);
  return PRS_Forget(PRS_StringContents(str));
}


/**********************************************************************
* Function:   PAS_Fence
* procedure Fence(name : string)
* This puts a "Fence" in the symbol table.  The named identifier,
* and all identifiers before it, are protected from unloading by
* PRS_Forget.  This allows firmware to be protected but still gives
* the end user the option of dumping any stuff they have put in but
* no longer want.
* Parameters:
*
* Returns:    0 for success non-zero (fault) for failure
**********************************************************************/
static int PAS_Fence(void)
{
  StringT* str = *(StringT**)PRS_Parameters(sizeof(StringT*));
  assert(str != NULL);
  return PRS_Fence(PRS_StringContents(str));
}


/**********************************************************************
* Function:   PAS_Version
* procedure Version
* This writes the current interpreter version to the output.
* Parameters: NONE
* Returns:    0 (for success)
**********************************************************************/
static int PAS_Version(void)
{
  int major;
  int minor;
  char szBuff[64];

  PRS_Version(&major, &minor);                  /* get current version */
  sprintf(szBuff,"Version %d.%d",major,minor);  /* format it as text */
  PRS_Write(szBuff);                            /* and write it to output */

  return 0;
}

/**********************************************************************
* Function:   PAS_ShowIdentifiers
* procedure list
* Parameters: NONE
* Returns:    0 (for success)
**********************************************************************/
static int PAS_ShowIdentifiers(void)
{
  PRS_ShowIdentifiers(0);
  return 0;
}

/**********************************************************************
* Function:   PAS_WhatIs
* procedure whatis(name : string)
* Parameters: NONE
* Returns:    0 (for success)
**********************************************************************/
static int PAS_WhatIs(void)
{
  const char* lpszIdent = NULL;
  StringT* str = *(StringT**)PRS_Parameters(sizeof(StringT*));
  assert(str != NULL);
  lpszIdent = PRS_StringContents(str);
  if(!PRS_WhatIs(lpszIdent))
    {
    char szBuff[128];
    strcpy(szBuff,"Don't know about ");
    strcat(szBuff,lpszIdent);
    PRS_Write(szBuff);
    }
  return 0;
}

/**********************************************************************
* Function:   PAS_Quit
*
* Parameters: NONE
* Returns:    0 (for success)
**********************************************************************/

static int PAS_Quit(void)
{
  blQuit = 1;
  return 0;
}

/**********************************************************************
* Function:   QuitInterpreter
* This returns true if the interpreter has received a quit command
* (via PAS_Quit).
* Parameters: NONE
* Returns:    TRUE if the interpreter should quit, FALSE otherwise.
**********************************************************************/
int QuitInterpreter(void)
{
  return blQuit;
}

/**********************************************************************
* Function:   SetupFunctions
* This sets up whatever functions are stored in the funtion table as
* procedures or functions which can be called by the interpreter
* Parameters: table is a function/declaration table.
*
* Returns:    NOTHING
**********************************************************************/
void SetupCFunctions(FunctionTableT* table)
{
  int i;
  assert(table != NULL);
  for(i=0; table[i].Function != NULL; ++i)
    {
    PRS_SetCExtension(table[i].Function,table[i].Declaration);
    }
  return;
}

/**********************************************************************
* Function:   SetupParserExtensions
* This sets up some predefined functions so that they can be called
* by the interpreter
* Parameters: NONE
*
* Returns:    NOTHING
**********************************************************************/
void SetupParserExtensions(void)
{
  SetupCFunctions(FunctionTable);
}
