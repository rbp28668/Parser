/* 
// File:          EXPR.C
// 
// Prefix:        NONE
// 
// Description:   Expression handling
//
* Expressions
*
* Operators evaluate left to right for equal precedence
* Precedence  4 not
*   3 * / div mod and
*   2 + - or
*   1 = <> > < >= <=
*
*expression = simple-expression [ relational-operator simple-expression ].
*
*simple-expression = [sign] term { adding-operator term }.
*
*term = factor { multiplying-operator factor }.
*
*factor = variable | unsigned-constant | function-designator | 
* "(" expression ")" | "not" factor.
*
*unsigned-constant = unsigned-integer | unsigned-real | 
* string | constant-identifier.
*
*relational-operator = "=" | "<>" | "<" | ">" | "<=" | ">=".
*
*adding-operator = "+" | "-" | "or".
*
*multiplying-operator = "*" | "/" | "div" | "mod" | "and".
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "prsif.h"
#include "parser.h"
#include "lex.h"
#include "tokens.h"
#include "musthave.h"
#include "ident.h"
#include "error.h"
#include "expr.h"
#include "malloc.h"
#include "aplist.h"

static ExprT *_Expression(void);

/**********************************************************************
* PRSI_FreeExpression
* Frees up an expression tree
**********************************************************************/
void PRSI_FreeExpression(ExprT *expr)
{
  if (expr == NULL)
    return;

  /* Strings have extra memory allocated to them so we need
  // to free this.
  */  
  if(expr->operator == TokString)
    free(expr->value.str);
    
  PRSI_FreeExpression(expr->left);
  PRSI_FreeExpression(expr->right);
  PRSI_FreeExpression(expr->aplist);
  free(expr);
  return;
}

/**********************************************************************
* PRSI_NewExpr
* Creates a new, empty expression
**********************************************************************/
ExprT *PRSI_NewExpr(void)
{
  ExprT *expr = NULL;

  expr = malloc(sizeof(ExprT));
  if (expr == NULL)
    {
    PRS_Error(IDS_ERR_EXPR_MEM);
    }
  else
    {
    expr->operator = 0;
    expr->aplist = NULL;
    expr->left = NULL;
    expr->right = NULL;
    expr->value.ident = NULL;
    }
  return expr;
}

/**********************************************************************
* Function
* Parses a function call
*
* function-designator = function-identifier [ actual-parameter-list ].
* 
* function-identifier = identifier.
*
**********************************************************************/
static ExprT *Function(IdentifierT *fn)
{
  ExprT *expr = NULL;
  int err = FALSE;

  assert(fn != NULL);
  
  expr = PRSI_NewExpr();
  if(expr == NULL)
    err = TRUE;
  else
    {
    expr->operator = TKFunction;
    expr->value.ident = fn;
    }

  /* Look for formal parameter list & tie it to fn's right ptr */
  if (!err)
    {
    if (fn->link != NULL) /* fn has formal parameter list */
      {
      err = PRSI_ActualParameterList(fn,&expr->right);
      }
    }

  if (err)
    {
    PRSI_FreeExpression(expr);
    expr = NULL;
    }
  
  return expr;
}

/**********************************************************************
* RecordFieldIdentifier
* Reads in the identifier for a record field and makes sure that
* it belongs to the parent record.  The identifier is returned in
* an expression node.
*
* field-identifier = identifier.
* 
**********************************************************************/
static ExprT *RecordFieldIdentifier(IdentifierT *parent)
{
  ExprT *expr = NULL;
  IdentifierT *field = NULL;
  char *name = NULL;
  int err = FALSE;

  assert(parent != NULL);

  err = PRSI_MustHave(TokIdentifier);

  if (!err)
    {
    name = PRSI_LexGetIdentifier();
    field = PRSI_FindRecordField(parent,name);

    if (field == NULL)
      {
      assert(parent->name != NULL);
      PRS_Error(IDS_ERR_EXPR_NOT_RECORD_MEMBER,name,parent->name);
      err = TRUE;
      }
    }

  if (!err)
    {
    expr = PRSI_NewExpr();
    if (expr == NULL)
      err = TRUE;
    else
      {
      expr->operator = TokIdentifier;
      expr->value.ident = field;
      }
    }

  if (err)
    expr = NULL;

  return expr;
}


