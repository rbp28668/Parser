/* 
// File:        PARSER.C
// 
// Prefix:      NONE
// 
// Description: Parser main entry point
//
*/

#include <stdio.h>
#include <assert.h>
#include "prsif.h"
#include "lex.h"
#include "tokens.h"
#include "parser.h"
#include "ident.h"
#include "musthave.h"
#include "error.h"
#include "record.h"
#include "enums.h"
#include "malloc.h"
#include "procdec.h"
#include "fndec.h"
#include "iproc.h"
#include "plant.h"

/**********************************************************************
* PRS_ParseUnit
* parses a syntactic unit.
* Returns 0 if parsed OK, Non-zero if error.
* If end of file is found *IsEOF is set to TRUE;
**********************************************************************/
int PRS_ParseUnit(int* IsEOF)
{
  int tok;
  int err = FALSE;
  MarkT mark;
  int EatSemicolon = FALSE;

  assert(IsEOF != NULL);

  PRSI_StartTransaction(&mark);

  /* Always start a new segment: most of these don't need it but
  // nothing is allocated.
  */
  PRSI_PlantNewSegment();

  tok = PRSI_LexGetToken();
  switch (tok)
    {
    case TKConst:
      PRSI_LexPushBackToken(tok);
      err = PRSI_ConstantDefinitionPart();
      break;

    case TKType:
      PRSI_LexPushBackToken(tok);
      err = PRSI_TypeDefinitionPart();
      break;

    case TKVar:
      PRSI_LexPushBackToken(tok);
      err = PRSI_VariableDeclarationPart();
      break;

    case TKProcedure:
      err = PRSI_ProcedureDeclaration();
      break;

    case TKFunction:
      err = PRSI_FunctionDeclaration();
      break;

    case TokIdentifier:
      /* should be a procedure call or an assignment
      // simple-statement = assignment-statement | procedure-statement.
      */
      err = PRSI_SimpleStatement();
      EatSemicolon = TRUE;
      if(!err)
        PRSI_PlantSetRunnable();
      break;

    case TokEof:
      *IsEOF = TRUE;
      break;

    default:
      PRS_Error(IDS_ERR_PARSER_SYNTAX,PRSI_LexTranslateToken(tok));
      err = TRUE;
      break;

    }


  /* now look for optional semicolon */
  if(!err && EatSemicolon)
    {
    tok = PRSI_LexGetToken();
    if(tok != TPSemicolon)
      PRSI_LexPushBackToken(tok);
    }

  if(err)
    {
    PRSI_LexFlushInput();
    PRSI_RollBackTransaction(&mark);
    }

  return err;
}

/**********************************************************************
* PRS_InitialiseInterpreter
* This initialises the parser and should be called before any
* other call to the parser.
* hInst is the application instance for windows versions, should be
* 0 for non-windows apps.
**********************************************************************/
int PRS_InitialiseInterpreter(HINSTANCE hInst)
{
  PRSI_SetupLexAnalyser();
  PRSI_InitialiseIdentifiers();
  PRSI_InitialiseICode();
  PRSI_PlantInitialise();
  PRSI_InitError(hInst);
  
  return 0;
}

/**********************************************************************
* PRS_TerminateInterpreter
* This terminates the parser and should be called after any other
* call to the parser.
**********************************************************************/
void PRS_TerminateInterpreter(void)
{
  PRSI_PlantTerminate();
  PRSI_TerminateICode();
  PRSI_TerminateIdentifiers();
  return;
}
