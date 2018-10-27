/*
// File:        GETVAR.C
// 
// Prefix:
// 
// Description: Allows code to find the address and value of
//              global variables.
//
*/

#include <stdlib.h>
#include <assert.h>
#include "parser.h"
#include "error.h"
#include "prsif.h"
#include "expr.h"
#include "ident.h"
#include "tokens.h"
#include "plant.h"

#ifdef _MSC_VER
#pragma warning ( disable : 4100 ) /* unref param warning */
#endif

/***********************************************************
* DummyInputFunction
* This is a dummy input function which returns NULL to
* signal EOF.  This is sent to the lexical analyser to
* to make sure any errors in the function or procedure
* declaration do not muck up any input file
***********************************************************/
#ifdef _BORLANDC_
#pragma argsused 
#endif
static char *DummyInputFunction(char *buff,int n)
{
  PRS_Error(IDS_ERR_GETVAR_INCOMPLETE_VAR);
  return NULL;
}


/***********************************************************
* EvaluateAddress
* This evaluates the address of a variable referenced by
* a variable expression.  This will work for simple
* and compound variables and also for record fields. It
* will not work for array indexing (although it will
* get the address of the whole array.
***********************************************************/
static void* EvaluateAddress(ExprT* expr)
{
  char* addr = NULL;
  IdentifierT* id = NULL;
  
  assert(expr != NULL);
  
  switch (expr->operator)
    {
    case TPLeftBracket:   /* an array */
      /* Expr->left is the array base, Expr->right is the index */
      PRS_Error(IDS_ERR_GETVAR_ARRAY_ADDR);
      break;

    case TPPeriod:    /* a record */
      assert(expr->left != NULL);
      
      addr = EvaluateAddress(expr->left);
      
      assert(expr->right != NULL);
      assert(expr->right->operator == TokIdentifier);
      
      id = expr->right->value.ident;
      
      assert(id->class == FieldIdentifier);
      addr += (int)id->value.offset; /* add in byte offset of field */
      break;

    case TokIdentifier:
      id = expr->value.ident;
      assert(id != NULL);
      assert(id->type != NULL);
      if(PRSI_IsSimpleType(id->type))
        addr = (char*)&(id->value);
      else
        addr = id->value.addr;
      break;

    default:
      PRS_Error(IDS_ERR_GETVAR_INVALID_ADDR_OP);
      break;

    }
  return addr;
}

/***********************************************************
* PRS_VariableAddress
* This parses a variable description and if the variable
* is global, and exists, returns a pointer to it.
***********************************************************/
int PRS_VariableAddress(const char* lpszVar,void** lplpVoid)
{
  int err = FALSE;
  ExprT *expr = NULL;
  IdentifierT* ident = NULL;
  PFSIS oldipfn = NULL;
  int tok;
  char* name = NULL;
  void *addr = NULL;

  assert(lpszVar != NULL);
  assert(lplpVoid != NULL);
  
  /* Set up a dummy input function to prevent any
  // read from file.
  */
  oldipfn = PRS_InputFunction(DummyInputFunction);

  PRS_SendCommand(lpszVar);         /* send declaration to lexical analyser */

  tok = PRSI_LexGetToken();
  if(tok == TokIdentifier)
    {
    name = PRSI_LexGetIdentifier();

    ident = PRSI_FindIdentifier(name);
    if (ident == NULL)
      {
      err = TRUE; /* Can't find variable */
      PRS_Error(IDS_ERR_GETVAR_UNKNOWN_VAR,name);
      }
    else if(ident->class != VariableIdentifier)
      {
      err = TRUE;
      PRS_Error(IDS_ERR_GETVAR_ID_NOT_VARIABLE,name);
      }
    else if(ident->scope != GlobalScope)
      {
      err = TRUE;
      PRS_Error(IDS_ERR_GETVAR_ID_NOT_GLOBAL,name);
      }
    else
      {
      expr = PRSI_Variable(ident);
      if(expr == NULL)
        err = TRUE;
      else
        {
        addr = EvaluateAddress(expr);
        if(addr == NULL)
          err = TRUE;
        else
          *lplpVoid = addr;
        }
      }
    }
  PRS_InputFunction(oldipfn);  /* restore old input function */

  return err;
}

/***********************************************************
* PRS_Real
* This gets the value of a global real 
***********************************************************/
int PRS_Real(const char* lpszVar,float* fVal)
{
  int err = FALSE;
  float* pfVal = NULL;
  
  assert(lpszVar != NULL);
  assert(fVal != NULL);
  
  err = PRS_VariableAddress(lpszVar,(void**)&pfVal);
  if(!err)
    {
    *fVal = *pfVal;
    }

  return err;
}

/***********************************************************
* PRS_Ordinal
* This gets the value of a global ordinal value
***********************************************************/
int PRS_Ordinal(const char* lpszVar,long* lVal)
{
  int err = FALSE;
  long *plVal = NULL;

  assert(lpszVar != NULL);
  assert(lVal != NULL);

  err = PRS_VariableAddress(lpszVar,(void**)&plVal);
  if(!err)
    {
    *lVal = *plVal;
    }
  return err;
}

/***********************************************************
* PRS_String
* This gets the value of a global string.
***********************************************************/
int PRS_String(const char* lpszVar, const char** lplpszVal)
{
  int err = FALSE;
  StringT *pstr = NULL;

  assert(lpszVar != NULL);
  assert(lplpszVal != NULL);

  err = PRS_VariableAddress(lpszVar,(void**)&pstr);
  if(!err)
    {
    *lplpszVal = PRS_StringContents(pstr);
    }
  return err;
}