/**********************************************************************
* PRSI_Variable
* Deals with simple and compound variables e.g. arrays & records
* The initial identifier has already been read in and is referenced
* by ident.
*
* variable = entire-variable | component-variable ;No referenced variable (no ptrs).
* 
* entire-variable = variable-identifier.
* 
* component-variable = indexed-variable | field-designator. ; no file-buffer.
* 
* field-designator = record-variable "." field-identiifer.
* 
* record-variable = variable.
* 
* indexed-variable = array-variable "[" expression "]". ; 1-D arrays only
* 
* array-variable = variable.
*
**********************************************************************/
ExprT *PRSI_Variable(IdentifierT *ident)
{
  ExprT *expr = NULL;
  ExprT *left = NULL;
  int tok;
  int err = FALSE;

  assert(ident != NULL);

  expr = PRSI_NewExpr();
  if (expr != NULL)
    {
    expr->operator = TokIdentifier;
    expr->value.ident = ident;
    
    tok = PRSI_LexGetToken();

    while (tok == TPLeftBracket || tok == TPPeriod)
      {
      if (tok == TPLeftBracket)   /* an array */
        {
        left = expr;
        expr = PRSI_NewExpr();
        if(expr == NULL)
          {
          PRSI_FreeExpression(left);
          break;
          }
        expr->operator = tok;
        expr->left = left;
        expr->right = _Expression();
        err = PRSI_MustHave(TPRightBracket);

        if (err || (expr->right == NULL))
          {
          PRSI_FreeExpression(left);
          free(expr);
          expr = NULL;
          break;
          }
        }
      else     /* Must be a . hence a record field */
        {
        left = expr;
        expr = PRSI_NewExpr();
        if(expr == NULL)
          {
          PRSI_FreeExpression(left);
          break;
          }

        expr->operator = tok;
        expr->left = left;
        expr->right = RecordFieldIdentifier(ident);
        if (expr->right == NULL)
          {
          PRSI_FreeExpression(left);
          free(expr);
          expr = NULL;
          break;
          }

        ident = expr->right->value.ident; /* in case of more sub-fields */
        }


      tok = PRSI_LexGetToken();
      }
    if(expr != NULL)
      PRSI_LexPushBackToken(tok);
    }

  return expr;
}

/**********************************************************************
* Factor
* factor = variable | unsigned-constant | function-designator | 
* "(" expression ")" | "not" factor.
* unsigned-constant = unsigned-integer | unsigned-real | 
*  string | constant-identifier.
**********************************************************************/
static ExprT *Factor(void)
{
  ExprT *expr = NULL;
  int tok;
  IdentifierT *ident = NULL;
  char *name = NULL;
  int err = FALSE;

  tok = PRSI_LexGetToken();

  switch (tok)
    {
    case TPLeftParen:
      expr = _Expression();
      err = PRSI_MustHave(TPRightParen);
      if (err)
        {
        PRSI_FreeExpression(expr);
        expr = NULL;
        }
      break;

    case TKNot:
      expr = PRSI_NewExpr();
      if (expr != NULL)
        {
        expr->operator = tok;
        expr->right = Factor();

        if(expr->right == NULL)
          {
          PRSI_FreeExpression(expr->right);
          free(expr);
          expr = NULL;
          }
        }
      break;

    case TokInt:
      expr = PRSI_NewExpr();
      if(expr != NULL)
        {
        expr->operator = tok;
        expr->value.integer = PRSI_LexGetLong();
        }
      break;


    case TokFloat:
      expr = PRSI_NewExpr();
      if(expr != NULL)
        {
        expr->operator = tok;
        expr->value.real = (float)PRSI_LexGetDouble();
        }
      break;
  
    case TokString:
      expr = PRSI_NewExpr();
      if(expr != NULL)
        {
        expr->operator = tok;
        expr->value.str = PRS_CreateString(PRSI_LexGetString());
        }
      break;

    case TokIdentifier:
      name = PRSI_LexGetIdentifier();

      ident = PRSI_FindIdentifier(name);
      if (ident == NULL)
        {
        PRS_Error(IDS_ERR_EXPR_UNKNOWN_IDENT,name);
        expr = NULL;
        }
      else
        {
        switch (ident->class)
          {
          case VariableIdentifier: 
            expr = PRSI_Variable(ident);
            break;

          case ConstantIdentifier:
            expr = PRSI_NewExpr();
            if(expr != NULL)
              {
              expr->operator = tok;
              expr->value.ident = ident;
              }
            break;

          case FunctionIdentifier:
            expr = Function(ident);
            break;

          default:
            PRS_Error(IDS_ERR_EXPR_NOT_CONST_VAR_FN,name);
            expr = NULL;
            break;
          }
        }
      break;

    default:
      {
      PRS_Error(IDS_ERR_EXPR_SYNTAX,PRSI_LexTranslateToken(tok));
      expr = NULL;
      }
    }

  return expr;
}

