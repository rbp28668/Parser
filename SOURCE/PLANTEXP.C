/* 
// File:          PLANTEXPR.C
// 
// Prefix:
// 
// Description:   To plant expressions.
//
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "parser.h"
#include "expr.h"
#include "prsif.h"
#include "iproc.h"
#include "plant.h"
#include "tokens.h"
#include "error.h"
#include "malloc.h"

#if !defined NDEBUG && defined SHOW
#include "printf.h"
#endif


/**********************************************************************
* PRSI_IsSimpleType
* returns TRUE if the type is a simple type, FALSE if compound.
**********************************************************************/
int PRSI_IsSimpleType(TypeT* type)
{
  int IsSimple = TRUE;
  assert(type != NULL);
  if(type->BasicType == RecordType ||
     type->BasicType == ArrayType)
    IsSimple = FALSE;
  return IsSimple;
}

/**********************************************************************
* PlantConstantIdentifier
* Plants the constant value of a constant identifier.
**********************************************************************/
static int PlantConstantIdentifier(IdentifierT* id)
{
  int err = FALSE;

  if (id==NULL)
    {
    PRS_Error(IDS_ERR_PLANTEXP_NULL_CONST_ID);
    err = TRUE;
    }
  else if (id->type == NULL)
    {
    PRS_Error(IDS_ERR_PLANTEXP_NULL_CONST_TYPE);
    err = TRUE;
    }
  else
    {
    switch (id->type->BasicType)
      {
      case IntegerType:
      case EnumeratedType:
        PRSI_PlantInteger(id->value.ordinal);
        break;

      case RealType:
        PRSI_PlantFloat(id->value.real);
        break;

      case StringType:
        PRSI_PlantAddr(id->value.string);
        break;

      default:
        PRS_Error(IDS_ERR_PLANTEXP_INVALID_CONST_TYPE);
        err = TRUE;
        break;
      }
    }
  return err;
}

/**********************************************************************
* PlantIdentifier
* This plants an identifier. Compound types are always planted as
* addresses, simple types will be planted as fetching their values
* (if blValue is true), or left as addresses.
* Procedures and functions are planted as their addresses (may
* have to get them off the stack if they are already formal params)
**********************************************************************/
static int PlantIdentifier(IdentifierT* id, int blValue)
{
  int err = FALSE;

  if(id == NULL)
    {
    PRS_Error(IDS_ERR_PLANTEXP_NULL_ID);
    err = TRUE;
    }
  else
    {
    switch (id->class)
      {
      case ConstantIdentifier:
        err = PlantConstantIdentifier(id);
        break;

      case ProcedureIdentifier:     /* Procedure parameter */
      case FunctionIdentifier:      /* Function parameter */
        if(id->flags & IDF_CCALL)   /* is it a 'C' function */
          {
          PRS_Error(IDS_ERR_PLANTEXP_C_NOT_AP);
          err = TRUE;
          }
        else                        /* not a C fn */
          {
          if(id->scope == ValueParamScope)  /* then get addr off stack */
            {
            PRSI_PlantInteger(id->value.FPOffset);
            PRSI_Plant(FPLD);
            }
          else                              /* directly stuff addr */
            PRSI_PlantAddr(id->value.addr);
          }
        break;

      case VariableIdentifier:
        switch (id->scope)
          {
          case VarParamScope:
            PRSI_PlantInteger(id->value.FPOffset);
            PRSI_Plant(FPLD);
            assert(id->type != NULL);
            if(blValue && PRSI_IsSimpleType(id->type))
              PRSI_Plant(LD);  /* dereference ptr */
            break;

          case ValueParamScope:
          case LocalScope:
            assert(id->type != NULL);
            if(blValue && PRSI_IsSimpleType(id->type))
              {
              PRSI_PlantInteger(id->value.FPOffset);  /* load value */
              PRSI_Plant(FPLD);
              }
            else
              {
              PRSI_PlantInteger(id->value.FPOffset); /* load addr */
              PRSI_Plant(FPADD);
              }
            break;

          case GlobalScope:
            assert(id->type != NULL);
            if(PRSI_IsSimpleType(id->type))
              {
              PRSI_PlantAddr(&(id->value));
              if(blValue)
                PRSI_Plant(LD);  /* get contents */
              }
            else  /* Compound type */
              PRSI_PlantAddr(id->value.addr);
            break;

          default:
            PRS_Error(IDS_ERR_PLANTEXP_SCOPE,id->scope);
            err = TRUE;
            break;
          }
        break;

      default:
        PRS_Error(IDS_ERR_PLANTEXP_INVALID_CLASS);
        err = TRUE;
        break;
      }
    }
  return err;
}

