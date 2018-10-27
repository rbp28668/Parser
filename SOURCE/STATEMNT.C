/*
// File:        STATEMNT.C
//
// Prefix:
//
// Description: Code for parsing a statement.
//
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "prsif.h"
#include "lex.h"
#include "tokens.h"
#include "error.h"
#include "parser.h"
#include "expr.h"
#include "ident.h"
#include "musthave.h"
#include "statemnt.h"
#include "iproc.h"
#include "plant.h"
#include "malloc.h"

#define MAX_CASE 64  /* Max no of statements per case statement */

static int Statement(void);
static int StatementSequence(void);

/**********************************************************************
* IfStatement
* Parses an if statement after the intial if has been read in.
*
* if-statement = "if" expression "then" statement [ "else" statement ].
*
* Returns FALSE for success, TRUE for failure.
**********************************************************************/
static int IfStatement(void)
{
  int err = FALSE;
  int tok;
  TypeT* etype=NULL;
  Opcode* ZBraPos = NULL;
  Opcode* BraPos = NULL;

  ExprT* expr = PRSI_ParseExpression();
  if(expr == NULL)
    err = TRUE;
  else
    {
    etype = PRSI_ExpressionType(expr);
    if(etype != &BooleanTypeDecl)
      {
      if(etype != NULL) /* if null, already displayed an error */
        PRS_Error(IDS_ERR_STATEMNT_NEED_BOOL_IN_IF);
      err = TRUE;
      }
    }


  if(!err)
    err = PRSI_PlantExpression(expr,TRUE); /* plant control statement */
  PRSI_FreeExpression(expr);

  if (!err)
    err = PRSI_MustHave(TKThen);

  if(!err)
    ZBraPos = PRSI_PlantBranch(ZBRA);


  if(!err)
    err = Statement();

  if(!err)
    {
    tok = PRSI_LexGetToken();

    /* Check for semicolon followed by else. If so then ; was
    // spurious, if semicolon was not followed by else then
    // it is probably wanted so push it back
    */

    if(tok == TPSemicolon)
      {
      tok = PRSI_LexGetToken();
      if(tok != TKElse)
        {
        PRSI_LexPushBackToken(tok);
        tok = TPSemicolon;
        }
      }


    if(tok == TKElse)
      {
      BraPos = PRSI_PlantBranch(BRA); /* at end of statement
                                         1 to branch round else*/
      PRSI_PlantSetBranch(PRSI_PlantPosition(),ZBraPos);

      err = Statement();              /* Else statement */

      PRSI_PlantSetBranch(PRSI_PlantPosition(),BraPos);
      }
    else
      {
      PRSI_PlantSetBranch(PRSI_PlantPosition(),ZBraPos);
      PRSI_LexPushBackToken(tok);
      }
    }
  return err;
}

/**********************************************************************
* CaseLabelList
* Parses a case label list.
* sel_type is the type of the case selector.  The case labels must
* have the same type as sel_type (which must be an ordinal type).
*
* case-label-list = constant { "," constant }.
*
* Returns FALSE for success, TRUE for failure.
**********************************************************************/
static int CaseLabelList(TypeT* sel_type)
{
  int err = FALSE;
  int tok;
  IdentifierT ConstID;
  int first = TRUE;

  assert(sel_type != NULL);
  do
    {
    /* read constant type & value into the (unnamed)
    // identifier.
    */

    err = PRSI_Constant(&ConstID);

    if(!err)
      {
      if(ConstID.type != sel_type)
        {
        PRS_Error(IDS_ERR_STATEMNT_CASE_LABEL_SEL_TYPE);
        err = TRUE;
        }
      else
        tok = PRSI_LexGetToken();
      }

    if(!err)
      {
      if(first)
        {
        PRSI_Plant(DUP);
        PRSI_PlantInteger(ConstID.value.ordinal);
        PRSI_Plant(EQU);
        first = FALSE;
        }
      else
        {
        PRSI_Plant(OVER);
        PRSI_PlantInteger(ConstID.value.ordinal);
        PRSI_Plant(EQU);
        PRSI_Plant(OR);
        }
      }
    }
  while (!err && tok == TPComma);

  if(!err)
    PRSI_LexPushBackToken(tok);

  return err;
}

