/*
// File:        IDENT.C
//
// Prefix:      NONE
//
// Description: Code for creating and storing identifiers and types
//
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "prsif.h"
#include "lex.h"
#include "malloc.h"
#include "parser.h"
#include "ident.h"
#include "builtin.h"
#include "error.h"
#include "printf.h"


/* Flag which is true when we are defining a function or procedure */
static int UseLocalScope = 0;

/* Block memory handles for Global and Local variables. */
static int GlobalBlock = -1;
static int LocalBlock = -1;


/* Heads of linked lists of identifiers for local and global scope 
// along with tail pointers to speed adding new identifiers to
// the tail of the lists
*/
static IdentifierT *GlobalIdentList = NULL; 
static IdentifierT **GlobalIdentTail = &GlobalIdentList;
static IdentifierT *LocalIdentList = NULL;
static IdentifierT **LocalIdentTail = &LocalIdentList;

/* "Fence" to prevent forgetting too much */
static IdentifierT *ForgetFence = NULL;

/* Pointer to formal parameter list for fn or proc */
static IdentifierT *FPList = NULL;


/* Spaces when dumping identifiers */
static int SpaceCount = 0;





/**********************************************************************
* PRSI_New Identifier creates a new identifer. If a function or procedure
*  is being defined then the identifier is given local scope,
*  otherwise it is given global scope. The identifier is linked into the
*  appropriate list.
**********************************************************************/
IdentifierT *PRSI_NewIdentifier(const char *name,int link)
{
  IdentifierT *id = NULL;
  IdentifierT ***tail = NULL;
  int handle = 0;
  char *str = NULL;

  assert(name != NULL);
  
  if (UseLocalScope)
    {
    tail = &LocalIdentTail;
    handle = LocalBlock;
    }
  else
    {
    tail = &GlobalIdentTail;
    handle = GlobalBlock;
    }

  id = (IdentifierT *) PRSI_BlockMalloc(handle,sizeof(IdentifierT));

  if (id != NULL)
    {

    /* Set up identifier name */
    str = PRSI_BlockMalloc(handle,strlen(name)+1);
    if (str != NULL)
      strcpy(str,name);
    id->name = str;

    /* Set up links */
    id->link = NULL;
    id->next = NULL;

    if (link) /* then tack on to the end of the list */
      {
      /* tack onto list */
      **tail = id;
      *tail = &id->next;
      }

    id->class = UndefinedIdentifier;
    id->scope = GlobalScope;
    id->type = NULL;
    id->flags = 0;
    id->value.ordinal = 0;
    }
  return id;
}

/**********************************************************************
* PRSI_NewType
* Creates a new type structure on the local or global memory blocks.
**********************************************************************/
TypeT *PRSI_NewType(void)
{
  int handle = 0;
  TypeT *tp = NULL;

  if (UseLocalScope)
    handle = LocalBlock;
  else
    handle = GlobalBlock;

  tp = (TypeT *) PRSI_BlockMalloc(handle,sizeof(TypeT));

  if(tp != NULL)
    memset(tp,0,sizeof(TypeT));
    
  return tp;
}


/**********************************************************************
* PRSI_FindIdentifier
* FindIdentifier tries to find an exisiting identifier. First any local
*  scope is checked. If there is no match then any formal parameter
*  list is checked.  Finally global scope is checked. If the requested
*  identifier is not found FindIdentifier returns NULL.
**********************************************************************/
IdentifierT *PRSI_FindIdentifier(const char *name)
{

  IdentifierT *id = NULL;
  IdentifierT *Match = NULL;

  assert(name != NULL);
  
  /* Initially look through any local identifiers */
  for(id = LocalIdentList; id != NULL; id=id->next)
    {
    assert(id->name != NULL);
    if(strcmp(id->name,name) == 0)
      {
      Match = id;
      break;
      }
    }

  /* If nothing found and there is a valid formal parameter list
  // then search through this.
  */
  if(Match == NULL && FPList != NULL)
    {
    for(id = FPList; id != NULL; id=id->next)
      {
      assert(id->name != NULL);
      if(strcmp(id->name,name) == 0)
        {
        Match = id;
        break;
        }
      }
    }

  /* If nothing found then look through global identifiers. */
  if(Match == NULL)
    {
    for(id = GlobalIdentList; id != NULL; id=id->next)
      {
      assert(id->name != NULL);
      if(strcmp(id->name,name) == 0)
        {
        Match = id;
        break;
        }
      }
    }

  /* and finally, If nothing found then look through predefined identifiers. */
  if(Match == NULL)
    {
    for(id = PRSI_FirstPredefinedIdent(); id != NULL; id=id->next)
      {
      assert(id->name != NULL);
      if(strcmp(id->name,name) == 0)
        {
        Match = id;
        break;
        }
      }
    }

  return Match;
}

