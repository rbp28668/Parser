/* 
// File:        RECORD.C
// 
// Prefix:      NONE
// 
// Description: Code for parsing a record declaration.
//
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "prsif.h"
#include "parser.h"
#include "record.h"
#include "error.h"
#include "ident.h"
#include "tokens.h"
#include "musthave.h"
#include "malloc.h"

/**************************************************************
* FixFieldOffsets
* This runs through the record setting up the offsets of
* each field from the start of the record.
* This returns the total size of the record.
**************************************************************/
static int FixFieldOffsets(TypeT *record)
{
  int size = 0;
  IdentifierT *this = NULL;

  assert(record != NULL);
  assert(record->BasicType == RecordType);

  for(this = record->data.record.FieldList; this != NULL; this = this->link)
    {
    this->value.offset = size;
    size += this->type->size;
    }
  return size;
}


/**************************************************************
* PRSI_FindRecordField
* Given a record identifier and a field name this scans the
* field list for that record and, assuming that name is a valid
* field then it returns a pointer to the field identifier.
**************************************************************/
IdentifierT *PRSI_FindRecordField(IdentifierT *parent, const char *name)
{
  TypeT *type = NULL;
  IdentifierT *id = NULL;

  assert(parent != NULL);
  assert(name != NULL);
  assert(parent->type != NULL);
  
  type = parent->type;

  /* Unwind any array references */
  while (type->BasicType == ArrayType)
    type = type->data.array.Type;

  if (type->BasicType != RecordType)
    return NULL;

  id = type->data.record.FieldList;

  while (id != NULL)
    {
    assert(id->name != NULL);
    if (strcmp(id->name,name) == 0)
      return id;
    id = id->link;
    }

  return NULL;  /* Not found */
}

/**************************************************************
*  IdentifierList
*  Creates a list of identifiers which are not linked into
*  the main symbol tables but are linked together with
*  their link fields. Each identifier is placed onto the
*  end of the list as it is found: the list is kept in the
*  order in which the fields are declared.
*
*  identifier-list = identifier { "," identifier }.
*
**************************************************************/
static int IdentifierList(IdentifierT **list)
{
  IdentifierT *here = NULL;
  IdentifierT *this = NULL;
  IdentifierT **tail = NULL;
  int tok;
  char *name = NULL;
  int err = FALSE;

  assert(list != NULL);

  /* List is initially empty */  
  *list = NULL;
  tail = list;
  
  do
    {
    err = PRSI_MustHave(TokIdentifier);
    if (err)
      break;
            
    name = PRSI_LexGetIdentifier();
    if (name == NULL)
      {
      PRS_Error(IDS_ERR_RECORD_NULL_IDENT);
      err = TRUE;
      break;
      }

    /* Look for duplicate fields */
    for (here = *list;here!=NULL;here = here->link)
      {
      assert(here->name != NULL);
      if (strcmp(name,here->name)==0)
        {
        PRS_Error(IDS_ERR_RECORD_DUP_FIELD,name);
        err = TRUE;
        break;
        }
      }

    /* Try and create the identifier (don't put on main list)*/
    if(!err)
      {
      this = PRSI_NewIdentifier(name,NOLINK);
      if(this == NULL)
        {
        PRS_Error(IDS_ERR_RECORD_IDENTIFIER_MEM,name);
        err = TRUE;
        }
      }

    /* Set identifier class & link it into local list */
    if (!err)
      {
      this->class = FieldIdentifier;
      this->scope = RecordScope;
      *tail = this;           /* tack onto end of list */
      tail = &this->link;     /* new tail ptr */
      this->link = NULL;      /* this is last entry */
      }

    tok = PRSI_LexGetToken();
    }while(tok == TPComma && !err);

  if(!err)
    PRSI_LexPushBackToken(tok);

  return err;
}

/**************************************************************
*  RecordSection
*  Parses a record section in a field list.  The identifiers in
*  this record section are returned as a linked list.  It is up
*  to the calling function to link this section list into the
*  main list of field identifiers.
*
*  record-section = identifier-list ":" type.
*
***************************************************************/
static int RecordSection(IdentifierT **list)
{
  int err = FALSE;
  IdentifierT *here = NULL;
  TypeT *idtype = NULL;

  assert(list != NULL);
  
  err = IdentifierList(list);

  if(!err)
    err = PRSI_MustHave(TPColon);

  if(!err)
    {
    idtype = PRSI_Type();
    if (idtype == NULL)
      {
      err = TRUE;
      }
    }

  /* Set up the type for all the identifiers in the list*/
  if(!err)
    {
    for (here = *list;here != NULL; here = here->link)
      {
      here->type = idtype;
      }
    }

  return err;
}


/**************************************************************
*  FieldList
*  FieldList parses a field list for a record and sets up a list of
*  identifiers containing the fields.
*  This returns a list of fields which are linked together by their
*  link field (not the next field)
*
*  field-list = record-section { ";" record-section }
*
**************************************************************/

static IdentifierT *FieldList(void)
{
  int err = FALSE;
  int tok;
  IdentifierT *list = NULL;
  IdentifierT *section = NULL;
  IdentifierT **tail = NULL;
  IdentifierT *here = NULL;

  tail = &list;

  do
    {

    /* A quick check in case the user has added an extra
    *  semicolon on the end of the last record section. This
    *  should allow the parser to recover from this.
    */
    tok = PRSI_LexGetToken();
    PRSI_LexPushBackToken(tok);
    if(tok == TKEnd)
      {
      /* Don't flag an error */
      break;
      }

    /* Read in a record section. Any identifiers in this section will
    *  be returned in the list "section".  This list should then be
    *  linked onto the end of the field list.
    */

    section = NULL;
    err = RecordSection(&section);
    if(err)
      break;

    assert(section != NULL);

    *tail = section;  /* add section on to the end of the list */

    
    /* Find the end of the section */
    for (here = section;here->link != NULL; here = here->link)
      ; /* NOP */

    /* here now points to the last entry in the list so we can
    *  now update the tail pointer.
    */
    tail = &here->link;
    
    /* Now see if there is a semicolon: this indicates more record
    *  sections
    */
    tok = PRSI_LexGetToken();
    if (tok != TPSemicolon)
      PRSI_LexPushBackToken(tok);
    }
  while (tok == TPSemicolon);

  if(err)
    PRS_Error(IDS_ERR_RECORD_FIELD_LIST_ERROR);

  return list;
}



/**********************************************************************
* PRSI_ParseRecordType
* Parses a record type. N.B. the Record keyword has already been
* read in.
* 
* record-type = "record" field-list "end".
* 
**********************************************************************/
TypeT* PRSI_ParseRecordType(void)
{
  TypeT* type = PRSI_NewType();
  if (type == NULL)
    PRS_Error(IDS_ERR_RECORD_TYPE_MEM);
  else
    {
    type->BasicType = RecordType;
    type->data.record.FieldList = FieldList();
    type->size = FixFieldOffsets(type);

    if(PRSI_MustHave(TKEnd))
      type = NULL;
    }
  return type;
}



