/* 
// File:          AMBLE.C
// 
// Prefix:
// 
// Description:   Pre and post amble for function & procedures.
//
*/

#include <stdlib.h>
#include <assert.h>
#include "parser.h"
#include "ident.h"
#include "prsif.h"
#include "iproc.h"
#include "plant.h"
#include "amble.h"
#include "malloc.h"

/**********************************************************************
* PRSI_PreAmble
* This plants any code needed at function or procedure entry. This
* should be called after the procedure or function heading and before
* the block is parsed.
* This will:
* Plant ENTER to set up the stack frame.
* Reference any strings passed as value parameters.
* Set any local strings to NULL
**********************************************************************/
int PRSI_PreAmble(IdentifierT* id)
{
  IdentifierT* fplist = NULL;
  IdentifierT* locals = NULL;
  int LocalSize = 0;
  int err = FALSE;

  assert(id != NULL);

  /* For a function, plant the default return value */
  if(id->class == FunctionIdentifier)
    PRSI_PlantInteger(0);

  /* Run along the local list summing the size of all the locals */
  locals = PRSI_GetLocalList();
  while(locals != NULL)
    {
    assert(locals->type != NULL);
    LocalSize += locals->type->size;
    locals = locals->next;
    }

  /* Always plant ENTER/LEAVE: the FP offsets of the parameters
  // need this.
  */
  PRSI_PlantInteger(LocalSize);
  PRSI_Plant(ENTER);

  /* Now run through the locals (again) making sure that
  // all the strings are initialised to 0
  */
  locals = PRSI_GetLocalList();
  while(locals != NULL)
    {
    assert(locals->type != NULL);
    if(locals->type->BasicType == StringType)
      {
      PRSI_PlantInteger(0);
      PRSI_PlantInteger(locals->value.FPOffset);
      PRSI_Plant(FPST);
      }
    locals = locals->next;
    }

  /* Now run through the formal parameters: All the strings
  // that are passed by value need to be referenced (the
  // semantics are equivalent to the string being copied
  // to a local variable.)
  */
  //fplist = id->link;        /* look at FP list */
  //while(fplist != NULL)
  //  {
  //  if(fplist->type != NULL &&        /* procedure params have null type */
  //     fplist->type->BasicType == StringType &&
  //     fplist->scope == ValueParamScope)
  //    {
  //    PRSI_PlantInteger(fplist->value.FPOffset);
  //    PRSI_Plant(FPADD);
  //    PRSI_Plant(SREF);
  //    }
  //  fplist = fplist->next;
  //  }

  return err;
}

/**********************************************************************
* PRSI_PostAmble
* This plants any code needed at function or procedure exit.
* This will:
* De-reference any strings passed as value parameters.
* De-reference any local strings.
* PRSI_Plant LEAVE to unwind the stack
* PRSI_Plant RET or NRET depending on the absence or presense of locals.
**********************************************************************/
int PRSI_PostAmble(IdentifierT* id)
{
  IdentifierT* fplist = NULL;
  IdentifierT* locals = NULL;
  int LocalSize = 0;
  int ParamSize = 0;
  int err = FALSE;

  assert(id != NULL);
  
  /* Run along the local list dereferencing any strings and
  //  summing the size of all the locals
  */
  locals = PRSI_GetLocalList();
  while(locals != NULL)
    {
    assert(locals->type != NULL);
    if(locals->type->BasicType == StringType)
      {
      PRSI_PlantInteger(locals->value.FPOffset);
      PRSI_Plant(FPADD);
      PRSI_Plant(SDEL);
      }
    LocalSize += locals->type->size;
    locals = locals->next;
    }


  /* Now run through the parameters summing their sizes
  // and dereferencing any strings passed as value parameters.
  */
  fplist = id->link;        /* look at FP list */
  while(fplist != NULL)
    {
    if(fplist->type != NULL &&
       fplist->type->BasicType == StringType &&
       fplist->scope == ValueParamScope)
      {
      PRSI_PlantInteger(fplist->value.FPOffset);
      PRSI_Plant(FPADD);
      PRSI_Plant(SDEL);
      }

    if(fplist->scope == VarParamScope)
      ParamSize += sizeof(void *);
    else
      {
      /* Check for procedure or fn parameters: always passed by ref */
      if(fplist->class == ProcedureIdentifier ||
         fplist->class == FunctionIdentifier)
        ParamSize += sizeof(void *);
      else /* otherwise get parameter size from type field */
        {
        assert(fplist->type != NULL);
        ParamSize += fplist->type->size;
        }
      }
    fplist = fplist->next;
    }

  PRSI_Plant(LEAVE);

  if(ParamSize == 0)
     PRSI_Plant(RET);
  else
    {
    PRSI_PlantInteger(ParamSize);
    PRSI_Plant(NRET);
    }

  return err;
}
