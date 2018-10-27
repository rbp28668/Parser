/* 
// File:        FPLIST.C
// 
// Prefix:
// 
// Description:  Parse formal parameter list.
//
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "prsif.h"
#include "parser.h"
#include "ident.h"
#include "error.h"
#include "lex.h"
#include "tokens.h"
#include "musthave.h"
#include "procdec.h"
#include "fndec.h"
#include "malloc.h"

#define PARAM_OFFSET -8 /* offset relative to FP of the last param */

/**************************************************************
* SetParamOffset
* This sets up the offsets from the frame pointer of
* all the variables in the formal parameter list. This
* function travels recursively along the list of formal
* parameters until it reaches the end.
* fplist is the identifier to be set up.
* Returns the offset of that identifier.
**************************************************************/
static int SetParamOffset(IdentifierT *fplist)
{
  int lsize = 0;
  
  if(fplist == NULL)
    lsize = PARAM_OFFSET;
  else
    {
    switch (fplist->class)
      {
      case VariableIdentifier:
        if(fplist->scope == ValueParamScope)
          {
          assert(fplist->type != NULL);
          lsize = -fplist->type->size;     /* passed by value */
          }
        else
          lsize = -(int)sizeof(void *);    /* passed by address */
        break;

      case FunctionIdentifier:
      case ProcedureIdentifier:
        lsize = -(int)sizeof(void *);   /* passed by address */
        break;

      default:
        PRS_Error(IDS_ERR_FPLIST_INVALID_PARAM_CLASS);
        break;
      }
    lsize += SetParamOffset(fplist->next);
    fplist->value.FPOffset = lsize;
    }
  return lsize;
}

/**************************************************************
*  IdentifierList
*  Creates a list of identifiers which are not linked into
*  the main symbol tables but are linked together with
*  their next fields.
*
*  list points to the head entry of the list.
*  scope is the scope of all the identifiers that will be
*  put onto the list: either VarParamScope or ValueParamScope.
*
*  identifier-list = identifier { "," identifier }.
*
**************************************************************/
static int IdentifierList(IdentifierT **list,IdentifierScopeT scope)
{
  IdentifierT *here = NULL;
  IdentifierT *this = NULL;
  IdentifierT **tail = NULL;
  int tok;
  char *name = NULL;
  int err = FALSE;

  assert(list != NULL);
  
  do
    {
    err = PRSI_MustHave(TokIdentifier);
    if (err)
      break;
            
    name = PRSI_LexGetIdentifier();
    if (name == NULL)
      {
      PRS_Error(IDS_ERR_FPLIST_NULL_IDENT);
      err = TRUE;
      }

    /* Look for duplicate formal parameters and also set
    // tail to point to the last entry on the list
    */
    tail = list;
    for (here = *list; here!=NULL; here = here->next)
      {
      if (strcmp(name,here->name)==0)
        {
        PRS_Error(IDS_ERR_FPLIST_DUP_FORMAL_PARAM,name);
        err = TRUE;
        break;
        }
      tail = &(here->next);
      }


    /* Try and create the identifier */
    if(!err)
      {
      this = PRSI_NewIdentifier(name,NOLINK);
      if(this == NULL)
        {
        PRS_Error(IDS_ERR_FPLIST_FP_IDENT_MEM,name);
        err = TRUE;
        }
      }

    /* Set identifier class & scope and link it into local list */
    if (!err)
      {
      this->class = VariableIdentifier;
      this->scope = scope;
      
      *tail = this;           /* tack onto end of list */
      this->next = NULL;      /* this is last entry */
      }

    tok = PRSI_LexGetToken();
    }while(tok == TPComma && !err);

  if(!err)
    PRSI_LexPushBackToken(tok);

  return err;
}

/**********************************************************************
* PRSI_ValueParameterSection
*
* This parses a value parameter section.
* list is a pointer to the formal parameter list head
*
* value-parameter-section = identifier-list ":" parameter-type.
*
**********************************************************************/
int PRSI_ValueParameterSection(IdentifierT **list)
{
  int err = FALSE;
  TypeT* type = NULL;
  IdentifierT **start = NULL;
  IdentifierT *here = NULL;
  
  assert(list != NULL);
    
  /* Get the current tail of the list */
  start = list;
  for (here = *list; here!=NULL; here = here->next)
    start = &(here->next);

  /* Tack on any identifiers in this list */
  err = IdentifierList(list,ValueParamScope);

  if(!err)
    err = PRSI_MustHave(TPColon);

  if(!err)
    {
    type = PRSI_Type();
    if(type != NULL)
      {
      /* Set the type of all the new entries */
      for( here = *start; here != NULL; here = here->next)
        here->type = type;
      }
    else
      err = TRUE;   /* NULL type */
    }
  return err;
}