/**********************************************************************
* Term
* term = factor { multiplying-operator factor }.
**********************************************************************/
static ExprT *Term(void)
{
  ExprT *expr = NULL;
  ExprT *left = NULL;
  int tok;
  int done = FALSE;

  expr = Factor();


  /* Now look for any {multiplying-operator factor} */
  if(expr != NULL)
    {
    done = FALSE;

    do
      {
      tok = PRSI_LexGetToken();
      switch (tok)
        {
        case TPTimes:
        case TPDivide:
        case TKDiv:
        case TKMod:
        case TKAnd:
          left = expr;

          expr = PRSI_NewExpr();
          if (expr == NULL)
            {
            PRSI_FreeExpression(left);
            }
          else
            {
            expr->operator = tok;
            expr->left = left;
            expr->right = Factor();

            if (expr->right == NULL)
              {
              PRSI_FreeExpression(expr->left);
              free(expr);
              expr = NULL;
              }
            }

          break;

        default:
          PRSI_LexPushBackToken(tok);
          done = TRUE;
          break;
        }
      }
    while (expr != NULL && !done);
    }

  return expr;
}

/**********************************************************************
* SimpleExpression
* simple-expression = [sign] term { adding-operator term }.
**********************************************************************/
static ExprT *SimpleExpression(void)
{
  ExprT *expr = NULL;
  ExprT *left = NULL;
  ExprT *right = NULL;
  int tok;
  int sign = 1;
  int done = FALSE;


  /* Look for optional sign */
  tok = PRSI_LexGetToken();

  switch (tok)
    {
    case TPPlus:  /* do nothing ! */
      break;

    case TPMinus:
      sign = -1;
      break;

    default:
      PRSI_LexPushBackToken(tok);
      break;
    }


  expr = Term();


  /* If there was a leading - sign then we want to negate the term. */
  if (expr != NULL)
    {
    if (sign == -1)
      {
      right = expr;
      expr = PRSI_NewExpr();
      if (expr == NULL)
        {
        PRSI_FreeExpression(right);
        }
      else
        {
        expr->operator = TSNegate;
        expr->right = right;
        }
      }
    }


  /* Now look for any {adding-operator term} */
  if(expr != NULL)
    {
    done = FALSE;

    do
      {
      tok = PRSI_LexGetToken();
      switch (tok)
        {
        case TPPlus:
        case TPMinus:
        case TKOr:
          left = expr;

          expr = PRSI_NewExpr();
          if (expr == NULL)
            {
            PRSI_FreeExpression(left);
            }
          else
            {
            expr->operator = tok;
            expr->left = left;
            expr->right = Term();

            if (expr->right == NULL)
              {
              PRSI_FreeExpression(expr->left);
              free(expr);
              expr = NULL;
              }
            }

          break;

        default:
          PRSI_LexPushBackToken(tok);
          done = TRUE;
          break;
        }
      }
    while (expr != NULL && !done);
    }

  return expr;
}


/**********************************************************************
* _Expression
* expression = simple-expression [ relational-operator simple-expression ].
**********************************************************************/
static ExprT *_Expression(void)
{
  ExprT *expr = NULL;
  ExprT *left = NULL;
  int tok;

  expr = SimpleExpression();

  if(expr != NULL)
    {

    tok = PRSI_LexGetToken();

    /* Look for relational operators */
    switch (tok)
      {
      case TPEquals:
      case TPLessThan:
      case TPGreaterThan:
      case TPLessEquals:
      case TPGreaterEquals:
      case TPNotEquals:

        left = expr;

        expr = PRSI_NewExpr();
        if (expr == NULL)
          {
          PRSI_FreeExpression(left);
          expr = NULL;
          break;
          }

        expr->operator = tok;
        expr->left = left;
        expr->right = SimpleExpression();

        /* If the rightmost simple expression failed then
        *  free the left branch & this node.
        */
        if(expr->right == NULL)
          {
          PRSI_FreeExpression(expr->left);
          free(expr);
          expr = NULL;
          }
        break;

      default:     /* not a relational operator */
        PRSI_LexPushBackToken(tok);
        break;
      }
    }

  return expr;
}



/**********************************************************************
* PRSI_ParseExpression
* Parses a complete expression and returns a parse tree for the
* expression or NULL if an error occured. This does syntax checking
* but no type or parameter checking.
**********************************************************************/
ExprT *PRSI_ParseExpression(void)
{
  return _Expression();
}
