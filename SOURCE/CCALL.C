/* 
// File:          CCALL.C
// 
// Prefix:        NONE
// 
// Description:   Setting up 'C' functions and procedures
//
*/

#include <stdlib.h>
#include <assert.h>
#include "prsif.h"
#include "parser.h"
#include "tokens.h"
#include "error.h"
#include "fndec.h"
#include "procdec.h"
#include "ident.h"
#include "malloc.h"

#ifdef _MSC_VER
#pragma warning ( disable : 4100 ) /* unused parameters */
#endif
/***********************************************************
* DummyInputFunction
* This is a dummy input function which returns NULL to
* signal EOF.  This is sent to the lexical analyser to
* to make sure any errors in the function or procedure
* declaration do not muck up any input file
***********************************************************/
#ifdef __BORLANDC__
#pragma argsused
#endif

static char *DummyInputFunction(char *buff,int n)
{
  assert(buff != NULL);
  return NULL;
}



/***********************************************************
* PRS_SetCExtension
* allows other program code to link 'C' functions to
* the parser so they can be called as normal.
* lpfi is a pointer to the c function, lpszDecl should
* contain a function or procedure heading.
***********************************************************/
int PRS_SetCExtension(PFVI lpfi,const char *lpszDecl)
{
  PFSIS oldipfn;
  IdentifierT *ident = NULL;
  int tok;
  int err = FALSE;

  assert(lpfi != NULL);
  assert(lpszDecl != NULL);
  
  /* Set up a dummy input function to prevent any
  // read from file.
  */
  oldipfn = PRS_InputFunction(DummyInputFunction);


  PRS_SendCommand(lpszDecl);         /* send declaration to lexical analyser */


  tok = PRSI_LexGetToken();

  switch (tok)
    {
    case TKProcedure:
      ident = PRSI_ProcedureHeading(LINK);
      break;

    case TKFunction:
      ident = PRSI_FunctionHeading(LINK);
      break;

    default:
      PRS_Error(IDS_ERR_CCALL_NOT_PROC_OR_FN);
      err = TRUE;
      break;
    }

  if(ident != NULL)
    {
    ident->flags |= IDF_CCALL;  /* set c-call flag */
    ident->value.pfvi = lpfi;   /* and record where it is */
    }
  else
    {
    err = TRUE;
    }
  
  PRS_InputFunction(oldipfn);  /* restore old input function */
  return err;
}
