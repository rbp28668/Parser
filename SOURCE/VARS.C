/* 
// File:          VARS.C
// 
// Prefix:        NONE
// 
// Description:   Code to handle variable declarations.
//
*/


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "prsif.h"
#include "lex.h"
#include "tokens.h"
#include "parser.h"
#include "musthave.h"
#include "ident.h"
#include "error.h"
#include "malloc.h"


#define LOCAL_OFFSET 0 /* FP offset of first local variable */


/* Variable to keep track of the allocation of stack in a
*  function or procedure.
*/
static int StackOffset = 0;



/**********************************************************************
* IdentifierList
* This parses an identifier list in a var declaration.
*
* identifier-list = identifier { "," identifier }.
*
**********************************************************************/
static int IdentifierList(IdentifierT **list)
{
  int err = FALSE;
  int tok;
  char *name = NULL;
  IdentifierT *this = NULL;

  assert(list != NULL);
  
  *list = NULL;

  do
    {
    err = PRSI_MustHave(TokIdentifier);
    
    if (!err)
      {
      name = PRSI_LexGetIdentifier();
      err = (name == NULL);
      }

    if (!err)
      {
      err = PRSI_CheckDuplicateIdentifier(name);
      if(err)
        PRS_Error(IDS_ERR_VARS_DUP_IDENT,name);
      }

    if (!err)
      {
      this = PRSI_NewIdentifier(name,LINK);
      if (this == NULL)
        {
        PRS_Error(IDS_ERR_VARS_MEM);
        err = TRUE;
        }
      }

    if (!err) /* Tack ident on to front of local list */
      {
      this->link = *list;
      *list = this;
      }

    if (!err)
      tok = PRSI_LexGetToken();
    }
  while (!err && (tok == TPComma));

  if (!err)
    PRSI_LexPushBackToken(tok);

  return err;
}


/**********************************************************************
* VariableDeclaration
* This handles a variable declaration
* This should be callled after the keyword "var" has already been
* read in.
*
* variable-declaration = "var" identifier-list ":" type.
*
* Returns TRUE on failure, FALSE if OK.
**********************************************************************/
static int VariableDeclaration(void)
{
  int err = FALSE;
  IdentifierT *list = NULL;
  IdentifierT *here = NULL;
  IdentifierT *next = NULL;
  TypeT *type = NULL;

  err = IdentifierList(&list);

  if (!err)
    err = PRSI_MustHave(TPColon);

  if (!err)
    {
    type = PRSI_Type();
    err = (type == NULL);
    }
  
  if (!err)
    {
    for (here = list; here != NULL; here = next)
      {
      here->class = VariableIdentifier;
      here->scope = (PRSI_InLocalScope()?LocalScope:GlobalScope);
      here->type = type;
      
      next = here->link;
      here->link = NULL;


      if (PRSI_InLocalScope())
        {
        here->value.FPOffset = StackOffset;
        StackOffset += type->size;
        }
      else  /* Global scope */
        {
        /* The built in types can all store their contents in the
        *  actual identifier.  Arrays, records and strings need
        *  other handling
        */
        switch (type->BasicType)
          {
          case IntegerType:
          case EnumeratedType:
            here->value.ordinal = 0l;
            break;

          case RealType:
            here->value.real = 0.0f;
            break;

          case StringType:
            here->value.string = PRS_CreateString("");
            break;

          case RecordType:
          case ArrayType:
            here->value.addr = PRSI_AllocateGlobalStore(type->size);
            if (here->value.addr != NULL)
              memset(here->value.addr,0,type->size);
            break;
             
          default:
            assert(FALSE);
            break;
          }
        }
      }
    }

  return err;
}


/**********************************************************************
* PRSI_VariableDeclarationPart
*
* variable-declaration-part = variable-declaration ";" 
*                            { variable-declaration ";" }
*
**********************************************************************/
int PRSI_VariableDeclarationPart(void)
{
  int tok;
  int err = FALSE;

  /* Reset stack offsets for local variables */
  if(PRSI_InLocalScope())
    StackOffset = LOCAL_OFFSET;

  err = PRSI_MustHave(TKVar);

  if(!err)
    {
    err = VariableDeclaration();
    if(!err) err = PRSI_MustHave(TPSemicolon);
    if(!err) tok = PRSI_LexGetToken();

    while (!err && tok == TKVar)
      {
      err = VariableDeclaration();
      if(!err) err = PRSI_MustHave(TPSemicolon);
      if(!err) tok = PRSI_LexGetToken();
      }
    
    if(!err)
      PRSI_LexPushBackToken(tok);
    }

  return err;
}

