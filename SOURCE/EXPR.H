/**********************************************************************
* EXPR.H
* Header file for expression handling
**********************************************************************/

#ifndef _EXPR_H
#define _EXPR_H 1

typedef struct s_expr ExprT;

struct s_expr
{
  int operator;     /* which is a token */
  ExprT *left;
  ExprT *right;
  ExprT *aplist;    /* actual parameter list for a function call */

  union             /* value for leaf nodes */
    {
    IdentifierT *ident;
    StringT *str;
    long integer;
    long boolean;
    float real;
    }value;
};

/* EXPR.C */
extern ExprT *PRSI_NewExpr(void);
extern void PRSI_FreeExpression(ExprT *expr);
extern ExprT *PRSI_Variable(IdentifierT *ident);
extern ExprT *PRSI_ParseExpression(void);

/* SHOWEXPR.C */
extern void PRSI_ShowAPList(ExprT *expr);
extern void PRSI_ShowExpression(ExprT *expr);

/* EXPRTYPE.C */

extern TypeT *PRSI_ExpressionType(ExprT *expr);
extern int PRSI_ExprIsLValue(ExprT *expr);
extern TypeT* PRSI_IsAssignmentCompatible(TypeT* LeftType,TypeT* RightType);


#endif
