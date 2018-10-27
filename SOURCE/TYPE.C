/*
// File:        TYPE.C
// 
// Prefix:      NONE
// 
// Description: Code for parsing type definitions.
//
*/
#include <stdio.h>
#include <memory.h>
#include <assert.h>
#include "prsif.h"
#include "lex.h"
#include "parser.h"
#include "musthave.h"
#include "error.h"
#include "tokens.h"
#include "ident.h"
#include "record.h"
#include "enums.h"
#include "malloc.h"

/*================ TYPE DEFINITION ===================*/

/**********************************************************************
* PRSI_IsOrdinalType
* Returns TRUE iff the given type is an ordinal type.
**********************************************************************/
int PRSI_IsOrdinalType(TypeT* type)
{
  int is = FALSE;
  
  assert(type != NULL);
  switch (type->BasicType)
    {
    case IntegerType:
    case EnumeratedType:
      is = TRUE;
      break;
    }
  return is;
}

/**********************************************************************
* PRSI_GetOrdinalBounds
* Gets the lower and upper bound for an ordinal type
**********************************************************************/
int PRSI_GetOrdinalBounds(TypeT* type, long* lower, long* upper)
{
  int err = FALSE;
  
  assert(type != NULL);
  assert(lower != NULL);
  assert(upper != NULL);
  
  switch (type->BasicType)
    {
    case IntegerType:
      *lower = type->data.integer.LowerBound;
      *upper = type->data.integer.UpperBound;
      break;

    case EnumeratedType:
      *lower = type->data.enumeration.LowerBound;
      *upper = type->data.enumeration.UpperBound;
      break;

    default:
      PRS_Error(IDS_ERR_TYPE_BOUNDS,type->data.array.IndexType->BasicType);
      err = TRUE;
      break;
      }
  return err;
}

/**********************************************************************
* ParseArrayType
* Parses an array type. N.B. The keyword array has already been
* read in.
*
* array-type = "array" "[" index-type "]" "of" type.    ; N.B. only 1D arrays
*
* index-type = type.    ;N.B. must be an ordinal type.
* 
**********************************************************************/
static TypeT* ParseArrayType(void)
{
  TypeT* type = NULL;
  int err = FALSE;
  long lsize,usize;

  type = PRSI_NewType();
      
  if (type == NULL)
    {
    PRS_Error(IDS_ERR_TYPE_ARRAY_MEM);
    err = TRUE;
    }
  else
    type->BasicType = ArrayType;

  if(!err)
    err = PRSI_MustHave(TPLeftBracket);

  if(!err)
    {
    type->data.array.IndexType = PRSI_Type();
    if(type->data.array.IndexType == NULL)
      err = TRUE;
    else if(!PRSI_IsOrdinalType(type->data.array.IndexType))
      {
      PRS_Error(IDS_ERR_TYPE_INDEX_NOT_ORDINAL);
      err = TRUE;
      }
    }

  if(!err)
    err = PRSI_MustHave(TPRightBracket);

  if(!err)
    err = PRSI_MustHave(TKOf);

  if(!err)
    {
    type->data.array.Type = PRSI_Type();
    if(type->data.array.Type == NULL)
      err = TRUE;
    else /* calculate size (in bytes) of array */
      {
      err = PRSI_GetOrdinalBounds(type->data.array.IndexType,&lsize,&usize);
      type->size = type->data.array.Type->size * (int)(1 + usize - lsize);
      }
    }
  if(err)
    type = NULL;

  return type;
}


/**********************************************************************
* StructuredType
* Parses a structured-type (an array or record type)
* 
* structured-type = array-type | record-type. ; N.B. no packed arrays or
*                                             ; records, sets or files
* 
**********************************************************************/
static TypeT* StructuredType(void)
{
  int tok;
  TypeT *type = NULL;

  tok = PRSI_LexGetToken();

  switch (tok)
    {
    case TKRecord:
      type = PRSI_ParseRecordType();
      break;

    case TKArray:
      type = ParseArrayType();
      break;

    default:
      PRS_Error(IDS_ERR_TYPE_NOT_RECORD_OR_ARRAY,PRSI_LexTranslateToken(tok));
      type = NULL;
    }

  return type;
}

