/* 
// File:        PROCDEC.C
// 
// Prefix:      
// 
// Description: This parses a procedure declaration
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
#include "procdec.h"
#include "plant.h"
#include "malloc.h"

/**********************************************************************
* PRSI_ProcedureHeading
* Parses a procedure heading.  The keyword "procedure" has already
* been read in.
* If link is true then the identifier is linked into the current
* scope, if false it is not
*
* procedure-heading = "procedure" identifier [ formal-parameter-list ].
*
**********************************************************************/
IdentifierT* PRSI_ProcedureHeading(int link)
{
  int err = FALSE;
  char *name = NULL;
  IdentifierT *this = NULL;
  int tok;

  err = PRSI_MustHave(TokIdentifier);
  
  if(!err)
    {
    name = PRSI_LexGetIdentifier();
    if (name == NULL)
      {
      PRS_Error(IDS_ERR_PROCDEC_NULL_ID);
      err = TRUE;
      }
    }

  if(!err)
    {
    err = PRSI_CheckDuplicateIdentifier(name);
    if(err)
      PRS_Error(IDS_ERR_PROCDEC_DUP_NAME,name);
    }

  /* Try and create the identifier */
  if(!err)
    {
    this = PRSI_NewIdentifier(name,link);
    if(this == NULL)
      {
      PRS_Error(IDS_ERR_PROCDEC_NO_MEMORY,name);
      err = TRUE;
      }
    }

  /* Set identifier class & set up its formal parameter list */
  if (!err)
    {
    this->class = ProcedureIdentifier;
    this->scope = (link) ? GlobalScope : ValueParamScope;
    this->type = NULL;

    PRSI_LexPushBackToken(tok = PRSI_LexGetToken());
    if(tok == TPLeftParen)
      {
      this->link = PRSI_FormalParameterList();
      err = (this->link == NULL);
      }
    else
      this->link = NULL;
    }

  if (err)
    this = NULL;

  return this;
}

/**********************************************************************
* PRSI_ProcedureDeclaration.
* Parses a procecdure declaration. (& definition)
* The "procedure" token has already been read in.
*
* procedure-declaration = procedure-heading ";" procedure-body.
* 
* Returns TRUE on error FALSE if ok.
**********************************************************************/
int PRSI_ProcedureDeclaration(void)
{
  int err = FALSE;
  IdentifierT *id = NULL;

  PRSI_PlantNewSegment();        /* initialise code planting for this proc */

  id = PRSI_ProcedureHeading(LINK);
  err = (id == NULL);

  if(!err)
    PRSI_StartLocalScope(id);    /* also makes formal param list visible */

  if(!err)
    err = PRSI_MustHave(TPSemicolon);

  if(!err)
    err = PRSI_Block(id);

  PRSI_EndLocalScope();    /* even if no error: want to clear scope regardless*/

  return err;
}


