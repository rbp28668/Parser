/* 
// File:        APLIST.C
//
// Prefix:
//
// Description: Parse actual parameter list & check its type
//
*/

#include <stdlib.h>
#include <assert.h>
#include "prsif.h"
#include "parser.h"
#include "lex.h"
#include "tokens.h"
#include "error.h"
#include "expr.h"
#include "musthave.h"
#include "ident.h"
#include "aplist.h"
#include "malloc.h"


/**********************************************************************
* CheckArgType
* This checks the type of an actual parameter against the
* corresponding formal parameter.
* It returns FALSE if they match, otherwise it returns TRUE
**********************************************************************/
static int CheckArgType(IdentifierT* fn,int count,
  ExprT* ActualParam,IdentifierT* FormalParam)
{
  int err = FALSE;
  TypeT* etype = NULL;
  
  assert(fn != NULL);
  assert(count > 0);
  assert(ActualParam != NULL);
  assert(FormalParam != NULL);
  
  etype = PRSI_ExpressionType(ActualParam);
  if(etype == NULL)
    err = TRUE;
  else
    {
    /* Check inward assignment, & for a var parameter, check
    // outward assignment
    */
    if(PRSI_IsAssignmentCompatible(FormalParam->type,etype) == NULL)
      err = TRUE;
    else if (FormalParam->scope == VarParamScope)
      {
      if(PRSI_IsAssignmentCompatible(etype,FormalParam->type) == NULL)
        err = TRUE;
      }
    }
    
  if(err)
    {
    if (fn->class == FunctionIdentifier)
      PRS_Error(IDS_ERR_APLIST_FN_TYPE_MISMATCH,
        count, FormalParam->name, fn->name);
    else
      PRS_Error(IDS_ERR_APLIST_PROC_TYPE_MISMATCH,
        count, FormalParam->name, fn->name);
    }


  /* Check VAR parameter is an LValue */
  if(!err && FormalParam->scope == VarParamScope)
    {
    if (!PRSI_ExprIsLValue(ActualParam))
      {
      PRS_Error(IDS_ERR_APLIST_NEED_LVALUE,
         count,FormalParam->name,fn->name);
      err = TRUE;
      }
    }

  return err;
}

/**********************************************************************
* CheckTypeOfFPLists
* This checks that the formal parameter list of a function or
* procedure actual parameter matches that the formal parameter
* list of the formal parameter. This match must be exact.
* Fn is the function or procedure which has the function or
*  procedur argument
* count is the index of it fn or proc arg
* ParamIdent is the identifier of the fn or proc being passed to it
* fplist is the identifier of the fn or proc arg in the formal
* parameter list.
**********************************************************************/
static int CheckTypeOfFPLists(IdentifierT* fn,int count,
  IdentifierT* ParamIdent,IdentifierT* fplist)
{
  int err = FALSE;
  IdentifierT *apfplist = NULL; /* actual param formal param list */
  IdentifierT *fpfplist = NULL; /* formal param formal param list */
    
  assert(fn != NULL);
  assert(count >0);
  assert(ParamIdent != NULL);
  assert(fplist != NULL);
   
  apfplist = ParamIdent->link;  /* fp list of actual param */
  fpfplist = fplist->link;      /* fp list of formal param */

  while (apfplist != NULL && fpfplist != NULL)
    {
    if(apfplist->type != fpfplist->type)
      {
      err = TRUE;
      break;
      }

    apfplist = apfplist->next;
    fpfplist = fpfplist->next;
    }

  /* If both procedures or functions have the same number of
  // arguments then both lists should finish at the same time.
  // Check this:
  */
  if(!err)
    {
    if(apfplist != NULL && fpfplist != NULL)
      err = TRUE;
    }

  if(err)
    {
    PRS_Error(IDS_ERR_APLIST_PARAM_MISMATCH,
      count,fplist->name,fn->name);
    }
  return err;
}