/**********************************************************************
* PRSI_CheckDuplicateIdentifier
* Checks an identifier for a name clash. In local scope local
* identifiers can shadow global ones so only the local list is checked
* otherwise the global list is checked. This returns TRUE if there
* is a clash otherwise false.  
**********************************************************************/
int PRSI_CheckDuplicateIdentifier(const char *name)
{
  IdentifierT *id = NULL;
  int found = FALSE;

  assert(name != NULL);
  
  found = PRSI_LexIsAKeyword(name);

  if(!found)
    {
    if (UseLocalScope)
      {
      for(id = LocalIdentList; id != NULL; id=id->next)
        {
        assert(id->name != NULL);
        if(strcmp(id->name,name) == 0)
          {
          found = TRUE;
          break;
          }
        }
      }
    else
      {
      for(id = GlobalIdentList; id != NULL; id=id->next)
        {
        assert(id->name != NULL);
        if(strcmp(id->name,name) == 0)
          {
          found = TRUE;
          break;
          }
        }
      }
    }

  return found;
}



/**********************************************************************
* PRSI_InitialiseIdentifiers
*  Initialises the identifier (and type) store
**********************************************************************/
void PRSI_InitialiseIdentifiers(void)
{
  GlobalBlock = PRSI_BlockCreate();
  GlobalIdentTail = &GlobalIdentList;
  PRSI_SetupPredefinedIdentifiers();
  return;
}

/**********************************************************************
* PRSI_TerminateIdentifiers
*  Terminates the identifier (and type) store
**********************************************************************/
void PRSI_TerminateIdentifiers(void)
{
  IdentifierT *here = NULL;

  /* Free any strings in memory */
  for(here = GlobalIdentList; here != NULL; here = here->next)
    {
    if(here->class == ConstantIdentifier ||
      here->class == VariableIdentifier)
      { 
      assert(here->type != NULL);
      if(here->type->BasicType == StringType)
        PRS_DeleteString(here->value.string);
      }
    }

  PRSI_BlockFree(GlobalBlock);
  return;
}

/**********************************************************************
* PRSI_StartLocalScope
*  Sets up variables for defining a function or procedure.
*  Any identifiers created between StartLocalScope and EndLocalScope
*  will be held on the local list and will be lost at EndLocalScope.
*  FnOrProc is a pointer to the function or procedure identifier for
*  that encloses this local scope: This allows FindIdentifier to
*  find the formal parameters.
**********************************************************************/
void PRSI_StartLocalScope(IdentifierT *FnOrProc)
{
  assert(FnOrProc != NULL);
  assert(!UseLocalScope);
  
  LocalBlock = PRSI_BlockCreate();
  LocalIdentTail = &LocalIdentList;

  if(FnOrProc != NULL)
    FPList = FnOrProc->link;

  UseLocalScope = 1;
}

/**********************************************************************
* PRSI_EndLocalScope
*  Clears up local variables etc from a function or procedure.
**********************************************************************/
void PRSI_EndLocalScope(void)
{ 
  if(UseLocalScope)
    {
    PRSI_BlockFree(LocalBlock);
    LocalIdentList = NULL;
    LocalIdentTail = &LocalIdentList;
    FPList = 0;
    UseLocalScope = 0;
    }
  return;
}

/**********************************************************************
* PRSI_InLocalScope
* allows functions to test whether we are defining a function or
* procedure.
**********************************************************************/
int PRSI_InLocalScope(void)
{
  return UseLocalScope;
}

/**********************************************************************
* PRSI_GetLocalList
* Gets a pointer to the local variable list
**********************************************************************/
IdentifierT* PRSI_GetLocalList(void)
{
  if (UseLocalScope)
    return LocalIdentList;
  else
    return NULL;
}

/**********************************************************************
* PRSI_AllocateGlobalStore
* Allocates memory from the global memory block
**********************************************************************/
void *PRSI_AllocateGlobalStore(unsigned int size)
{
  return PRSI_BlockMalloc(GlobalBlock,size);
}

/**********************************************************************
* PRSI_StartTransaction
* This records the current state of the identifier list and global
* memory block. The MarkT structure is filled in an can be used to
* roll back the system to its previous state after a failed operation
* using RollBackTransaction.
**********************************************************************/
void PRSI_StartTransaction(MarkT *mark)
{
  assert(mark != NULL);
  assert(!UseLocalScope);    /* should only mark outside proc/fn declarations */
  
  mark->tail = GlobalIdentTail;
  mark->MemMark = PRSI_BlockMark(GlobalBlock);
  return;
}


