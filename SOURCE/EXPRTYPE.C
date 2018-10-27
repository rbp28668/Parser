/*
// File:        EXPRTYPE.C
//
// Prefix:      NONE
//
// Description: Takes an expression tree and checks it for type consistency.
//
*/

#include <stdio.h>
#include <assert.h>

#include "prsif.h"
#include "parser.h"
#include "expr.h"
#include "error.h"
#include "malloc.h"
#include "lex.h"
#include "tokens.h"
#include "ident.h"
#include "aplist.h"

#define BINARY_OPERATOR(e,l,r) l=PRSI_ExpressionType(e->left);\
  r = PRSI_ExpressionType(e->right); \
  if (l==NULL || r==NULL) break;

#define UNARY_OPERATOR(e,l) l=PRSI_ExpressionType(e->right);\
  if (l==NULL) break;


#define INTEGER (&IntegerTypeDecl)
#define REAL    (&RealTypeDecl)
#define BOOLEAN (&BooleanTypeDecl)
#define STRING  (&StringTypeDecl)
#define ORDINAL (&OrdinalTypeDecl)

/**********************************************************************
* PRSI_ExprIsLvalue
* This returns TRUE iff the expression is an LValue.
**********************************************************************/
int PRSI_ExprIsLValue(ExprT *expr)
{
  int IsLValue = FALSE;

  if (expr == NULL) /* Trivial case for recursion */
    IsLValue = TRUE;
  else if (!(PRSI_ExprIsLValue(expr->left) && PRSI_ExprIsLValue(expr->right)))
    IsLValue = FALSE;
  else
    {
    switch (expr->operator)
      {
      case TSReturn:        /* returning a fn value */
      case TPPeriod:        /* Record field */
      case TPLeftBracket:   /* array reference */
        IsLValue = TRUE;
        break;

      case TokIdentifier:
        assert(expr->value.ident != NULL);
        IsLValue = (expr->value.ident->class == VariableIdentifier);
        break;

      default:
        IsLValue = FALSE;
    }
  }

  return IsLValue;
}


/**********************************************************************
* PRSI_IsAssignmentCompatible
* This checks the types of both sides of an assignement to see if
* the assignement should be allowed.  This returns the type of the
* assignement if ok, NULL if not ok. The assignment is
* Left := Right;
**********************************************************************/
TypeT* PRSI_IsAssignmentCompatible(TypeT* LeftType,TypeT* RightType)
{
  TypeT* type = NULL;
  
  assert(LeftType != NULL);
  assert(RightType != NULL);
  
  switch (LeftType->BasicType)
    {
    case IntegerType:
      if(RightType->BasicType == IntegerType)
        type = INTEGER;
      break;

    case RealType:
      if(RightType->BasicType == RealType ||
         RightType->BasicType == IntegerType) /* integer -> real promotion */
        type = REAL;
        break;

    case StringType:
      if(RightType->BasicType == StringType)
        type = STRING;
      break;

    case EnumeratedType:
      /* Enumerations are equivalent if they share the same base type: */
      if(RightType->BasicType == EnumeratedType)
        {
        if (LeftType->data.enumeration.BaseType ==
            RightType->data.enumeration.BaseType)
          {
          type = LeftType->data.enumeration.BaseType;
          }
        }

      break;

    case ArrayType:
      /* Arrays must match exactly */
      if (RightType == LeftType)
        type = LeftType;
      break;

    case RecordType:
      /* Records must match exactly */
      if (RightType == LeftType)
        type = LeftType;
      break;

    default:
      /* Either this is an unknown type or the left type is
      // a compound type made up as a bitwise or of a number
      // of types.
      */
      if((RightType->BasicType & LeftType->BasicType) == 0)
        PRS_Error(IDS_ERR_EXPRTYPE_UNKNOWN_ASSIGN_TYPE);
      else
        type = RightType;
      break;
    }

  return type;
}