/**********************************************************************
* PRSI_PlantCompoundVariable
* Plants array and record variables.
**********************************************************************/
int PRSI_PlantCompoundVariable(ExprT *expr)
{
  int err = FALSE;
  TypeT* type = NULL;
  TypeT* IdxType = NULL;
  long lbnd,ubnd;
  IdentifierT* id = NULL;

  if( expr == NULL)
    {
    PRS_Error(IDS_ERR_PLANTEXP_NULL_COMPOUND_EXPR);
    err = TRUE;
    }
  else
    {
    switch (expr->operator)
      {
      case TPLeftBracket:   /* an array */
        {
        assert(expr->left != NULL);
        assert(expr->right != NULL);
        PRSI_PlantExpression(expr->left,FALSE);  /* The array base */
        PRSI_PlantExpression(expr->right,TRUE);  /* the index */

        type = PRSI_ExpressionType(expr->left);
        assert(type != NULL);
        assert(type->BasicType == ArrayType);

        IdxType = type->data.array.IndexType;
        assert(IdxType != NULL);
        assert(PRSI_IsOrdinalType(IdxType));

        /* Bound check array access */
        err = PRSI_GetOrdinalBounds(IdxType,&lbnd,&ubnd);
        PRSI_PlantInteger(lbnd);
        PRSI_PlantInteger(ubnd);
        PRSI_Plant(BND);

        /* Convert index into byte offset and add to base addr*/
        if(lbnd != 0)
          {
          PRSI_PlantInteger(lbnd); /* subtract lower bound */
          PRSI_Plant(SUB);
          }
        assert(type->data.array.Type != NULL);
        PRSI_PlantInteger(type->data.array.Type->size);
        PRSI_Plant(MULT);
        PRSI_Plant(ADD);
        }
      break;

      case TPPeriod:    /* a record */
        assert(expr->left != NULL);
        assert(expr->right != NULL);
        PRSI_PlantExpression(expr->left,FALSE);  /* record base addr */
        assert(expr->right->operator == TokIdentifier);
        id = expr->right->value.ident;
        assert(id->class == FieldIdentifier);
        PRSI_PlantInteger(id->value.offset);     /* offset of field */
        PRSI_Plant(ADD);                         /* add field offset to record base */
        break;

      }
    }
  return err;
}

/**********************************************************************
* PlantFPAddr
* This plants a procedure or function address.
* id is the identifier.
**********************************************************************/
static int PlantFPAddr(IdentifierT *ident)
{
  int err = FALSE;
  assert(ident != NULL);
  if(ident->scope == ValueParamScope)
    {
    PRSI_PlantInteger(ident->value.FPOffset);
    PRSI_Plant(FPLD);
    }
  else if(ident->scope == GlobalScope)
    {
    if(ident->value.addr == 0)    /* undefined fn */
      {
      PRSI_PlantAddr(&(ident->value));
      PRSI_Plant(LD);                  /* so indirect via its identifier */
      }
    else
      PRSI_PlantAddr(ident->value.addr);
    }
  else
    {
    PRS_Error(IDS_ERR_PLANTEXP_INVALID_PROC_FN_SCOPE);
    err = TRUE;
    }
  return err;
}