/**********************************************************************
* PRSI_RollBackTransaction
* This restores the system state recorded by StartTransaction
**********************************************************************/
void PRSI_RollBackTransaction(const MarkT *mark)
{
  assert(mark != NULL);
  
  GlobalIdentTail = mark->tail;
  *GlobalIdentTail = NULL;

  PRSI_BlockFreeFrom(GlobalBlock,mark->MemMark);  
  
  PRSI_EndLocalScope();   /* in case in middle of proc or fn */
  
  return;
}

/**********************************************************************
* PRS_Fence
* This works with Forget and prevents the anything before, and
* including, a particular identifier, being forgotton.
* Returns TRUE on error (can't find identifier) FALSE if forget OK
**********************************************************************/
int PRS_Fence(const char* name)
{
  IdentifierT *id = NULL;
  int err = TRUE;

  id = GlobalIdentList;

  while(id != NULL)
    {
    assert(id->name != NULL);
    if(strcmp(id->name,name) == 0)
      {
      ForgetFence = id;
      err = FALSE;
      break;
      }

    id=id->next;
    }
  return err;

}


/**********************************************************************
* PRS_Forget
* This allows the system to remove all the global identifiers
* from (and including) a specific identifiers.  Any associated
* type information is removed also (they are both stored on the
* global block).  Note that enumerators cannot be "forgotten" as
* they would leave an incomplete definition.
* Returns TRUE on error, FALSE if forget OK.
**********************************************************************/
int PRS_Forget(const char* name)
{
  IdentifierT *id = NULL;
  IdentifierT **prev = NULL;
  IdentifierT *here = NULL;
  int err = TRUE;

  assert(name != NULL);
  
  /* If a "fence" has been set up start after it
  // otherwise search the whole list
  */
  if (ForgetFence == NULL)
    {
    prev = &GlobalIdentList;
    id = GlobalIdentList;
    }
  else
    {
    prev = &(ForgetFence->next);
    id = ForgetFence->next;
    }

  while(id != NULL)
    {
    assert(id->name != NULL);
    if(strcmp(id->name,name) == 0)
      {
      if(id->class == ConstantIdentifier &&
        id->type->BasicType == EnumeratedType)
        PRS_Error(IDS_ERR_IDENT_FORGET_ENUMERATION,name);
      else
        {
        /* Run through list dereferencing any strings as the
        // actual string data is stored on the heap.
        // Also get rid of function and procedure bodies
        */
        for(here = id; here != NULL; here = here->next)
          {
          /* Strings.... */
          if(here->class == ConstantIdentifier ||
             here->class == VariableIdentifier)
            { 
            assert(here->type != NULL);
            if(here->type->BasicType == StringType)
              {
              PRS_DeleteString(here->value.string);
              }
            }
          /* Procedures and functions... */
          if(here->class == FunctionIdentifier ||
             here->class == ProcedureIdentifier)
            {
            if((here->flags & IDF_CCALL) == 0)
              {
              if(here->value.addr != NULL)
                {
                free(here->value.addr);
                here->value.addr = NULL;
                }
              }
            }
          }
        *prev = NULL;           /* chop off list */
        GlobalIdentTail = prev; /* new tail of list */
        PRSI_BlockFreeFrom(GlobalBlock,id); /* and free id and all after it */
        err = FALSE;
        }
      break;
      }

    prev = &(id->next);
    id=id->next;
    }
  return err;
}



/*====================================================================*/


static void ShowIdent(IdentifierT *ident);
static void ShowType(TypeT *type, int IsTypeID);

static char szWriteBuff[76] = ""; /* 80 cols - a few for borders */
static int iWritePos = 0;

/**********************************************************************
* Spaces
* Prints SpaceCount spaces to indent record fields.
**********************************************************************/
static void Spaces(void)
{
  int i;
  for (i=0;i<SpaceCount && iWritePos < sizeof(szWriteBuff)-1; ++i)
    szWriteBuff[iWritePos++] = ' ';
  szWriteBuff[iWritePos] = '\0';
}

static void Newline(void)
{
  PRS_Write(szWriteBuff);
  strcpy(szWriteBuff,"");
  iWritePos = 0;
}