/**********************************************************************
* CheckRelationalExpression
* This checks a relational expression:
* Integers can be compared with integer
* Reals can be compared with reals
* integers can be compared with reals and vice-versa
* enumerated types can be compared with the same type
* strings can be compared.
**********************************************************************/
static TypeT *CheckRelationalExpression(ExprT *expr)
{
  TypeT *type = NULL;
  TypeT *LeftType = NULL;
  TypeT *RightType = NULL;
  
  assert(expr != NULL);

  LeftType = PRSI_ExpressionType(expr->left);
  RightType = PRSI_ExpressionType(expr->right);

  if(LeftType != NULL && RightType != NULL)
    {
    /* Check for integral->real promotion */
    if (LeftType == REAL || RightType == REAL)
      {
      if(LeftType->BasicType == IntegerType)
        LeftType = REAL;

      if(RightType->BasicType == IntegerType)
        RightType = REAL;
      }

    switch (LeftType->BasicType)
      {
      case IntegerType:
        if(RightType->BasicType == IntegerType)
          type = BOOLEAN;
        else
          PRS_Error(IDS_ERR_EXPRTYPE_COMPARE_INTEGER_NON_NUMERIC);
        break;

      case RealType:
        if(RightType->BasicType == RealType)
          type = BOOLEAN;
        else
          PRS_Error(IDS_ERR_EXPRTYPE_COMPARE_REAL_NON_NUMERIC);
        break;

      case EnumeratedType:
        if(RightType->BasicType == EnumeratedType)
          {
          if(LeftType->data.enumeration.list ==
             RightType->data.enumeration.list)
            type = BOOLEAN;
          else
            PRS_Error(IDS_ERR_EXPRTYPE_COMPARE_DIFF_ENUMS);
          }
        else
          PRS_Error(IDS_ERR_EXPRTYPE_COMPARE_ENUM_DIFFERENT_TYPE);
        break;

      case StringType:
        if(RightType->BasicType == StringType)
          type = BOOLEAN;
        else
          PRS_Error(IDS_ERR_EXPRTYPE_COMPARE_STRING_NON_STRING);
        break;

      }
    }
  return type;
}

/**********************************************************************
* Function:   FunctionType
* This returns a pointer to the type of a function in a function
* call.  This is the normal defined return type for that function
* for most functions. Some CCALL functions may have a polymorphic
* return type (e.g. sqrt & sqr).  In this case the functions
* return the same type as their first actual parameter.
* Parameters:   expr is a function expression.
*
* Returns:      the type of the function expression.
**********************************************************************/
static TypeT* FunctionType(ExprT* expr)
{
  TypeT* type = NULL;

  assert(expr != NULL);
  assert(expr->operator == TKFunction);
  
  type = expr->value.ident->type;
  assert(type != NULL);
    
  switch(type->BasicType)
    {
    case IntegerType:
    case RealType:
    case StringType:
    case EnumeratedType:
    case ArrayType:
    case RecordType:
      /* Normal type: leave type as type of function */
      break;

    default:  /* not one of the above: must be a polymorphic type.
              // Where a function has a polymorphic return type it
              // is cast to be the same type as the first actual
              // parameter to the function.
              */
      type = PRSI_ExpressionType(expr->right);
      break;
    }
  return type;
}

