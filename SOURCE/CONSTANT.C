/*
// File:          CONSTANT.C
// 
// Prefix:        NONE
// 
// Description:   This parses a constant definition after the keyword
//                const has been read in.
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
/*===================== CONSTANT DECLARATION ====================*/

/**********************************************************************
* PRSI_Constant
* Parses a constant for a constant definition. The constant identifier
* that is being set up is passed as ident.
*
* constant = integer-number | real-number | string | [sign] constant-identifier.
*
* This returns FALSE for success, TRUE for failure
**********************************************************************/
int PRSI_Constant(IdentifierT *ident)
{
  int tok;
  int sign = 1;
  IdentifierT *ConstID = NULL;
  int err = FALSE;
  int HasSign = FALSE;

  assert(ident != NULL);

  tok = PRSI_LexGetToken();
  switch (tok)
    {
    case TokInt:
      ident->type = &IntegerTypeDecl;
      ident->value.ordinal = PRSI_LexGetLong();
      break;

    case TokFloat:
      ident->type = &RealTypeDecl;
      ident->value.real = (float)PRSI_LexGetDouble();
      break;

    case TokString:
      ident->type = &StringTypeDecl;
      ident->value.string = PRS_CreateString(PRSI_LexGetString());
      /* Now string is created, immediately reference it */
      if(ident->value.string)
        PRSI_ReferenceString(ident->value.string);
      break;

    case TPMinus:
      sign = -1;  /* FALL THROUGH */

    case TPPlus:  /* do nothing for unary + and FALL THROUGH */
      err = PRSI_MustHave(TokIdentifier);
      if(err)
        {
        PRS_Error(IDS_ERR_CONSTANT_MISSING_ID);
        err = TRUE;
        break;
        }
      HasSign = TRUE;
      /* FALL THROUGH */

    case TokIdentifier:
      ConstID = PRSI_FindIdentifier(PRSI_LexGetIdentifier());
      if(ConstID == NULL)
        {
        PRS_Error(IDS_ERR_CONSTANT_CANT_FIND_ID);
        err = TRUE;
        break;
        }

      if(!err && ConstID->class != ConstantIdentifier)
        {
        PRS_Error(IDS_ERR_CONSTANT_NON_CONST_ID);
        err = TRUE;
        }

      /* Inherit type and value from exisiting constant */
      if(!err)
        {
        assert(ConstID->type != NULL);
        ident->type = ConstID->type;
        switch (ident->type->BasicType)
          {
          case IntegerType:
            ident->value.ordinal = sign * ConstID->value.ordinal;
            break;

          case RealType:
            ident->value.real = sign * ConstID->value.real;
            break;

          case StringType:
            if(HasSign)
              {
              PRS_Error(IDS_ERR_CONSTANT_SIGN_STRING);
              err = TRUE;
              }
            ident->value.addr = ConstID->value.addr;
            break;

          case EnumeratedType:
            if(HasSign)
              {
              PRS_Error(IDS_ERR_CONSTANT_SIGN_ENUM);
              err = TRUE;
              }
            ident->value.ordinal = ConstID->value.ordinal;
            break;

          default:
            PRS_Error(IDS_ERR_CONSTANT_INVALID_TYPE);
            err = TRUE;
            break;
          }

        }
      break;

    default:
      PRS_Error(IDS_ERR_CONSTANT_NOT_VALID);
      err = TRUE;
      break;
    }
  return err;
}

/**********************************************************************
* ConstantDefinition
* parses a constant definition. The initial "const" keyword has
* already been read in.
*
* constant-definition = "const" identifier "=" constant
*
* This returns FALSE for success, TRUE for failure.
**********************************************************************/
static int ConstantDefinition(void)
{
  int err = FALSE;
  char *name = NULL;
  IdentifierT *ident = NULL;

  err = PRSI_MustHave(TokIdentifier);

  if(!err)
    {
    name = PRSI_LexGetIdentifier();
    if (PRSI_FindIdentifier(name) != NULL)
      {
      PRS_Error(IDS_ERR_CONSTANT_DUP_NAME);
      err = TRUE;
      }
    }

  if (!err)
      err = PRSI_MustHave(TPEquals);

  if (!err)
    {
    ident = PRSI_NewIdentifier(name,LINK);
    err = (ident == NULL);
    }

  if(!err)
    {
    ident->class = ConstantIdentifier;
    ident->scope = PRSI_InLocalScope() ? LocalScope : GlobalScope;
    err = PRSI_Constant(ident);
    }

  return err;
}


/**********************************************************************
* PRSI_ConstantDefinitionPart
*
* constant-definition-part = constant-definition ";"
*                            { constant-defintion ";" }.
*
**********************************************************************/
int PRSI_ConstantDefinitionPart(void)
{
  int tok;
  int err = FALSE;

  err = PRSI_MustHave(TKConst);

  if(!err)
    {
    err = ConstantDefinition();
    if(!err) err = PRSI_MustHave(TPSemicolon);
    if(!err) tok = PRSI_LexGetToken();

    while (!err && tok == TKConst)
      {
      err = ConstantDefinition();
      if(!err) err = PRSI_MustHave(TPSemicolon);
      if(!err) tok = PRSI_LexGetToken();
      }
    if(!err)
      PRSI_LexPushBackToken(tok);
    }

  return err;
}