static void Send(const char* str)
{
  assert(str != NULL);
  
  if(iWritePos + strlen(str) < sizeof(szWriteBuff)-1)
    {
    strcat(szWriteBuff,str);
    iWritePos += strlen(str);
    }
  else /* won't fit */
    {
    /* so write out anything already on the line */
    Newline();

    /* indent it a bit */
    SpaceCount += 2;
    Spaces();
    SpaceCount -= 2;

    strncpy(szWriteBuff+iWritePos, str, sizeof(szWriteBuff)-iWritePos);
    szWriteBuff[sizeof(szWriteBuff)-1] = '\0';
    iWritePos += strlen(str);
    }
}
/**********************************************************************
* For base types this shows the identifiers
*  value. For compound types this does nothing.
**********************************************************************/
static void ShowValue(IdentifierT *ident)
{
  IdentifierT* id = NULL;
  char szBuff[64];
  
  assert(ident != NULL);
  
  if(ident->scope != GlobalScope)
    return;

  strcpy(szBuff,"");
  
  assert(ident->type != NULL);
  switch (ident->type->BasicType)
    {
    case IntegerType:
      sprintf(szBuff,"%ld",ident->value.ordinal);
      break;

    case RealType:
      sprintf(szBuff,"%lf",ident->value.real);
      break;

    case StringType:
      sprintf(szBuff,"'%s'",ident->value.string->text);
      break;

    case EnumeratedType:
      /* Search through list of members for a match */
      for(id = ident->type->data.enumeration.list; id != NULL; id = id->link)
        {
        if(id->value.ordinal == ident->value.ordinal)
          {
          strcpy(szBuff,id->name);
          break;
          }
        }
      break;

    default:
      break;
    }

  Send(szBuff);
  return;
}

/**********************************************************************
* ShowRecordType
* This displays the definition of a record type
**********************************************************************/
static void ShowRecordType(TypeT *type)
{
  IdentifierT *list = NULL;

  assert(type != NULL);

  Send("record");
  Newline();

  SpaceCount += 2;

  Spaces();
  Send("begin");
  Newline();

  list = type->data.record.FieldList;
  while (list != NULL)
    {
    Spaces();
    ShowIdent(list);
    list = list->link;
    if(list != NULL)
      Send(";");
    Newline();
    }

  Spaces();
  Send("end");
  Newline();

  SpaceCount -= 2;
  return;
}

/**********************************************************************
* Function:   FindTypeID
* Given a pointer to a type this tries to find an identifier
* that defines the type (e.g. was set up by a type definition).
* Parameters: type is a pointer to the type we are interested in.
* Returns:    a pointer to its defining identifier or NULL if not found
**********************************************************************/
static IdentifierT *FindTypeID(TypeT *type)
{
  IdentifierT *list = NULL;

  assert(type != NULL);

  for(list = LocalIdentList; list != NULL; list=list->next)
    {
    if (list->type == type && list->class == TypeIdentifier )
      {
      return list;
      }
    }

  for(list = GlobalIdentList; list != NULL; list=list->next)
    {
    if (list->type == type && list->class == TypeIdentifier)
      {
      return list;
      }
    }

    /* If not in the global list then look in the predefined id list */
  for(list = PRSI_FirstPredefinedIdent(); list != NULL; list=list->next)
    {
    if (list->type == type && list->class == TypeIdentifier)
      {
      return list;
      }
    }

  return NULL;
}
/**********************************************************************
* ShowEnumType
* This writes out the full type of an enumeration.
**********************************************************************/
static void ShowEnumType(TypeT *type)
{
  IdentifierT *list = NULL;
  int first = TRUE;

  assert(type != NULL);
  assert(type->BasicType == EnumeratedType);
  
  Send("(");
  
  /* Get list of enumeration entries. Note that this list is the
  // full list of any base type so need to prune using the lower
  // and upper bounds.
  */
  for(list = type->data.enumeration.list;
      list != NULL;
      list = list->link)
    {
    if(list->value.ordinal < type->data.enumeration.LowerBound)
      continue;
    
    if(list->value.ordinal > type->data.enumeration.UpperBound)
      break;
            
    if(!first)
      Send(",");

    Send(list->name);
    
    first = FALSE;    
    }

  Send(")");

  return;
}