/**********************************************************************
* CaseLimb
* Parses a case limb of a case statement.
*
* caselimb = case-label-list ":" statement.
*
* Returns FALSE for success, TRUE for failure.
**********************************************************************/
static int CaseLimb(TypeT* sel_type,Opcode** BraList, int* CaseCount)
{
  int err = FALSE;
  Opcode* ZBraPos = NULL;
  Opcode* BraPos = NULL;

  assert(sel_type != NULL);
  assert(BraList != NULL);
  assert(CaseCount != NULL);
  
  err = CaseLabelList(sel_type);
 
  if(!err)
    err = PRSI_MustHave(TPColon);

  if(!err)
    {
    ZBraPos = PRSI_PlantBranch(ZBRA);
    PRSI_Plant(DROP);
    err = Statement();
    BraPos = PRSI_PlantBranch(BRA);
    PRSI_PlantSetBranch(PRSI_PlantPosition(),ZBraPos);

    /* Need to pass BraPos up so the branch can be patched
    // at the end of the case structure
    */

    if(*CaseCount >= MAX_CASE)
      {
      PRS_Error(IDS_ERR_STATEMNT_TOO_MANY_CASE,MAX_CASE);
      err = TRUE;
      }
    else
      {
      BraList[(*CaseCount)++] = BraPos;
      }
    }

  return err;
}

/**********************************************************************
* CaseStatement
* Parses a case statement after the initial "case" has been read in.
*
* case-statement = "case" expression "of" caselimb { ";" caselimb }
*                   [";"] "end".
*
* Returns FALSE for success, TRUE for failure.
**********************************************************************/
static int CaseStatement(void)
{
  int err = FALSE;
  int tok;
  TypeT* selector_type = NULL;
  Opcode* BraList[MAX_CASE];
  int CaseCount = 0;
  int i;
  Opcode* BraTarget = NULL;

  ExprT* expr = PRSI_ParseExpression();
  if(expr == NULL)
    err = TRUE;
  else
    {
    selector_type = PRSI_ExpressionType(expr);
    if(!PRSI_IsOrdinalType(selector_type))
      {
      if(selector_type != NULL)
        PRS_Error(IDS_ERR_STATEMNT_CASE_NOT_ORDINAL);
      err = TRUE;
      }
    }

  if(!err)
    PRSI_PlantExpression(expr,TRUE);  /* Plant control expression */
  PRSI_FreeExpression(expr);

  if (!err)
    err = PRSI_MustHave(TKOf);

  do
    {
    err = CaseLimb(selector_type,BraList,&CaseCount);

    /* After a caselimb we either have "; caselimb" or "; end" or "end"
    // So the loop should terminate with ; end or just end.
    */
    if(!err)
      {
      tok = PRSI_LexGetToken();
      if(tok == TPSemicolon)
        {
        tok = PRSI_LexGetToken();
        if(tok != TKEnd)
          PRSI_LexPushBackToken(tok); /* not end after ; so must be caselimb */
        }
      else if (tok != TKEnd)     /* if not a semicolon, must be end */
        {
        PRS_Error(IDS_ERR_STATEMNT_MISSING_END_OR_SEMICOLON,PRSI_LexTranslateToken(tok));
        err = TRUE;
        }
      }
    }
  while (!err && tok != TKEnd);

  if(!err)
    {
    PRSI_Plant(DROP);
    PRSI_PlantInteger(MISMATCH_CASE_FAULT);
    PRSI_Plant(FAULT); /* as no case labels matched */

    /* Set branches for all the cases */
    BraTarget = PRSI_PlantPosition();
    assert(BraTarget != NULL);
    for(i=0; i<CaseCount; ++i)
      {
      assert(BraList[i] != NULL);
      PRSI_PlantSetBranch(BraTarget,BraList[i]);
      }
    }

  return err;
}

/**********************************************************************
* ConditionalStatement
*
* conditional-statement = if-statement | case-statement.
*
* Returns FALSE for success, TRUE for failure.
**********************************************************************/
int ConditionalStatement(void)
{
  int err = FALSE;
  int tok = PRSI_LexGetToken();

  switch (tok)
    {
    case TKIf:
      err = IfStatement();
      break;
    
    case TKCase:
      err = CaseStatement();
      break;

    default:
      PRS_Error(IDS_ERR_STATEMNT_EXPECTED_IF_OR_CASE);
      err = TRUE;
      break;
    }

  return err;
}