/**********************************************************************
* PRSI_ExpressionType
* Takes an expression tree and checks it for type consistency.
* This returns the type of the tree.  Note that the assignment
* operator returns the type of its sub-trees.
**********************************************************************/
TypeT *PRSI_ExpressionType(ExprT *expr)
{
  TypeT *type = NULL;
  TypeT *LeftType = NULL;
  TypeT *RightType = NULL;
  TypeT *IndexType = NULL;

  if (expr == NULL)
    {
    return NULL;
    }

  switch (expr->operator)
    {
    case TokInt:
      type = INTEGER;
      break;

    case TokFloat:
      type = REAL;
      break;

    case TokString:
      type = STRING;
      break;

    case TokIdentifier:
      type = expr->value.ident->type;
      break;

    case TSReturn:     /* := returning a value from a function */
      type = expr->value.ident->type;
      break;

    case TKFunction:  /* Function call */
      type = FunctionType(expr);
      break;

    case TPEquals:
    case TPNotEquals:
    case TPLessThan:
    case TPGreaterThan:
    case TPLessEquals:
    case TPGreaterEquals:
      type = CheckRelationalExpression(expr);
      break;

    case TPPlus:
    case TPMinus:
    case TPTimes:

      BINARY_OPERATOR(expr,LeftType,RightType);

      /* Do any integer->real promotion */
      if (LeftType == REAL || RightType == REAL)
        {
        if(LeftType->BasicType == IntegerType)
          LeftType = REAL;

        if(RightType->BasicType == IntegerType)
          RightType = REAL;
        }

      switch (LeftType->BasicType)
        {
        case IntegerType:
          if(RightType->BasicType != IntegerType)
            PRS_Error(IDS_ERR_EXPRTYPE_TYPE_MISMATCH);
          else
            type = INTEGER;
          break;

        case RealType:
          if(RightType->BasicType != RealType)
            PRS_Error(IDS_ERR_EXPRTYPE_TYPE_MISMATCH);
          else
            type = REAL;
          break;

        default:
          PRS_Error(IDS_ERR_EXPRTYPE_TYPE_PLUS_MINUS_TIMES);
          break;
        }
      break;

    case TKDiv:
    case TKMod:
      BINARY_OPERATOR(expr,LeftType,RightType);

      if (LeftType != INTEGER || RightType != INTEGER)
        PRS_Error(IDS_ERR_EXPRTYPE_TYPE_DIV_MOD);
      else
        type = INTEGER;

      break;

    case TPDivide:
      BINARY_OPERATOR(expr,LeftType,RightType);
      if((LeftType->BasicType == IntegerType ||
          LeftType->BasicType == RealType) &&
          (RightType->BasicType == IntegerType ||
          RightType->BasicType == RealType))
        type = REAL;
      else
        PRS_Error(IDS_ERR_EXPRTYPE_TYPE_DIVIDE);
      break;

    case TKAnd:
    case TKOr:
      BINARY_OPERATOR(expr,LeftType,RightType);

      if (LeftType == BOOLEAN)
        {
        if (RightType == BOOLEAN)
          type = BOOLEAN;
        else
          PRS_Error(IDS_ERR_EXPRTYPE_TYPE_MISMATCH_AND_OR);
        }
      else if(LeftType->BasicType == IntegerType)
        {
        if(RightType->BasicType == IntegerType)
          type = INTEGER;
        else
          PRS_Error(IDS_ERR_EXPRTYPE_TYPE_MISMATCH_AND_OR);
        }
      else
        PRS_Error(IDS_ERR_EXPRTYPE_TYPE_AND_OR);
      break;

    case TKNot:
      UNARY_OPERATOR(expr,RightType);
      
      if(RightType == BOOLEAN)
        type = BOOLEAN;
      else if(RightType->BasicType == IntegerType)
        type = INTEGER;
      else
        PRS_Error(IDS_ERR_EXPRTYPE_TYPE_NOT);
      break;

    case TSNegate:
      UNARY_OPERATOR(expr,RightType);
      if(RightType != INTEGER && RightType != REAL)
        {
        PRS_Error(IDS_ERR_EXPRTYPE_MINUS_NAN);
        type = NULL;
        }
      else
        type = RightType;

      break;

    case TPPeriod:
      BINARY_OPERATOR(expr,LeftType,RightType);
      if(LeftType->BasicType != RecordType)
        {
        PRS_Error(IDS_ERR_EXPRTYPE_LHS_PERIOD_NOT_RECORD);
        type = NULL;
        }
      else
        type = RightType;

      break;

    case TPLeftBracket:
      BINARY_OPERATOR(expr,LeftType,RightType);
      if(!PRSI_IsOrdinalType(RightType))
        PRS_Error(IDS_ERR_EXPRTYPE_ARRAY_IDX_NOT_ORDINAL);
      else
        {
        if (LeftType->BasicType != ArrayType)
          {
          PRS_Error(IDS_ERR_EXPRTYPE_LHS_BRACKET_NOT_ARRAY);
          type = NULL;
          }
        else
          {
          IndexType = LeftType->data.array.IndexType;
          if(IndexType->BasicType != RightType->BasicType)
            {
            PRS_Error(IDS_ERR_EXPRTYPE_INDEX_WRONG_TYPE);
            type = NULL;
            }
          else if (IndexType->BasicType == EnumeratedType &&
            IndexType->data.enumeration.list != RightType->data.enumeration.list)
            {
            PRS_Error(IDS_ERR_EXPRTYPE_INDEX_WRONG_ENUM);
            type = NULL;
            }
          else
            type = LeftType->data.array.Type;
          }
        }
      break;

    case TPAssignment:
      BINARY_OPERATOR(expr,LeftType,RightType);
      type = PRSI_IsAssignmentCompatible(LeftType,RightType);
      if(type == NULL)
        {
        PRS_Error(IDS_ERR_EXPRTYPE_ASSIGNMENT_TYPE);
        }
      break;

    default:
      PRS_Error(IDS_ERR_EXPRTYPE_UNKNOWN_OP,expr->operator);
      break;

    }/* End switch expr->operator */

  return type;
}