/**********************************************************************
* PlantAPList
* Plants the actual parameter list for a function or procedure.
**********************************************************************/
static int PlantAPList(ExprT* expr)
{
  int err = FALSE;
  ExprT* aplist = NULL;
  IdentifierT* id = NULL;
  IdentifierT* fplist = NULL;
  TypeT* type = NULL;
  TypeT* apType = NULL;
  long lbnd,ubnd;
  int nArgCount = 0;
  int blVarargs = 0;

  if(expr == NULL)
    {
    PRS_Error(IDS_ERR_PLANTEXP_NULL_APLIST_EXPR);
    err = TRUE;
    }
  else
    {
    aplist = expr->right;
    id = expr->value.ident;
    fplist = id->link;

    assert(id != NULL);
    blVarargs = (id->flags & IDF_VARARGS) != 0;

    while (aplist != NULL && fplist != NULL)
      {
      ++nArgCount;                  /* count #of args pushed */
      switch (fplist->class)
        {
        case VariableIdentifier:
          switch (fplist->scope)    /* must be either value or var param */
            {
            case ValueParamScope:
              #if !defined NDEBUG && defined SHOW
              PRSI_printf("Planting (value) Parameter %s\n",fplist->name);
              #endif

              PRSI_PlantExpression(aplist,TRUE);
              type = fplist->type;  /* do type/bounds checking against
                                    // formal parameter
                                    */
              assert(type != NULL);
              if(PRSI_IsSimpleType(type))
                {
                /* Ordinal types may need bound checking */
                if(PRSI_IsOrdinalType(type))
                  {
                  PRSI_GetOrdinalBounds(type,&lbnd,&ubnd);
                  if(lbnd != -LONG_MAX || ubnd != LONG_MAX)
                    {
                    PRSI_PlantInteger(lbnd);
                    PRSI_PlantInteger(ubnd);
                    PRSI_Plant(BND);
                    }
                  }
                /* Check for integer to real promotion for value param */
                else if(type->BasicType == RealType)
                  {
                  apType = PRSI_ExpressionType(aplist);
                  if(apType->BasicType == IntegerType)
                    PRSI_Plant(ITOF);  /* promote integer to real */
                  }
                  
                else if(type->BasicType == StringType)
                  {
                  PRSI_Plant(SCLONE);
                  }

                PRSI_Plant(TOR); /* single value to return stack */
                }
              else
                {
                /* A compound variable: need to allocate a chunk of memory on
                // the stack and then move bytes from source to the allocated
                // region.
                */
                PRSI_PlantInteger(type->size);
                PRSI_Plant(DUP);
                PRSI_Plant(ALLOC);
                PRSI_Plant(SWAP);
                PRSI_Plant(MOV);
                }

              /* Some value parameters for 'C' call may be polymorphic.
              // in this case the formal parameter should have the
              // IDF_PLANT_TYPE bit set in its flags word. If this
              // bit is set we want to push a pointer to the type of
              // the actual parameter. This allows the called 'C' function
              // to work out what it is getting.
              */
              if(fplist->flags & IDF_PUSH_TYPE)
                {
                apType = PRSI_ExpressionType(aplist);
                PRSI_PlantAddr(apType);
                PRSI_Plant(TOR);
                }

              break;

            case VarParamScope:
              #if !defined NDEBUG && defined SHOW
              PRSI_printf("Planting (var) Parameter %s\n",fplist->name);
              #endif
              PRSI_PlantExpression(aplist,FALSE); /* want address*/
              PRSI_Plant(TOR);                   /* copy addr to return stack */
              break;

            default:
              PRS_Error(IDS_ERR_PLANTEXP_ILLEGAL_FP_SCOPE);
              err = TRUE;
              break;
            }
          break;

        case ProcedureIdentifier:       /* proc argument */
        case FunctionIdentifier:
          #if !defined NDEBUG && defined SHOW
            if(fplist->class == ProcedureIdentifier)
              PRSI_printf("Planting procedure parameter %s\n",fplist->name);
            else
              PRSI_printf("Planting function parameter %s\n",fplist->name);
          #endif

          assert(aplist->value.ident != NULL);
          if((aplist->value.ident->flags & IDF_CCALL) != 0)
            {
            assert(aplist->value.ident->name != NULL);
            PRS_Error(IDS_ERR_PLANTEXP_PASS_C_AS_AP,
              aplist->value.ident->name);
            err = TRUE;
            }
          else
            {
            err = PlantFPAddr(aplist->value.ident);
            PRSI_Plant(TOR);
            }
          break;

        default:
          PRS_Error(IDS_ERR_PLANTEXP_APLIST_CLASS,fplist->class);
          err = TRUE;
          break;
        }

      aplist = aplist->aplist;  /* next actual parameter */
      if(!blVarargs)
        fplist = fplist->next;  /* and next formal parameter */
      }

    /* For functions and procudures which take variable numbers of
    // arguments we plant a count on top of the stack.
    */
    if(blVarargs)
      {
      PRSI_PlantInteger(nArgCount);
      PRSI_Plant(TOR);
      }
    }
  return err;
}