/**********************************************************************
* WhileStatement
* Parses a while statement. Note that the initial "while" has
* already been read in.
*
* while-statement = "while" expression "do" statement.
*
* Returns FALSE for success, TRUE for failure.
**********************************************************************/
static int WhileStatement(void)
{
  int err = FALSE;
  TypeT* etype=NULL;
  Opcode* BraTarget = NULL;
  Opcode* ZBraPos = NULL;

  ExprT* expr = PRSI_ParseExpression();
  if(expr == NULL)
    err = TRUE;
  else
    {
    etype = PRSI_ExpressionType(expr);
    if(etype != &BooleanTypeDecl)
      {
      if(etype != NULL)
        PRS_Error(IDS_ERR_STATEMNT_WHILE_EXPR_NOT_BOOL);
      err = TRUE;
      }
    }

  if(!err)
    {
    BraTarget = PRSI_PlantPosition();
    err = PRSI_PlantExpression(expr,TRUE); /* plant control expression */
    }
  PRSI_FreeExpression(expr);

  if (!err)
    err = PRSI_MustHave(TKDo);

  if(!err)
    {
    ZBraPos = PRSI_PlantBranch(ZBRA);
    assert(ZBraPos != NULL);
    err = Statement();
    PRSI_PlantSetBranch(BraTarget,PRSI_PlantBranch(BRA));
    PRSI_PlantSetBranch(PRSI_PlantPosition(),ZBraPos);
    }
  return err;
}

/**********************************************************************
* RepeatStatement
* Parses a repeat statement after the initial "repeat" has
* been read in.
*
* repeat-statement = "repeat" statement-sequence "until" expression.
* 
* Returns FALSE for success, TRUE for failure.
**********************************************************************/
static int RepeatStatement(void)
{
  int err = FALSE;
  TypeT* etype = NULL;
  ExprT* expr = NULL;
  Opcode* ZBraTarget = NULL;

  ZBraTarget = PRSI_PlantPosition();
  assert(ZBraTarget != NULL);
  
  err = StatementSequence();

  if (!err)
    err = PRSI_MustHave(TKUntil);

  if(!err)
    {
    expr = PRSI_ParseExpression();
    if(expr == NULL)
      err = TRUE;
    else
      {
      etype = PRSI_ExpressionType(expr);
      if(etype != &BooleanTypeDecl)
        {
        if(etype != NULL)
          PRS_Error(IDS_ERR_STATEMNT_REPEAT_EXPR_NOT_BOOL);
        err = TRUE;
        }
      }
    }

  if(!err)
    err = PRSI_PlantExpression(expr,TRUE); /* plant control expression */

  PRSI_FreeExpression(expr);

  if(!err)
    {
    PRSI_PlantSetBranch(ZBraTarget,PRSI_PlantBranch(ZBRA));
    }

  return err;
}

