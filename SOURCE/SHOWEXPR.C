/* 
// File:          SHOWEXPR.C
// 
// Prefix:        NONE
// 
// Description:   Diagnostic aid for dumping a parsed expression.
//
*/
#include <stdio.h>
#include <stdlib.h>
#include "prsif.h"
#include "parser.h"
#include "lex.h"
#include "tokens.h"
#include "musthave.h"
#include "ident.h"
#include "error.h"
#include "expr.h"
#include "malloc.h"
#include "printf.h"

#if !defined NDEBUG && defined SHOW

static void _ShowExpression(ExprT *expr);


/**********************************************************************
* _ShowExpression
* Recursively traverses an expression tree displaying the expression.
**********************************************************************/
static void _ShowExpression(ExprT *expr)
{
  if (expr != NULL)
    {

    if (expr->left != NULL)
      {
      PRSI_printf(" (");
      _ShowExpression(expr->left);
      PRSI_printf(" )");
      }

    switch (expr->operator)
      {
      case TSReturn:
        PRSI_printf (" RETURN: %s",expr->value.ident->name);
        break;

      case TSNegate:
        PRSI_printf(" -");
        break;

      case TokInt:
        PRSI_printf(" %ld",expr->value.integer);
        break;

      case TokFloat:
        PRSI_printf(" %f",expr->value.real);
        break;

      case TokString:
        PRSI_printf(" '%s'",expr->value.str->text);
        break;

      case TokIdentifier:
        PRSI_printf(" %s",expr->value.ident->name);
        break;

      case TKFunction:
      case TKProcedure:
        PRSI_printf(" %s %s",PRSI_LexTranslateToken(expr->operator),
          expr->value.ident->name);
        break;

      default:
        PRSI_printf(" %s",PRSI_LexTranslateToken(expr->operator));
        break;
      }

    if (expr->right != NULL)
      {
      PRSI_printf(" (");
      _ShowExpression(expr->right);
      PRSI_printf(" )");
      }

    if(expr->aplist != NULL)
      {
      PRSI_printf(",");
      _ShowExpression(expr->aplist);
      }
    }
  

  return;
}


/**********************************************************************
* PRSI_ShowExpression
* Is the entry point for displaying an expression. _ShowExpression
* does most of the work.
**********************************************************************/
void PRSI_ShowExpression(ExprT *expr)
{
  _ShowExpression(expr);
  PRSI_printf("\n\n");
}

#endif
