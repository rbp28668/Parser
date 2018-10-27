/* 
// File:        PARSER.H
// 
// Prefix:
// 
// Description: Main parser definitions
//
*/

#ifndef _PARSER_H
#define _PARSER_H

#include "environ.h"

#include "pstring.h"

/* Size of fundamental types in bytes
// note that enum and int should be the same lengths
// so they can be treated as ordinal.
*/
#define ENUM_LEN 4      /* stored as long */
#define INT_LEN 4       /* stored as long */
#define REAL_LEN 4      /* stored as float */
#define STRING_LEN 4    /* stored as PStringT* */


/* Basic classes of identifer i.e. variable, function etc */
typedef enum e_IdentifierClass
{
  ConstantIdentifier,    /* Constant (numeric, enumerator, char or string ) */
  TypeIdentifier,        /* a type definition */
  VariableIdentifier,    /* general variable (be it global,local or what)*/
  FunctionIdentifier,    /* a function */
  ProcedureIdentifier,   /* a procedure */
  FieldIdentifier,       /* of a record */
  UndefinedIdentifier    /* undefined identifiers(yet) */ 
}IdentifierClassT;

/* Scope of a variable */
typedef enum e_IdentifierScope
{
  GlobalScope,
  LocalScope,
  VarParamScope,
  ValueParamScope,
  RecordScope
}IdentifierScopeT;

/* Basic types of data (be they variables, parameters
*  constants or whatever) These are set up as bit masks so
*  that the (polymorphic) built in types can take and retun
*  multiples types.
*/
typedef enum e_BasicType
{
  IntegerType =   0x0001,
  RealType =      0x0002,
  StringType =    0x0004,
  EnumeratedType= 0x0008,
  ArrayType =     0x0010,
  RecordType =    0x0020
}BasicTypeT;


typedef struct s_identifier IdentifierT;

typedef struct s_type
{
  BasicTypeT BasicType;
  int size;                      /* in bytes */
  union
    {
    struct s_enum
      {
      long LowerBound;           /* for sub-range */
      long UpperBound;           /* ditto */
      IdentifierT *list;         /* list of members of this type */
      int count;                 /* count of members in list */
      struct s_type* BaseType;   /* base type of sub-range */
      }enumeration;
    struct s_integer
      {
      long LowerBound;           /* for sub-range */
      long UpperBound;           /* ditto */
      }integer;
    struct s_array
      {
      struct s_type *Type;       /* array of what ? */
      struct s_type *IndexType;  /* indexed by this */
      }array;
    struct s_record
      {
      IdentifierT *FieldList;
      }record;
    }data;
}TypeT;

/* Flags for identifiers */
enum e_flags 
  {
  IDF_CCALL = 1,      /* Set if function or proc is 'C' call */
  IDF_PUSH_TYPE = 2,  /* Set for arg of polymorphic fn or proc: push type
                      // of arg before arg itself.
                      */
  IDF_VARARGS = 4     /* Set if proc or fn takes variable number of args*/
  };

/* Data for holding an identifer */
struct s_identifier
{
  char *name;                    /* The identifier name */
  struct s_identifier *next;     /* linked list of identifiers */
  struct s_identifier *link;     /* link field for parameter list & enum */
  IdentifierClassT class;        /* class of identifier (e.g. variable, function */
  IdentifierScopeT scope;        /* scope of identifier (e.g. local, global etc */
  TypeT *type;                   /* type of this variable, constant etc */
  int flags;                     /* general flags for ident */
  union
    {
    long ordinal;        /* for ordinal types */
    int FPOffset;        /* for parameters & local variables */
    void *addr;          /* for records & arrays */
    int(*pfvi)(void);    /* for CCALL functions */
    float real;          /* for reals */
    StringT *string;     /* for string */
    long offset;         /* for record fields */
    }value;
};

/*
A member of an enumerated type is a ConstantIdentifier with BasicType
of EnumeratedType. Its type field points to the same type has
the enumeration's name.
*/



/* CONSTANT.C */
extern int PRSI_Constant(IdentifierT *ident);
extern int PRSI_ConstantDefinitionPart(void);

/* SIMPSTAT.C */
extern int PRSI_SimpleStatement(void);

/* TYPE.C */
extern TypeT *PRSI_Type(void);
extern int PRSI_TypeDefinitionPart(void);