/**********************************************************************
* PRSI_PlantExpression
* This plants an expression tree.
* expr is the expression to be planted.
* If blValue is true the value of the expression is planted, if
* false (where possible) its address.
**********************************************************************/
int PRSI_PlantExpression(ExprT* expr, int blValue)
{
  int err = FALSE;
  TypeT* LeftType = NULL;
  TypeT* RightType = NULL;
  long lbnd,ubnd;

  if(expr == NULL)
    return err;

  switch (expr->operator)
    {
    case TokInt:      /* Constant integer value */
      PRSI_PlantInteger(expr->value.integer);
      break;

    case TokFloat:    /* Constant real value */
      PRSI_PlantFloat(expr->value.real);
      break;

    case TokString:   /* Constant string value */
      PRSI_PlantString(expr->value.str);
      break;

    case TKFunction:  /* a function call or procedure call*/
    case TKProcedure:

      /* Plant the actual parameter list */
      err = PlantAPList(expr);

      /* followed by the function/proc address*/
      if(!err)
        err = PlantFPAddr(expr->value.ident);

      /* and call it */
      if(!err)
        {
        if(expr->value.ident->flags & IDF_CCALL)
          PRSI_Plant(CCALL);
        else
          PRSI_Plant(CALL);
        }
      break;

    case TokIdentifier:
      err = PlantIdentifier(expr->value.ident,blValue);
      break;

    case TSReturn:
      /* Do nothing: the returning bit is done by TPAssignment */
      break;

    case TPAssignment:
      assert(expr->left != NULL);
      assert(expr->right != NULL);
 
      LeftType = PRSI_ExpressionType(expr->left);
      RightType = PRSI_ExpressionType(expr->right);

      PRSI_PlantExpression(expr->right,TRUE); /* RHS of assignment */
      
      //if(RightType->BasicType == StringType)
      //  PRSI_Plant(SREF);  /* Need to reference strings */
      //
      
      if(PRSI_IsSimpleType(LeftType))
        {
        /* Ordinal types may need bound checking */
        if(PRSI_IsOrdinalType(LeftType))
          {
          PRSI_GetOrdinalBounds(LeftType,&lbnd,&ubnd);
          if(lbnd != -LONG_MAX || ubnd != LONG_MAX)
            {
            PRSI_PlantInteger(lbnd);
            PRSI_PlantInteger(ubnd);
            PRSI_Plant(BND);
            }
          }
        else if(LeftType->BasicType == StringType)
          {
          PRSI_Plant(SCLONE);
          }
        else if(LeftType->BasicType == RealType && RightType->BasicType == IntegerType)
          {
          PRSI_Plant(ITOF);  /* promote integer to real */
          }
          
        /* Now, if the lhs of the expression is a return
        // value (e.g. the function name) then we want to
        // replace the 2tos with the tos. Otherwise we
        // want to store the 2tos in the address in tos.
        */
        if(expr->left->operator == TSReturn)
          {
          if(LeftType->BasicType == StringType)
            {
            PRSI_Plant(SWAP);
            PRSI_Plant(SDEL);
            }
          else
            {
            PRSI_Plant(REPL);
            }
          }
        else
          {
          PRSI_PlantExpression(expr->left,FALSE); /* get addr of lhs */
          if(LeftType->BasicType == StringType)
            {
            PRSI_Plant(DUP);  /* get string address */
            PRSI_Plant(LD);   /* fetch the string */
            PRSI_Plant(SDEL); /* and delete old one */
            }
          PRSI_Plant(ST);
          }
        }
      else  /* compound type */
        {
        PRSI_PlantExpression(expr->left,FALSE); /* get addr of lhs */
        PRSI_PlantInteger(RightType->size);
        PRSI_Plant(MOV);
        }
      break;

    case TSNegate:
      assert(expr->right != NULL);
      PRSI_PlantInteger(0);  /* 0 is same for int and real */
      PRSI_PlantExpression(expr->right,TRUE);
      RightType = PRSI_ExpressionType(expr->right);
      if(RightType->BasicType == IntegerType)
        PRSI_Plant(SUB);
      else
        PRSI_Plant(FSUB);
      break;

    case TKNot:
      assert(expr->right != NULL);
      PRSI_PlantExpression(expr->right,TRUE);
      RightType = PRSI_ExpressionType(expr->right);
      if(RightType->BasicType == IntegerType)
        PRSI_Plant(NOT);   /* bitwise not */
      else
        PRSI_Plant(ZEQU); /* logical not */
      break;


    case TKDiv:     /* Integer division */
    case TKMod:     /* Integer modulus */
    case TKAnd:     /* Logical or bitwise AND */
    case TKOr:      /* Logical or bitwise OR */
      assert(expr->left != NULL);
      assert(expr->right != NULL);

      PRSI_PlantExpression(expr->left,TRUE);
      PRSI_PlantExpression(expr->right,TRUE);
      switch (expr->operator)
        {
        case TKDiv: PRSI_Plant(DIV);   break;
        case TKMod: PRSI_Plant(MOD);   break;
        case TKAnd: PRSI_Plant(AND);   break;
        case TKOr:  PRSI_Plant(OR);    break;
        }
      break;

    case TPDivide:  /* Floating point division */
      assert(expr->left != NULL);
      assert(expr->right != NULL);
      
      LeftType = PRSI_ExpressionType(expr->left);
      RightType = PRSI_ExpressionType(expr->right);

      PRSI_PlantExpression(expr->left,TRUE);
      if(LeftType->BasicType == IntegerType)
        PRSI_Plant(ITOF);  /* promote integer->float*/

      PRSI_PlantExpression(expr->right,TRUE);
      if(RightType->BasicType == IntegerType)
        PRSI_Plant(ITOF);  /* promote integer->float*/

      PRSI_Plant(FDIV);
      break;

    /* These all for ordinal real or string types */
    case TPPlus:
    case TPMinus:
    case TPTimes:
    case TPEquals:
    case TPNotEquals:
    case TPLessThan:
    case TPGreaterThan:
    case TPLessEquals:
    case TPGreaterEquals:
      
      assert(expr->left != NULL);
      assert(expr->right != NULL);
      
      LeftType = PRSI_ExpressionType(expr->left);
      RightType = PRSI_ExpressionType(expr->right);

      PRSI_PlantExpression(expr->left,TRUE);
      if(LeftType->BasicType == IntegerType &&
         RightType->BasicType == RealType)
        PRSI_Plant(ITOF);  /* promote integer->float*/

      PRSI_PlantExpression(expr->right,TRUE);
      if(RightType->BasicType == IntegerType &&
         LeftType->BasicType == RealType)
        PRSI_Plant(ITOF);  /* promote integer->float*/

      if(LeftType->BasicType == RealType ||
         RightType->BasicType == RealType)
        {
        switch (expr->operator)
          {
          case TPPlus:          PRSI_Plant(FADD);  break;
          case TPMinus:         PRSI_Plant(FSUB);  break;
          case TPTimes:         PRSI_Plant(FMULT); break;
          case TPEquals:        PRSI_Plant(EQU);   break;
          case TPNotEquals:     PRSI_Plant(NEQU);  break;
          case TPLessThan:      PRSI_Plant(FLT);   break;
          case TPGreaterThan:   PRSI_Plant(FGT);   break;
          case TPLessEquals:    PRSI_Plant(FLE);   break;
          case TPGreaterEquals: PRSI_Plant(FGE);   break;
          default: assert(FALSE); break;
          }
        }
      else if(LeftType->BasicType == StringType &&
         RightType->BasicType == StringType)
        {
        /* Call the internal C function which compares 2 strings.  This
        // should put -1 on the stack if the first string is < second,
        // 0 if they are the same and 1 if the first is > second.
        // Given this we can compare the result against 0 using the
        // normal integer comparison operators.
        */
        PRSI_Plant(SWAP);
        PRSI_Plant(TOR);
        PRSI_Plant(TOR);
        PRSI_PlantAddr(PRSI_StrCmp);
        PRSI_Plant(CCALL);
        PRSI_PlantInteger(0);

        switch (expr->operator)
          {
          case TPEquals:        PRSI_Plant(EQU);   break;
          case TPNotEquals:     PRSI_Plant(NEQU);  break;
          case TPLessThan:      PRSI_Plant(LT);    break;
          case TPGreaterThan:   PRSI_Plant(GT);    break;
          case TPLessEquals:    PRSI_Plant(LE);    break;
          case TPGreaterEquals: PRSI_Plant(GE);    break;
          default: assert(FALSE); break;
          }
        }
      else /* neither sides real nor string*/
        {
        switch (expr->operator)
          {
          case TPPlus:          PRSI_Plant(ADD);   break;
          case TPMinus:         PRSI_Plant(SUB);   break;
          case TPTimes:         PRSI_Plant(MULT);  break;
          case TPEquals:        PRSI_Plant(EQU);   break;
          case TPNotEquals:     PRSI_Plant(NEQU);  break;
          case TPLessThan:      PRSI_Plant(LT);    break;
          case TPGreaterThan:   PRSI_Plant(GT);    break;
          case TPLessEquals:    PRSI_Plant(LE);    break;
          case TPGreaterEquals: PRSI_Plant(GE);    break;
          default: assert(FALSE); break;
          }
        }
      break;

    case TPLeftBracket:
    case TPPeriod:
      assert(expr->left != NULL);
      assert(expr->right != NULL);

      err = PRSI_PlantCompoundVariable(expr);
      /* If blValue is true then we want a value: This should only
      // be true for simple types so it should be safe to use LD
      */
      if(blValue)
        {
        assert(PRSI_IsSimpleType(PRSI_ExpressionType(expr)));
        PRSI_Plant(LD);
        }
      break;

    default:
      PRS_Error(IDS_ERR_PLANTEXP_UNKNOWN_OP,expr->operator);
      err = TRUE;
    }

  return err;
}