/**********************************************************************
* ForStatement
* Parses a for statement after the initial "for" has been
* read in.
*
* for-statement = "for" variable-identifier ":=" 
* initial-expression ( "to" | "downto" )
* final-expression "do" statement.
*
* Returns FALSE for success, TRUE for failure.
**********************************************************************/
static int ForStatement(void)
{
  int err = FALSE;
  IdentifierT* ControlIdent = NULL;
  ExprT* ControlExpression = NULL;
  ExprT* InitialExpression = NULL;
  ExprT* FinalExpression = NULL;
  int tok;
  int up = TRUE;
  Opcode* BraTarget = NULL;
  Opcode* ZBraPos = NULL;
  long lbnd = 0;
  long ubnd = 0;

  err = PRSI_MustHave(TokIdentifier);

  /* Get the control variable and check it.  It must:
  // 1. exist
  // 2. be a variable (global, value or var parameter)
  // 3. have an ordinal type
  */
  if(!err)
    {
    ControlIdent = PRSI_FindIdentifier(PRSI_LexGetIdentifier());
    if(ControlIdent == NULL)
      {
      PRS_Error(IDS_ERR_STATEMNT_UNKNOWN_FOR_VAR);
      err = TRUE;
      }
    else
      {
      switch (ControlIdent->class)
        {
        case VariableIdentifier:    
          ControlExpression = PRSI_Variable(ControlIdent);
          break;
        default:
          {
          PRS_Error(IDS_ERR_STATEMNT_INVALID_FOR_LOOP_ID);
          err = TRUE;
          }
        }
      }
    if(!err)
      {
      assert(ControlIdent->type != NULL);
      if (!PRSI_IsOrdinalType(ControlIdent->type))
        {
        PRS_Error(IDS_ERR_STATEMNT_FOR_VAR_NOT_ORDINAL);
        err = TRUE;
        }
      }
    }


  if(!err)
    err = PRSI_MustHave(TPAssignment);

  if(!err)
    {
    InitialExpression = PRSI_ParseExpression();
    if(PRSI_IsAssignmentCompatible(ControlIdent->type,
      PRSI_ExpressionType(InitialExpression)) == NULL )
      {
      PRS_Error(IDS_ERR_STATEMNT_FOR_INITIAL_EXPR_TYPE);
      err = TRUE;
      }
    }

  if(!err)
    {
    tok = PRSI_LexGetToken();
    switch (tok)
      {
      case TKTo:
        up = TRUE;
        break;

      case TKDownto:
        up = FALSE;
        break;

      default:
        PRS_Error(IDS_ERR_STATEMNT_MISSING_TO_OR_DOWNTO);
        err = TRUE;
      }
    }

  if(!err)
    {
    FinalExpression = PRSI_ParseExpression();
    if(PRSI_IsAssignmentCompatible(ControlIdent->type,
      PRSI_ExpressionType(FinalExpression)) == NULL)
      {
      PRS_Error(IDS_ERR_STATEMNT_FOR_FINAL_EXPR_TYPE);
      err = TRUE;
      }
    }

  if(!err)
    err = PRSI_MustHave(TKDo);

  /* Want the bounds of the ordinal type so we can range
  // check the control variable on the initial assignement
  // and its increment/decrement.
  */
  if(!err)
    PRSI_GetOrdinalBounds(ControlIdent->type,&lbnd,&ubnd);


  /* Evaluate final expression and store it on the return stack */
  if(!err)
    err = PRSI_PlantExpression(FinalExpression,TRUE);
  if(!err)
    PRSI_Plant(TOR);


  /* Evaluate initial expression. This initialises the temporary
  *  loop counter held on the top of stack.
  */
  if(!err)
    {
    err = PRSI_PlantExpression(InitialExpression,TRUE);
    PRSI_Plant(NOP); /* make sure any constant initial expression is
                     // written before the loop: if it is tied to
                     // FROMR then it would be stacked each iteration.
                     */
    }

  if(!err)
    {
    BraTarget = PRSI_PlantPosition();

    /* Get a copy of the final value */
    PRSI_Plant(FROMR);
    PRSI_Plant(DUP);
    PRSI_Plant(TOR);

    /* Compare temp with the final value & branch if done */
    PRSI_Plant(OVER);
    PRSI_Plant( (Opcode)((up) ? GE : LE));
    ZBraPos = PRSI_PlantBranch(ZBRA);

    /* Bounds check new value as needed */
    if(lbnd != -LONG_MAX || ubnd != LONG_MAX)
      {
      PRSI_PlantInteger(lbnd);
      PRSI_PlantInteger(ubnd);
      PRSI_Plant(BND);
      }
    }

  if(!err)
    {
    /* Store temp value in loop variable */
    PRSI_Plant(DUP);
    err = PRSI_PlantExpression(ControlExpression,FALSE); /* addr of control variable */
    PRSI_Plant(ST);
    }

  if(!err)
    {
    PRSI_Plant(TOR);  /* clear eval stack for statement */
    err = Statement();
    PRSI_Plant(FROMR); /* and restore temp loop value */
    }

  /* Increment or decrement temp loop variable on TOS */
  if(!err)
    {
    PRSI_PlantInteger(1);
    PRSI_Plant( (Opcode)((up) ? ADD : SUB));
    }

  if(!err)
    {
    /* Loop back to try again */
    PRSI_PlantSetBranch(BraTarget,PRSI_PlantBranch(BRA));

    /* Tidy up at loop exit */
    PRSI_PlantSetBranch(PRSI_PlantPosition(),ZBraPos);
    PRSI_Plant(FROMR);
    PRSI_Plant(DROP);
    PRSI_Plant(DROP);
    }

  PRSI_FreeExpression(ControlExpression);
  PRSI_FreeExpression(InitialExpression);
  PRSI_FreeExpression(FinalExpression);

  return err;
}