/**********************************************************************
* SubrangeType
* Both constants must be of an ordinal type and the types must be
* the same.
*
* subrange-type = constant ".." constant  
* 
**********************************************************************/
static TypeT* SubrangeType(void)
{
  TypeT *type = NULL;
  IdentifierT LowerConst;
  IdentifierT UpperConst;
  int err = FALSE;
  
  memset(&LowerConst,0,sizeof(LowerConst));
  memset(&UpperConst,0,sizeof(UpperConst));
  
  err = PRSI_Constant(&LowerConst);

  if(!err)
    err = PRSI_MustHave(TPDotDot);

  if(!err)
    err = PRSI_Constant(&UpperConst);

  if(!err)
    {
    assert(LowerConst.type != NULL);
    assert(UpperConst.type != NULL);
    
    if (!PRSI_IsOrdinalType(LowerConst.type))
      {
      PRS_Error(IDS_ERR_TYPE_LB_NOT_ORDINAL);
      err = TRUE;
      }
    else if (!PRSI_IsOrdinalType(UpperConst.type))
      {
      PRS_Error(IDS_ERR_TYPE_UB_NOT_ORDINAL);
      err = TRUE;
      }
    else if (LowerConst.type != UpperConst.type)
      {
      PRS_Error(IDS_ERR_TYPE_LB_UP_TYPE_MISMATCH);
      err = TRUE;
      }
    else if(LowerConst.value.ordinal > UpperConst.value.ordinal)
      {
      PRS_Error(IDS_ERR_TYPE_LB_GT_UB);
      err = TRUE;
      }
    }

  if(!err)
    {
    type = PRSI_NewType();
    if(type == NULL)
      {
      PRS_Error(IDS_ERR_TYPE_SUBRANGE_MEM);
      err = TRUE;
      }
    }

  if(!err)
    {
    switch (LowerConst.type->BasicType)
      {
      case IntegerType:
        *type = *LowerConst.type;
        type->data.integer.LowerBound = LowerConst.value.ordinal;
        type->data.integer.UpperBound = UpperConst.value.ordinal;
        break;

      case EnumeratedType:
        *type = *LowerConst.type;
        type->data.enumeration.LowerBound = LowerConst.value.ordinal;
        type->data.enumeration.UpperBound = UpperConst.value.ordinal;
        break;

      default:
        PRS_Error(IDS_ERR_TYPE_SUBRANGE);
        type = NULL;
        break;
      }
    }
  return type;
}

/**********************************************************************
* SimpleType
*
* simple-type = enumerated-type | subrange-type
*
**********************************************************************/
static TypeT* SimpleType(void)
{
  TypeT *type = NULL;
  int tok;
    
  tok = PRSI_LexGetToken();
  PRSI_LexPushBackToken(tok);

  switch (tok)
    {
    case TPLeftParen:
      type = PRSI_ParseEnumeratedType();
      break;

    default:
      type = SubrangeType();
      break;
    }
  return type;
}

/**********************************************************************
* PRSI_Type
* Parses and sets up a type definition
* 
* type = simple-type | structured-type | type-identifier.
* 
* type-identifier = identifier.
* 
**********************************************************************/
TypeT *PRSI_Type(void)
{
  int tok;
  TypeT *type = NULL;
  IdentifierT *ident = NULL;
  char *name = NULL;
  int err = FALSE;

  tok = PRSI_LexGetToken();

  switch (tok)
    {
    case TKRecord:    
    case TKArray:
      PRSI_LexPushBackToken(tok);
      type = StructuredType(); 
      break;

    case TokIdentifier: /* either an existing type OR a constant */

      ident = PRSI_FindIdentifier(name = PRSI_LexGetIdentifier());
      if(ident == NULL)
        {
        PRS_Error(IDS_ERR_TYPE_MISSING_IDENT,name);
        err = TRUE;
        }

      if(!err)
        {
        switch (ident->class)
          {
          case TypeIdentifier:
            type = ident->type; /* use existing type */
            break;

          case ConstantIdentifier:
            PRSI_LexPushBackToken(tok);
            type = SimpleType(); /* should be subrange type */
            break;

          default:
            PRS_Error(IDS_ERR_TYPE_NOT_TYPE_OR_CONSTANT,name);
            err = TRUE;
            type = NULL;
            break;
           
          }
        }

      break;
    
    case TokInt:             /* should be a subrange type */
    case TPLeftParen:        /* should be an enumerated type */
      PRSI_LexPushBackToken(tok);
      type = SimpleType();    
      break;

    default:
      PRS_Error(IDS_ERR_TYPE_UNKNOWN);
      type = NULL;
      break;

    }

  return type;
}



/**********************************************************************
* Parses a type definition. The type keyword has already been
* read in.
* 
* type-definition = "type" identifier "=" type
* 
**********************************************************************/
static int TypeDefinition(void)
{
  int err;
  char *name = NULL;
  IdentifierT *ident = NULL;

  err = PRSI_MustHave(TokIdentifier);
  if(!err)
    {
    name = PRSI_LexGetIdentifier();
    if (PRSI_FindIdentifier(name) != NULL)
      {
      PRS_Error(IDS_ERR_TYPE_DUP_NAME,name);
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
    ident->class = TypeIdentifier;
    ident->scope = (PRSI_InLocalScope())?LocalScope:GlobalScope;
    ident->type = PRSI_Type();
    if(ident->type == NULL)
      err = TRUE;
    }

  return err;

}


/**********************************************************************
* PRSI_TypeDefinitionPart
*
* type-definition-part = type-definition ";" { type-definition ";" }.
*
**********************************************************************/
int PRSI_TypeDefinitionPart(void)
{
  int tok;
  int err = FALSE;

  err = PRSI_MustHave(TKType);

  if (!err)
    {
    err = TypeDefinition();
    if(!err) err = PRSI_MustHave(TPSemicolon);
    if(!err) tok = PRSI_LexGetToken();

    while (!err && tok == TKType)
      {
      err = TypeDefinition();
      if(!err) err = PRSI_MustHave(TPSemicolon);
      if(!err) tok = PRSI_LexGetToken();
      }
    if(!err)
      PRSI_LexPushBackToken(tok);
    }

  return err;
}
