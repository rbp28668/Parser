/* 
// File:          ENUMS.C
// 
// Prefix:        NONE
// 
// Description:   This handles parsing of enumerated types
//
*/

#include <stdio.h>
#include <assert.h>
#include "prsif.h"
#include "parser.h"
#include "musthave.h"
#include "error.h"
#include "tokens.h"
#include "ident.h"
#include "enums.h"
#include "malloc.h"

/**********************************************************************
* EnumerationList reads in the list of identifiers forming
* an enumerated type.  These identifiers are placed in the
* appropriate scope and the start of the list is returned.
* The number of items in the list is returned in *count.
* The type of this enumerated type is passed in type
* Each new identifier is placed on the tail of the list: the list
* is returned in the order which the items are declared.
*
**********************************************************************/
static IdentifierT *EnumerationList(int *count, TypeT *type)
{
  char *name = NULL;
  int err = FALSE;
  int tok;
  IdentifierT *ident = NULL;
  IdentifierT *list = NULL;
  IdentifierT** tail = &list;

  assert(count != NULL);
  assert(type != NULL);
  
  *count = 0;

  do
    {
    err = PRSI_MustHave(TokIdentifier);
    if(err)
      break;


    name = PRSI_LexGetIdentifier();
    if (PRSI_FindIdentifier(name) != NULL)
      {
      PRS_Error(IDS_ERR_ENUMS_DUP_NAME);
      err = TRUE;
      }

    ident = PRSI_NewIdentifier(name,LINK);
    if(ident == NULL)
      {
      PRS_Error(IDS_ERR_ENUMS_MEM_ELEMENT);
      break;
      }

    ident->class = ConstantIdentifier;
    ident->scope = PRSI_InLocalScope()?LocalScope:GlobalScope;
    ident->type = type;
    ident->value.ordinal = *count;

    /* tack onto list */
    ident->link = NULL;
    *tail = ident;
    tail = &(ident->link);

    /* Look for , or trailing ) */
    tok = PRSI_LexGetToken();

    ++ *count;
    }
  while(tok == TPComma);

  if (!err)
    {
    PRSI_LexPushBackToken(tok);
    err = PRSI_MustHave(TPRightParen);
    }

  if(err)
    list = NULL;

  return list;
}

/**********************************************************************
* PRSI_ParseEnumeratedType
*
* enumerated-type = "(" identifier-list ")".
*
**********************************************************************/
TypeT* PRSI_ParseEnumeratedType(void)
{
  TypeT* type = NULL;
  int count = 0;
  int err = PRSI_MustHave(TPLeftParen);
  
  if(!err)
    {
    type = PRSI_NewType();
  
    if (type == NULL)
      PRS_Error(IDS_ERR_ENUMS_MEM_TYPE);
    else
      {
      type->BasicType = EnumeratedType;
      type->size = ENUM_LEN;
      type->data.enumeration.list = EnumerationList(&count,type);
      type->data.enumeration.count = count;
      type->data.enumeration.LowerBound = 0;
      type->data.enumeration.UpperBound = count-1;

      if (type->data.enumeration.list == NULL)
        type = NULL;
      else
        type->data.enumeration.BaseType = type; /* its own base type */
      }
    }

  return type;
}