/**********************************************************************
* PRSI_ActualParameterList
*
* Reads the actual parameter list for a function or procedure call.
* This is returned in ppExpr as a list of ExprT linked by their
* aplist fields.  This returns FALSE on success, TRUE on failure.
* The procedure or function identifier is passed as id.
*
* actual-parameter-list = "(" actual-parameter { "," actual-parameter } ")".
*
**********************************************************************/
int PRSI_ActualParameterList(IdentifierT *id, ExprT** ppExpr)
{
  ExprT *list = NULL;
  ExprT *param = NULL;
  ExprT **tail = &list;
  IdentifierT *fplist = NULL;
  int tok;
  int err = FALSE;
  int count = 0;
  IdentifierT* ParamIdent = NULL;
  char* name = NULL;
  int blVarargs = FALSE;
  int blFinished = FALSE;
  
  assert(id != NULL);
  assert(ppExpr != NULL);
  
  fplist = id->link;
  blVarargs = ((id->flags & IDF_VARARGS) != 0);
  
  tok = PRSI_LexGetToken();
  if(tok != TPLeftParen)  /* There is no argument list */
    {
    if(blVarargs)
      {
      PRSI_LexPushBackToken(tok);
      blFinished = TRUE;
      }
    else
      {
      PRS_Error(IDS_ERR_APLIST_MISSING_ARGUMENT_LIST,id->name);
      err = TRUE;
      }
    }
  else  /* there is an argument list */
    {
    while(!blFinished)
      {
      ++count;
      param = NULL;

      if(fplist == NULL)
        {
        PRS_Error(IDS_ERR_APLIST_TOO_MANY_PARAMS,id->name);
        err = TRUE;
        break;
        }

      /* Depending on the formal parameter class we need to
      // treat the arguments differently.
      */
      switch (fplist->class)
        {
        case VariableIdentifier:
          /* be this ValueParamScope or VarParamScope */
          param = PRSI_ParseExpression();
          if (param == NULL)
            err = TRUE;
          else
            {
            err = CheckArgType(id,count,param,fplist);
            if(err)
              {
              PRSI_FreeExpression(param);
              break;
              }
            }
          break;

        case ProcedureIdentifier:
          tok = PRSI_LexGetToken();
          if(tok != TokIdentifier)
            {
            PRS_Error(IDS_ERR_APLIST_EXPECTED_PROC_PARAM,
              count, fplist->name, id->name);
            err = TRUE;
            }
          else if( (ParamIdent = PRSI_FindIdentifier(name = PRSI_LexGetIdentifier()))
             == NULL)
            {
            PRS_Error(IDS_ERR_APLIST_UNABLE_TO_FIND,name);
            err = TRUE;
            }
          else if(ParamIdent->class != ProcedureIdentifier)
            {
            PRS_Error(IDS_ERR_APLIST_NOT_PROC_PARAM,
              count,fplist->name,id->name,name);
            err = TRUE;
            }
          else
            err = CheckTypeOfFPLists(id,count,ParamIdent,fplist);

          if(!err)
            {
            if((param = PRSI_NewExpr()) == NULL)
              {
              PRS_Error(IDS_ERR_APLIST_EXPR_MEM);
              err = TRUE;
              }
            else
              {
              param->operator = TokIdentifier;
              param->value.ident = ParamIdent;
              }

            }
          break;

        case FunctionIdentifier:
          tok = PRSI_LexGetToken();
          if(tok != TokIdentifier)
            {
            PRS_Error(IDS_ERR_APLIST_EXPECTED_FN_PARAM,
              count, fplist->name, id->name);
            err = TRUE;
            }
          else if((ParamIdent = PRSI_FindIdentifier(name = PRSI_LexGetIdentifier()))
            == NULL)
            {
            PRS_Error(IDS_ERR_APLIST_UNABLE_TO_FIND,name);
            err = TRUE;
            }
          else if(ParamIdent->class != FunctionIdentifier)
            {
            PRS_Error(IDS_ERR_APLIST_NOT_FN_PARAM,
              count,fplist->name,id->name,name);
            err = TRUE;
            }
          else if(ParamIdent->type != fplist->type)
            {
            PRS_Error(IDS_ERR_APLIST_FN_PARAM_RETURN_TYPE,
              ParamIdent->name,id->name,fplist->name);
            err = TRUE;
            }
          else
            err = CheckTypeOfFPLists(id,count,ParamIdent,fplist);

          if(!err)
            {
            if((param = PRSI_NewExpr()) == NULL)
              {
              PRS_Error(IDS_ERR_APLIST_EXPR_MEM);
              err = TRUE;
              }
            else
              {
              param->operator = TokIdentifier;
              param->value.ident = ParamIdent;
              }

            }
          break;
        }

      /* Link parameter onto actual parameter list if everything ok.
      // If the parameter is for a function or proc with IDF_VARARGS
      // flag set then we want to link them onto the FRONT of the list
      // so they will end up being pushed right-left. Otherwise link
      // them onto the tail of the list so they are pushed left-right
      // as per normal.
      */

      if(param != NULL && !err)
        {
        param->aplist = NULL;
        if(blVarargs)
          {
          param->aplist = list;
          list = param;
          }
        else
          {
          *tail = param;
          tail = & param->aplist;
          fplist = fplist->next;
          }
        }


      tok = PRSI_LexGetToken();
      if(tok != TPComma)
        {
        blFinished = TRUE;
        if(!err)
          PRSI_LexPushBackToken(tok);
        }
      } /* end-while (!blFinished) */

    if(!err)
      err = PRSI_MustHave(TPRightParen);

    } /* end-else (there is an argument list */


  if(!err && !blVarargs && fplist != NULL)
    {
    PRS_Error(IDS_ERR_APLIST_TOO_FEW_PARAMS,id->name);
    err = TRUE;
    }

  if (err)
    {
    PRSI_FreeExpression(list);
    list = NULL;
    }

  *ppExpr = list;
  return err;
}





