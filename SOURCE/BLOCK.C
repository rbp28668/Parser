/* 
// File:        BLOCK.C
// 
// Prefix:
// 
// Description: Reads in a procedure or function body.
//
*/
#include <stdlib.h>
#include <assert.h>
#include "prsif.h"
#include "lex.h"
#include "tokens.h"
#include "parser.h"
#include "statemnt.h"
#include "musthave.h"
#include "block.h"
#include "iproc.h"
#include "plant.h"
#include "amble.h"
#include "error.h"
#include "malloc.h"



/**********************************************************************
* DeclarationPart
* Parses the declaration part of a procedure or function definition. 
*
* declaration-part = [constant-definition-part]
*                   [type-definition-part]
*                   [variable-declaration-part]
* 
* Returns TRUE on error, FALSE if OK
**********************************************************************/
static int DeclarationPart(void)
{
  int err = FALSE;
  int tok;

  tok = PRSI_LexGetToken();
  PRSI_LexPushBackToken(tok);

  if(tok == TKConst)
    err = PRSI_ConstantDefinitionPart();

  if(!err)
    {
    tok = PRSI_LexGetToken();
    PRSI_LexPushBackToken(tok);

    if(tok == TKType)
      err = PRSI_TypeDefinitionPart();
    }

  if(!err)
    {
    tok = PRSI_LexGetToken();
    PRSI_LexPushBackToken(tok);

    if(tok == TKVar)
      err = PRSI_VariableDeclarationPart();
    }
  
  return err;
}



/**********************************************************************
* PRSI_Block
* Parses the body for a procedure or a function
* id is the procedure or function identifier.
*
* block = declaration-part statement-part.
*
* statement-part = compound-statement.
*
* Returns TRUE on error FALSE if ok.
**********************************************************************/
int PRSI_Block(IdentifierT* id)
{
  int err = 0;
  
  assert(id != NULL);
  
  err = DeclarationPart();

  if(!err)
    PRSI_PreAmble(id);              /* Stuff proc/fn entry code */

  if(!err)
    err = PRSI_CompoundStatement();

  if(!err)                        /* Stuff proc/fn exit code */
    PRSI_PostAmble(id);

  if(!err)
    {
    id->value.addr = PRSI_PlantGetSegment();
    if(id->value.addr == NULL)
      {
      PRS_Error(IDS_ERR_BLOCK_NO_CODE,id->name);
      err = TRUE;
      }
    }

  return err;
}