/**********************************************************************
* ShowType
* This prints out the type of an identifier or whatever.
* type is the type to print out.
* IsTypeID is a flag: if it is true then type is a type ID so we
* want to print its definition.
**********************************************************************/
static void ShowType(TypeT *type, int IsTypeID)
{
  IdentifierT* typeID = NULL;

  assert(type != NULL);

  /* If not a type ID then we want to search the symbol
  // table to find an associated type name.
  */
  if(!IsTypeID)
    {
    typeID = FindTypeID(type);
    }

  /* If we have a type identifier then print out its name,
  // otherwise we have to print out the full definition.
  */
  if(typeID != NULL)  /* then just print out the name of the type */
    {
    Send(typeID->name);
    }
  else
    {
    switch (type->BasicType)
      {
      case IntegerType:
        if(type->data.integer.LowerBound == -LONG_MAX &&
           type->data.integer.UpperBound == +LONG_MAX)
          Send("integer ");
        else
          {
          char szBuff[32];
          sprintf(szBuff,"%ld",type->data.integer.LowerBound);
          Send(szBuff);
          Send("..");
          sprintf(szBuff,"%ld",type->data.integer.UpperBound);
          Send(szBuff);
          }
        break;

      case RealType:
        Send("real ");
        break;

      case StringType:
        Send("string ");
        break;

      case EnumeratedType:
        ShowEnumType(type);
        break;

      case ArrayType:
        Send("array [");
        ShowType(type->data.array.IndexType,FALSE);
        Send(" ] of ");
        ShowType(type->data.array.Type,FALSE); /* array of what ? */
        break;

      case RecordType:
        ShowRecordType(type);
        break;

      default:
        Send("UNKNOWN type ");
      }
    }
  return;
}



/**********************************************************************
* ShowFPList
* shows the formal parameter list for a function or procedure.
**********************************************************************/
static void ShowFPList(IdentifierT *list)
{
  while(list != NULL)
    {
    ShowIdent(list);
    list = list->next;
    if(list != NULL)
      Send("; ");
    }

  return;
}


/**********************************************************************
* ShowIdent
*  Displays the name, type and for base types, the value
*  of an identifier.
**********************************************************************/
static void ShowIdent(IdentifierT *ident)
{
  assert(ident != NULL);
  switch (ident->class)
    {
    case ConstantIdentifier:
      if(ident->type->BasicType == EnumeratedType)  /* member of enumeration */
        {
        }
      else
        {
        Send("const ");
        Send(ident->name);
        Send(" = ");
        ShowValue(ident);
        }
      break;

    case TypeIdentifier:
      Send("type ");
      Send(ident->name);
      Send(" = ");
      ShowType(ident->type,TRUE);
      break;

    case VariableIdentifier:
      if(ident->scope != ValueParamScope)
        Send("var ");
      Send(ident->name);
      Send(" : ");
      ShowType(ident->type,FALSE);
      break;

    case FunctionIdentifier:
    case ProcedureIdentifier:
      if(ident->flags & IDF_CCALL)
        Send("{C} ");
      if(ident->class == FunctionIdentifier)
        Send("function ");
      else
        Send("procedure ");

      Send(ident->name);
      if(ident->link != NULL)
        {
        Send("(");
        ShowFPList(ident->link);
        Send(")");
        }
      /* return type for function */
      if(ident->class == FunctionIdentifier)
        {
        Send(":");
        ShowType(ident->type,FALSE);
        }
      break;

    case FieldIdentifier:
      Send(ident->name);
      Send(" : ");
      ShowType(ident->type,FALSE);
      break;

    default:
      PRSI_printf("Unknown Identifier Class for %s\n",ident->name);
      break;
    }

  return;
}

/**********************************************************************
* PRS_WhatIs
*  Displays the type of an identifier
*  returns TRUE for success, FALSE for failure
**********************************************************************/
int PRS_WhatIs(const char* lpszIdent)
{
  IdentifierT* ident = NULL;
  
  assert(lpszIdent != NULL);
  
  ident = PRSI_FindIdentifier(lpszIdent);
  if(ident != NULL)
    {
    ShowIdent(ident);
    Send(";");
    Newline();
    }
  return(ident != NULL);
}

/**********************************************************************
* PRS_ShowIdentifiers
*  Displays either the local or global identifiers.
**********************************************************************/
void PRS_ShowIdentifiers(int ShowLocals)
{
  IdentifierT *here = NULL;

  if(ShowLocals)
    {
    for (here = LocalIdentList; here != NULL; here = here->next)
      {
      /* don't display members of enumerations */
      if(here->class == ConstantIdentifier 
        && here->type->BasicType == EnumeratedType)
        continue;
      
      ShowIdent(here);
      Send(";");
      Newline();
      }
    }
  else  /* show globals */
    {
    for (here = GlobalIdentList; here != NULL; here = here->next)
      {
      /* don't display members of enumerations */
      if(here->class == ConstantIdentifier 
        && here->type->BasicType == EnumeratedType)
        continue;

      ShowIdent(here);
      Send(";");
      Newline();
      }
    }
  return;
}