extern int PRSI_IsOrdinalType(TypeT* type);
extern int PRSI_GetOrdinalBounds(TypeT* type, long* lower, long* upper);

/* VARS.C */
extern int PRSI_VariableDeclarationPart(void);



#endif




/* 
Example declarations:

First example: an integer variable.  Note that Integers, boolean, real,
string and char are similar.

var test : integer;

IDENTIFIER:
name "test",
next --> ???,
link NULL
class VariableIdentifier
type -------------------------->TYPE:
value.integer                   BasicType IntegerType


second example: an array of real from 0 to 10.

var list : array [0..10] of real;

IDENTIFIER:
name "list"
next --> ???
link NULL
class VariableIdentifer
type --------------------------->TYPE:
value.addr -----+                 BasicType ArrayType
                |                 size 44
                |                 Type ----------------->TYPE:
                |                 LowerBound 0            BasicType RealType
                |                 UpperBound 10          Size 4
                |
                +---->44 bytes of store


Third example: an enumerated type

type suite = (Hearts,Spades,Clubs,Diamonds);

Note that there is a link from the type of the enumeration back to
the parent. This is not shown below.
IDENTIFER:
name "suite"
next --> ???
link NULL
class TypeIdentifier
type ------------------->TYPE:
addr NULL    ^           BasicType EnumeratedType
             |           size 4
             |           data.enumerated.list  ------------+
             |           count 4                           |
             |                                             |
             |         +-----------------------------------+
             |         |
             |         +-->IDENTIFIER:
             |             name "Hearts"
             |             next --> ???
             |             link ---------------------------+
             |             class ConstantIdentifier        |
             +-------------type                            |
             |             value.enumeration 0             |
             |                                             |
             |         +-----------------------------------+
             |         |
             |         +-->IDENTIFIER:
             |         name "Spades"
             |         next --> ???
             |         link ---------------------------+
             |         class ConstantIdentifier        |
             +---------type                            |
             |         value.enumeration 1             |
             |                                         |
             |     +-----------------------------------+
             |     |
             |     +-->IDENTIFIER:
             |         name "Clubs"
             |         next --> ???
             |         link ---------------------------+
             |         class ConstantIdentifier        |
             +---------type                            |
             |         value.enumeration 2             |
             |                                         |
             |     +-----------------------------------+
             |     |
             |     +-->IDENTIFIER:
             |         name "Diamonds"
             |         next --> ???
             |         link NULL
             |         class ConstantIdentifier
             +---------type 
                       value.enumeration 3
              
              
Example four: a record

type point : record x,y : integer end

IDENTIFIER:
name "point"
next --> ???
link 
class TypeIdentifier 
type --------------->TYPE:
value undefined       BasicType RecordType
                     size 8
                     data.record.FieldList ---+
                                              |
           +----------------------------------+
           |
           +-->IDENTIFIER:
               name "x"
               next --> ???
               link ---------------------------+
               class FieldIdentifier           |
               type --> TYPE: IntegerType,4    |
               value.offset 0                  |
                                               |
                                               |
           +-----------------------------------+
           |
           +-->IDENTIFIER:
               name "y"
               next --> ???
               link NULL
               class FieldIdentifier
               type --> TYPE: IntegerType,4
               value.offset 4


Example five: a function declaration

The formal parameter list is pointed to by the link field of the
function or procedure identifier.  The list itself is linked 
using the next fields.

function max(var first:integer; second : integer) integer; 
Note that the function return type is specified in the function 
identifier. For a procedure this will be NULL.


IDENTIFIER:
name "max"
next --> ???
link ---------------------------+
class FunctionIdentifier        |
type --> TYPE: IntegerType,4    |
addr -----> function code       |
                                |
                                |
                                |
                     +----------+
                     |
                     +-->IDENTIFIER:
                     name "first"
                     next  --------------------------+
                     link --> NULL                   |
                     class VarIdentifier             |
                     type --> TYPE: IntegerType,4    |
                     value.offset 0                  |
                                                     |
                                                     |
                 +-----------------------------------+
                 |
                 +-->IDENTIFIER:
                 name "second"
                 next --> NULL
                 link --> NULL
                 class ValueIdentifier
                 type --> TYPE: IntegerType,4
                 value.offset 1
*/
