/* 
// File:        SIMPSTAT.C
// 
// Prefix:      NONE
// 
// Description: Code for parsing a simple statement
//
*/

#include <stdio.h>
#include <assert.h>
#include "prsif.h"
#include "lex.h"
#include "tokens.h"
#include "musthave.h"
#include "parser.h"
#include "expr.h"
#include "ident.h"
#include "error.h"
#include "malloc.h"
#include "aplist.h"
#include "iproc.h"
#include "plant.h"


/**********************************************************************
* AssignementStatement
* Parses an assignement statement.
* ident is the variable identifier.
*
* assignment-statement = (variable | function-identifier) ":=" expression.
*
**********************************************************************/
static int AssignmentStatement(IdentifierT *ident)
{
  int err = FALSE;
  ExprT *expr = NULL;
  ExprT *right = NULL;
  ExprT *lvalue = NULL;

  assert(ident != NULL);
  
  if(ident->class == FunctionIdentifier)
    {
    lvalue = PRSI_NewExpr();
    if(lvalue != NULL)
      {
      lvalue->operator = TSReturn; /* special to mark return value */
      lvalue->value.ident = ident; /* remember fn for type checking */
      }
    else
      PRS_Error(IDS_ERR_SIMPSTAT_FN_RETURN_MEM);
    }
  else
    lvalue = PRSI_Variable(ident);

  err = (lvalue == NULL);
        
  if(!err)
    err = PRSI_MustHave(TPAssignment);

  if(!err)
    {
    expr = PRSI_ParseExpression();
    if (expr == NULL)
      {
      err = TRUE;
      PRSI_FreeExpression(lvalue);
      lvalue = NULL;
      }
    }

  if(!err)
    {
    right = expr;
    expr = PRSI_NewExpr();
    if(expr == NULL)
      {
      err = TRUE;
      PRS_Error(IDS_ERR_SIMPSTAT_ASSIGNMENT_MEM);
      PRSI_FreeExpression(right);
      PRSI_FreeExpression(lvalue);
      right = lvalue = NULL;
      }
    else
      {
      expr->operator = TPAssignment;
      expr->left = lvalue;
      expr->right = right;
      }
    }


  if(!err)
    err = (PRSI_ExpressionType(expr) == NULL); /* check type consistency */

  if(!err)
    {
    #if !defined NDEBUG && defined SHOW
    PRSI_ShowExpression(expr);
    #endif

    PRSI_PlantExpression(expr,TRUE);
    }
  PRSI_FreeExpression(expr);

  return err;
}

/**********************************************************************
* PRSI_SimpleStatement
* This parses a simple statement. The token for the identifier has
* already been read in.
*
* simple-statement = assignment-statement | procedure-statement.
*
* Returns FALSE for success, TRUE for failure.
**********************************************************************/
int PRSI_SimpleStatement(void)
{
  int err = FALSE;
  char *name = NULL;
  IdentifierT *ident = NULL;
  ExprT *expr = NULL;
  
  name = PRSI_LexGetIdentifier();
  ident = PRSI_FindIdentifier(name);
  if (ident == NULL)
    {
    PRS_Error(IDS_ERR_SIMPSTAT_MISSING_IDENT,name);
    err = TRUE;
    }

  if (!err)
    {
    switch (ident->class)
      {
      case ConstantIdentifier:
        assert(ident->name != NULL);
        PRS_Error(IDS_ERR_SIMPSTAT_LVALUE_IS_CONSTANT,ident->name);
        err = TRUE;
        break;

      case TypeIdentifier:
        assert(ident->name != NULL);
        PRS_Error(IDS_ERR_SIMPSTAT_LVALUE_IS_TYPE,ident->name);
        err = TRUE;
        break;

      case VariableIdentifier:
        err = AssignmentStatement(ident);
        break;

      case FunctionIdentifier:
        if (PRSI_InLocalScope())
          {
          err = AssignmentStatement(ident);
          }
        else
          {
          PRS_Error(IDS_ERR_SIMPSTAT_LVALUE_IS_FUNCTION,ident->name);
          err = TRUE;
          }
        break;

      case ProcedureIdentifier:
        expr = PRSI_NewExpr();
        if(expr == NULL)
          {
          PRS_Error(IDS_ERR_SIMPSTAT_PROC_MEM);
          err = TRUE;
          }
                 
        if(expr != NULL)
          {
          expr->operator = TKProcedure;

          if (ident->link)    /* then has a parameter list */
            err = PRSI_ActualParameterList(ident,&expr->right);
          
          if(!err)
            {
            expr->value.ident = ident;

            #if !defined NDEBUG && defined SHOW
            PRSI_ShowExpression(expr);
            #endif

            err = PRSI_PlantExpression(expr,FALSE);
            }

          PRSI_FreeExpression(expr);
          }
        else
          err = TRUE;

        break;

      case FieldIdentifier:
        assert(ident->name != NULL);
        PRS_Error(IDS_ERR_SIMPSTAT_LVALUE_IS_RECORD_FIELD,ident->name);
        err = TRUE;
        break;

      }
    }

  return err;

}