/**********************************************************************
* PRSI_VariableParameterSection
*
* This parses a var parameter section
* list is a pointer to the formal parameter list head.
*
* variable-parameter-section = "var" identifier-list ":" parameter-type.
*
**********************************************************************/
int PRSI_VariableParameterSection(IdentifierT **list)
{
  int err = FALSE;
  TypeT* type = NULL;
  IdentifierT **start = NULL;
  IdentifierT *here = NULL;

  assert(list != NULL);  
  
  /* Get the current tail of the list */
  start = list;
  for (here = *list; here!=NULL; here = here->next)
    start = &(here->next);

  /* Tack on any identifiers in this list */
  err = IdentifierList(list,VarParamScope);

  if(!err)
    err = PRSI_MustHave(TPColon);

  if(!err)
    {
    type = PRSI_Type();
    if(type != NULL)
      {
      /* Set the type of all the new entries */
      for( here = *start; here != NULL; here = here->next)
        here->type = type;
      }
    else
      err = TRUE;   /* NULL type */
    }
  return err;
}

/**********************************************************************
* PRSI_ProcedureParameterSection
* Parses a procedure parameter section. Note that the keyword
* "procedure" has already been read in.  The procedure identifier
* is tacked on to the tail of the parameter list.
*
* procedure-parameter-section = procedure-heading
*
**********************************************************************/
int PRSI_ProcedureParameterSection(IdentifierT **list)
{
  int err = FALSE;
  IdentifierT *here = NULL;
  IdentifierT **tail = NULL;
  IdentifierT* procid = NULL;

  assert(list != NULL);
    
  procid = PRSI_ProcedureHeading(NOLINK);
  err = (procid == NULL);

  if(!err)
    {
    /* Look for duplicate formal parameters and also set
    // tail to point to the last entry on the list
    */
    tail = list;
    for (here = *list; here!=NULL; here = here->next)
      {
      assert(procid->name != NULL);
      assert(here->name != NULL);
      if (strcmp(procid->name,here->name)==0)
        {
        PRS_Error(IDS_ERR_FPLIST_DUP_PROC_FP,procid->name);
        err = TRUE;
        break;
        }
      tail = &(here->next);
      }
    }

  if(!err)
    {
    procid->scope = ValueParamScope;
    *tail = procid;
    procid->next = NULL;
    }
  return err;
}

/**********************************************************************
* PRSI_FunctionParameterSection
* Parses a function parameter section. Note that the keyword
* "function" has already been read in.  The function identifier
* is tacked on to the tail of the parameter list.
*
* function-parameter-section = function-heading.
*
**********************************************************************/
int PRSI_FunctionParameterSection(IdentifierT **list)
{
  int err = TRUE;
  IdentifierT *here = NULL;
  IdentifierT **tail = NULL;
  IdentifierT* fnid = NULL;

  assert(list != NULL);
    
  fnid = PRSI_FunctionHeading(NOLINK);
  err = (fnid == NULL);

  if(!err)
    {
    /* Look for duplicate formal parameters and also set
    // tail to point to the last entry on the list
    */
    tail = list;
    for (here = *list; here!=NULL; here = here->next)
      {
      if (strcmp(fnid->name,here->name)==0)
        {
        assert(fnid->name != NULL);
        assert(here->name != NULL);
        PRS_Error(IDS_ERR_FPLIST_DUP_FN_FP,fnid->name);
        err = TRUE;
        break;
        }
      tail = &(here->next);
      }
    }

  if(!err)
    {
    fnid->scope = ValueParamScope;
    *tail = fnid;
    fnid->next = NULL;
    }
  return err;
}


/**********************************************************************
* PRSI_FormalParameterSection
*
* This parses a FormalParameterSection
* list is a pointer to the formal parameter list head.
*
* formal-parameter-section = value-parameter-section |
*   variable-parameter-section |
*   procedure-parameter-section |
*   function-parameter-section.
*
**********************************************************************/
int PRSI_FormalParameterSection(IdentifierT **list)
{
  int tok;
  int err = FALSE;

  assert(list != NULL);
  
  tok = PRSI_LexGetToken();

  switch (tok)
    {
    case TKVar: 
      err = PRSI_VariableParameterSection(list);
      break;

    case TKProcedure:
      err = PRSI_ProcedureParameterSection(list);
      break;

    case TKFunction:
      err = PRSI_FunctionParameterSection(list);
      break;

    case TokIdentifier:
      PRSI_LexPushBackToken(tok);
      err = PRSI_ValueParameterSection(list);
      break;

    default:
      PRS_Error(IDS_ERR_FPLIST_SYNTAX);
      err = TRUE;
      break;
    }

  return err;
}

/**********************************************************************
* PRSI_FormalParameterList
*
* This sets up a list of identifiers which are the formal parameter
* list for the function or procedure
*
* formal-parameter-list = "(" formal-parameter-section 
* { ";" formal-parameter-section } ")".
*
**********************************************************************/
IdentifierT* PRSI_FormalParameterList(void)
{
  IdentifierT* list = NULL;
  int tok;
  int err;

  err = PRSI_MustHave(TPLeftParen);

  if(!err)
    {
    do
      {
      err = PRSI_FormalParameterSection(&list);
      tok = PRSI_LexGetToken();
      }
    while (!err && tok == TPSemicolon);
    }

  if(!err)
    {
    PRSI_LexPushBackToken(tok);
    err = PRSI_MustHave(TPRightParen);
    }

  if(!err)
    SetParamOffset(list); /* set up offsets of parameters w.r.t FP */
    
  return list;
}


