/* 
// File:        FNDEC.C
// 
// Prefix:
// 
// Description: Code for parsing a function declaration.
//
*/

#include <stdlib.h>
#include <assert.h>

#include "prsif.h"
#include "parser.h"
#include "tokens.h"
#include "lex.h"
#include "musthave.h"
#include "ident.h"
#include "error.h"
#include "block.h"
#include "fplist.h"
#include "fndec.h"
#include "plant.h"
#include "malloc.h"

/**********************************************************************
* PRSI_FunctionHeading.
* This parses a function heading.  The "function" token has already
* been read in.
* If link is true then the identifier is linked into the current
* scope, if false it is not
*
* function-heading = "function" identifier [ formal-parameter-list ] 
*   ":" result-type.
*
**********************************************************************/
IdentifierT* PRSI_FunctionHeading(int link)
{
  int err = FALSE;
  IdentifierT *this = NULL;
  char* name = NULL;
  int tok;

  err = PRSI_MustHave(TokIdentifier);
  
  if(!err)
    {
    name = PRSI_LexGetIdentifier();
    if (name == NULL)
      {
      PRS_Error(IDS_ERR_FNDEC_NULL_ID);
      err = TRUE;
      }
    }

  if(!err)
    {
    
    err = PRSI_CheckDuplicateIdentifier(name);
    if(err)
      PRS_Error(IDS_ERR_FNDEC_DUP_NAME,name);
    }

  /* Try and create the identifier */
  if(!err)
    {
    this = PRSI_NewIdentifier(name,link);
    if(this == NULL)
      {
      PRS_Error(IDS_ERR_FNDEC_MEMORY,name);
      err = TRUE;
      }
    }

  /* Set identifier class & set up its formal parameter list. If
  // we are linking the identifier to the list then it is a normal
  // global fn declaration, otherwise it is a function parameter.
  */
  if (!err)
    {
    this->class = FunctionIdentifier;
    this->scope = (link)?GlobalScope:ValueParamScope;

    PRSI_LexPushBackToken(tok = PRSI_LexGetToken());
    if(tok == TPLeftParen)
      {
      this->link = PRSI_FormalParameterList();
      err = (this->link == NULL);
      }
    else
      this->link = NULL;  /* no formal parameter list */
    }


  /* Get : return-type */
  if(!err)
    err = PRSI_MustHave(TPColon);

  if(!err)
    {
    this->type = PRSI_Type();
    if(this->type == NULL)
      err = TRUE;
    }


  if (err)
    this = NULL;

  return this;
}


/**********************************************************************
* PRSI_FunctionDeclaration.
* Parses a function declaration. (& definition)
* The "function" token has already been read in.
*
* function-declaration = function-heading ";" function-body.
* 
* Returns TRUE on error FALSE if ok.
**********************************************************************/
int PRSI_FunctionDeclaration(void)
{
  int err = FALSE;
  IdentifierT *id = NULL;

  PRSI_PlantNewSegment();        /* initialise code planting for this proc */

  id = PRSI_FunctionHeading(LINK);
  err = (id == NULL);

  if(!err)
    PRSI_StartLocalScope(id);  /* also makes formal param list visible */

  if(!err)
    err = PRSI_MustHave(TPSemicolon);

  if(!err)
    err = PRSI_Block(id);

  PRSI_EndLocalScope();    /* even if no error: want to clear scope regardless*/

  return err;
}