/**********************************************************************
* PRSI_RepetativeStatement
* 
* repetative-statement = while-statement | repeat-statement | for-statement.
*
* Returns FALSE for success, TRUE for failure.
**********************************************************************/
int PRSI_RepetativeStatement(void)
{
  int err = FALSE;
  int tok = PRSI_LexGetToken();

  switch (tok)
    {
    case TKWhile:
      err = WhileStatement();
      break;

    case TKRepeat:
      err = RepeatStatement();
      break;

    case TKFor:
      err = ForStatement();
      break;

    default:
      PRS_Error(IDS_ERR_STATEMNT_EXPECTED_WHILE_REPEAT_FOR);
      err = TRUE;
      break;
    }
  return err;
}

/**********************************************************************
* StructuredStatement
*
* structured-statement = compound-statement | repetative-statement |
*                        conditional-statement.
* Returns FALSE for success, TRUE for failure.
**********************************************************************/
PRSI_StructuredStatement(void)
{
  int err=FALSE;
  int tok = PRSI_LexGetToken();

  switch (tok)
    {
    case TKBegin:
      PRSI_LexPushBackToken(tok);
      err = PRSI_CompoundStatement();
      break;

    case TKIf:
    case TKCase:
      PRSI_LexPushBackToken(tok);
      err = ConditionalStatement();
      break;

    case TKWhile:
    case TKRepeat:
    case TKFor:
      PRSI_LexPushBackToken(tok);
      err = PRSI_RepetativeStatement();
      break;

    default:
      PRS_Error(IDS_ERR_STATEMNT_SYNTAX_UNEXPECTED,
        PRSI_LexTranslateToken(tok));
      err = TRUE;
      break;
    }
  return err;
}




/**********************************************************************
* Statement
*
* statement = simple-statement | structured statement.
*
* Returns TRUE on error, FALSE if OK
**********************************************************************/
static int Statement(void)
{
  int err = FALSE;

  int tok = PRSI_LexGetToken();
  switch (tok)
    {
    case TokIdentifier:   
      err = PRSI_SimpleStatement();    /* either assignment or procedure call */
      break;

    default:
      PRSI_LexPushBackToken(tok);
      err = PRSI_StructuredStatement();
      break;
    }

  return err;

}

/**********************************************************************
* StatementSequence
*
* statement-sequence = statement { ";" statement }.
*
* Returns TRUE on error, FALSE if OK
**********************************************************************/
static int StatementSequence(void)
{
  int err = FALSE;
  int tok;

  do
    {

    /* Look for TKEnd or TKUntil which would indicate an
    // extra semicolon in a block or repeat-until loop. If we
    // find one of these then we should break the loop, 
    // otherwise we should push it back and continue.
    */
    tok = PRSI_LexGetToken();
    if(tok == TKEnd || tok == TKUntil)
      {
      /* Could print an error message but don't bother */
      /* Note that this token will be pushed back after loop */
      break;
      }
    else
      PRSI_LexPushBackToken(tok);

    err = Statement();
    tok = PRSI_LexGetToken();
    }
  while (!err && tok == TPSemicolon);

  if(!err)
    PRSI_LexPushBackToken(tok);    /* token wasn't a semicolon */

  return err;
}

/**********************************************************************
* PRSI_CompoundStatement
*
* compound-statement = "begin" statement-sequence "end".
*
* Returns FALSE for success, TRUE for failure.
**********************************************************************/
int PRSI_CompoundStatement(void)
{
  int err = FALSE;
  
  err = PRSI_MustHave(TKBegin);

  if(!err)
    err = StatementSequence();

  if(!err)
    err = PRSI_MustHave(TKEnd);

  return err;
}
