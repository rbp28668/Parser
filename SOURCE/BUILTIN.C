/*
// File:        BUILTIN.C
//
// Prefix:
//
// Description: Built-in functions.
//
*/
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "parser.h"
#include "prsif.h"
#include "builtin.h"
#include "ident.h"
#include "error.h"

#define WRITE_BUFF_LEN 256

static char szWriteBuffer[WRITE_BUFF_LEN] = {'\0'};
static PFCSV lpfnUserOutput = NULL;

/* Type declarations for predefined identifiers */
TypeT IntegerTypeDecl =  {IntegerType,INT_LEN,{{0}}};
TypeT BooleanTypeDecl =  {EnumeratedType,ENUM_LEN,{{0}}};
TypeT RealTypeDecl =     {RealType,REAL_LEN,{{0}}};
TypeT StringTypeDecl =   {StringType,STRING_LEN,{{0}}};

/* Built-in 'C' functions. */
static int PRSI_abs(void);
static int PRSI_arctan(void);
static int PRSI_chr(void);
static int PRSI_cos(void);
static int PRSI_exp(void);
static int PRSI_ln(void);
static int PRSI_odd(void);
static int PRSI_ord(void);
static int PRSI_page(void);
static int PRSI_pred(void);
static int PRSI_round(void);
static int PRSI_sin(void);
static int PRSI_sqr(void);
static int PRSI_sqrt(void);
static int PRSI_succ(void);
static int PRSI_trunc(void);
static int PRSI_write(void);
static int PRSI_writeln(void);

/* "Special" type declarations for built-in functions */
static TypeT NumberTypeDecl = {IntegerType|RealType,INT_LEN,{{0}}};
static TypeT OrdinalTypeDecl = {IntegerType|EnumeratedType,INT_LEN,{{0}}};
static TypeT OrdTypeDecl = {IntegerType|EnumeratedType|StringType,INT_LEN,{{0}}};
static TypeT SimpleTypeDecl = {IntegerType|EnumeratedType|RealType|StringType,INT_LEN,{{0}}};

/* Predefined identifiers for use in builtin fn fp lists */
static IdentifierT idArgNumber = {"val",NULL,NULL,VariableIdentifier,ValueParamScope,&NumberTypeDecl,IDF_PUSH_TYPE};
static IdentifierT idArgReal = {"rVal",NULL,NULL,VariableIdentifier,ValueParamScope,&RealTypeDecl,0};
static IdentifierT idArgInt = {"iVal",NULL,NULL,VariableIdentifier,ValueParamScope,&IntegerTypeDecl,0};
static IdentifierT idArgOrdinal = {"oVal",NULL,NULL,VariableIdentifier,ValueParamScope,&OrdinalTypeDecl,0};
static IdentifierT idArgOrd = {"oVal",NULL,NULL,VariableIdentifier,ValueParamScope,&OrdTypeDecl,IDF_PUSH_TYPE};
static IdentifierT idArgSimple = {"sVal",NULL,NULL,VariableIdentifier,ValueParamScope,&SimpleTypeDecl,IDF_PUSH_TYPE};

/* These are Pascal's standard identifiers. */

static IdentifierT pdi_writeln = {"writeln",NULL,        &idArgSimple,ProcedureIdentifier,GlobalScope,NULL,IDF_CCALL|IDF_VARARGS};
static IdentifierT pdi_write =   {"write",  &pdi_writeln,&idArgSimple,ProcedureIdentifier,GlobalScope,NULL,IDF_CCALL|IDF_VARARGS};
static IdentifierT pdi_unpack =  {"unpack", &pdi_write,  NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_trunc =   {"trunc",  &pdi_unpack, &idArgReal,FunctionIdentifier,GlobalScope,&IntegerTypeDecl,IDF_CCALL};
static IdentifierT pdi_true =    {"true",   &pdi_trunc,  NULL,ConstantIdentifier,GlobalScope,&BooleanTypeDecl};
static IdentifierT pdi_text =    {"text",   &pdi_true,   NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_succ =    {"succ",   &pdi_text,   &idArgOrdinal,FunctionIdentifier,GlobalScope,&NumberTypeDecl,IDF_CCALL};
static IdentifierT pdi_string =  {"string", &pdi_succ,   NULL,TypeIdentifier,GlobalScope,&StringTypeDecl};
static IdentifierT pdi_sqrt =    {"sqrt",   &pdi_string, &idArgReal,FunctionIdentifier,GlobalScope,&RealTypeDecl,IDF_CCALL};
static IdentifierT pdi_sqr =     {"sqr",    &pdi_sqrt,   &idArgNumber,FunctionIdentifier,GlobalScope,&NumberTypeDecl,IDF_CCALL};
static IdentifierT pdi_sin =     {"sin",    &pdi_sqr,    &idArgReal,FunctionIdentifier,GlobalScope,&RealTypeDecl,IDF_CCALL};
static IdentifierT pdi_round =   {"round",  &pdi_sin,    &idArgReal,FunctionIdentifier,GlobalScope,&IntegerTypeDecl,IDF_CCALL};
static IdentifierT pdi_rewrite = {"rewrite",&pdi_round,  NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_reset =   {"reset",  &pdi_rewrite,NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_real =    {"real",   &pdi_reset,  NULL,TypeIdentifier,GlobalScope,&RealTypeDecl};
static IdentifierT pdi_readln =  {"readln", &pdi_real,   NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_read =    {"read",   &pdi_readln, NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_put =     {"put",    &pdi_read,   NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_pred =    {"pred",   &pdi_put,    &idArgOrdinal,FunctionIdentifier,GlobalScope,&OrdinalTypeDecl,IDF_CCALL};
static IdentifierT pdi_page =    {"page",   &pdi_pred,   NULL,ProcedureIdentifier,GlobalScope,NULL,IDF_CCALL};
static IdentifierT pdi_pack =    {"pack",   &pdi_page,   NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_output =  {"output", &pdi_pack,   NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_ord =     {"ord",    &pdi_output, &idArgOrd,FunctionIdentifier,GlobalScope,&IntegerTypeDecl,IDF_CCALL};
static IdentifierT pdi_odd =     {"odd",    &pdi_ord,    &idArgInt,FunctionIdentifier,GlobalScope,&BooleanTypeDecl,IDF_CCALL};
static IdentifierT pdi_new =     {"new",    &pdi_odd,    NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_maxint =  {"maxint", &pdi_new,    NULL,ConstantIdentifier,GlobalScope,&IntegerTypeDecl};
static IdentifierT pdi_ln =      {"ln",     &pdi_maxint, &idArgReal,FunctionIdentifier,GlobalScope,&RealTypeDecl,IDF_CCALL};
static IdentifierT pdi_integer = {"integer",&pdi_ln,     NULL,TypeIdentifier,GlobalScope,&IntegerTypeDecl};
static IdentifierT pdi_input =   {"input",  &pdi_integer,NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_get =     {"get",    &pdi_input,  NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_false =   {"false",  &pdi_get,    NULL,ConstantIdentifier,GlobalScope,&BooleanTypeDecl};
static IdentifierT pdi_exp =     {"exp",    &pdi_false,  &idArgReal,FunctionIdentifier,GlobalScope,&RealTypeDecl,IDF_CCALL};
static IdentifierT pdi_eoln =    {"eoln",   &pdi_exp,    NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_eof =     {"eof",    &pdi_eoln,   NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_dispose = {"dispose",&pdi_eof,    NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_cos =     {"cos",    &pdi_dispose,&idArgReal,FunctionIdentifier,GlobalScope,&RealTypeDecl,IDF_CCALL};
static IdentifierT pdi_chr =     {"chr",    &pdi_cos,    &idArgInt,FunctionIdentifier,GlobalScope,&StringTypeDecl,IDF_CCALL};
static IdentifierT pdi_char =    {"char",   &pdi_chr,    NULL,UndefinedIdentifier,GlobalScope,NULL};
static IdentifierT pdi_boolean = {"boolean",&pdi_char,   NULL,TypeIdentifier,GlobalScope,&BooleanTypeDecl};
static IdentifierT pdi_arctan =  {"arctan", &pdi_boolean,&idArgReal,FunctionIdentifier,GlobalScope,&RealTypeDecl,IDF_CCALL};
static IdentifierT pdi_abs =     {"abs",    &pdi_arctan, &idArgNumber,FunctionIdentifier,GlobalScope,&NumberTypeDecl,IDF_CCALL};


/**********************************************************************
* Function:   PRSI_SetupPredefinedIdentifiers
* This sets up any code needed for the predefined identifiers.
* Parameters: NONE
*
* Returns:    NOTHING
**********************************************************************/
void PRSI_SetupPredefinedIdentifiers(void)
{

  /*=========================*/
  /* Setup the basic types   */
  /*=========================*/

  IntegerTypeDecl.data.integer.LowerBound = -LONG_MAX;
  IntegerTypeDecl.data.integer.UpperBound = LONG_MAX;

  BooleanTypeDecl.data.enumeration.LowerBound = FALSE;
  BooleanTypeDecl.data.enumeration.UpperBound = TRUE;
  BooleanTypeDecl.data.enumeration.list = &pdi_false;
  BooleanTypeDecl.data.enumeration.count = 2;
  BooleanTypeDecl.data.enumeration.BaseType = &BooleanTypeDecl;
  /* RealTypeDecl is ok as it stands */
  /* StringTypeDecl is ok as it stands */

  /*=========================*/
  /* Set up the identifiers. */
  /*=========================*/

  /* Maxint */
  pdi_maxint.value.ordinal = LONG_MAX;

  /* Boolean Types */
  pdi_false.link = &pdi_true;
  pdi_false.value.ordinal = FALSE;
  pdi_true.value.ordinal = TRUE;
  pdi_boolean.link = &pdi_false;

  pdi_writeln.value.pfvi = PRSI_writeln;
  pdi_write.value.pfvi = PRSI_write;
  pdi_trunc.value.pfvi = PRSI_trunc;  
  pdi_succ.value.pfvi = PRSI_succ;    
  pdi_sqrt.value.pfvi = PRSI_sqrt;    
  pdi_sqr.value.pfvi = PRSI_sqr;     
  pdi_sin.value.pfvi = PRSI_sin;     
  pdi_round.value.pfvi = PRSI_round;   
  pdi_pred.value.pfvi = PRSI_pred;   
  pdi_page.value.pfvi = PRSI_page;    
  pdi_ord.value.pfvi = PRSI_ord;    
  pdi_odd.value.pfvi = PRSI_odd;    
  pdi_ln.value.pfvi = PRSI_ln;     
  pdi_exp.value.pfvi = PRSI_exp;     
  pdi_cos.value.pfvi = PRSI_cos;    
  pdi_chr.value.pfvi = PRSI_chr;     
  pdi_arctan.value.pfvi = PRSI_arctan;  
  pdi_abs.value.pfvi = PRSI_abs;    
}

/**********************************************************************
* Function:   PRSI_FirstPredefinedIdent
* Gets a pointer to the first of the predefined identifiers.
* Parameters: NONE
*
* Returns:    A pointer to the first predefined identifier.
**********************************************************************/
IdentifierT* PRSI_FirstPredefinedIdent(void)
{
  return(&pdi_abs);
}

/**********************************************************************
* Function:   PRSI_abs
*
* Returns the absolute value of an integer or a real. If parameter
* is integer it returns integer, if real it returns real
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_abs(void)
{
  struct sParam
    {
    union
      {
      long lVal;
      float fVal;
      }val;
    TypeT* type;
    };

  struct sParam* params;
  int err = 0;

  params = (struct sParam*)PRS_Parameters(sizeof(struct sParam));
  assert(params != NULL);
  assert(params->type != NULL);
  if(params->type->BasicType == IntegerType)
    {
      err = PRS_ReturnOrdinal((params->val.lVal < 0l)
        ? (0l - params->val.lVal)
        : params->val.lVal);
    }
  else if(params->type->BasicType == RealType)
    {
      err = PRS_ReturnReal((params->val.fVal < 0.0f)
        ? (0.0f - params->val.fVal)
        : params->val.fVal);
    }
  else
    err = IDS_ERR_BUILTIN_ILLEGAL_TYPE_FOR_ABS;

  return err;
}

/**********************************************************************
* Function:   PRSI_arctan
*
* Gives the angle in radians in the range 0 to pi whose tangent is
* the argument.  Takes real, returns real
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_arctan(void)
{

  float fVal = 0.0f;

  fVal = *(float *)PRS_Parameters(sizeof(float));
  fVal = (float)atan(fVal);  /* returns value in range -PI/2 to PI/2 */
  if(fVal < 0.0f)
    fVal += 3.1415927f;
  return PRS_ReturnReal(fVal);
}

/**********************************************************************
* Function:   PRSI_chr
*
* Converts an integer to a single string length 1.
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_chr(void)
{
  long lVal=0;
  char str[2];

  lVal = *(long*)PRS_Parameters(sizeof(long));
  str[0] = (char)lVal;
  str[1] = '\0';

  return PRS_ReturnPtr(PRS_CreateString(str));
}

/**********************************************************************
* Function:   PRSI_cos
*
* Gets the cosine of an angle in radians. Takes real, returns real
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_cos(void)
{
  float fVal = 0.0f;

  fVal = *(float *)PRS_Parameters(sizeof(float));
  fVal = (float)cos(fVal);
  return PRS_ReturnReal(fVal);
}

/**********************************************************************
* Function:   PRSI_exp
*
* Gets the exponential of its argument: takes real, returns real
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_exp(void)
{
  float fVal = 0.0f;

  fVal = *(float *)PRS_Parameters(sizeof(float));
  fVal = (float)exp(fVal);
  return PRS_ReturnReal(fVal);
}


/**********************************************************************
* Function:   PRSI_ln
*
* Takes the natural log of its argument: takes real, returns real.
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_ln(void)
{
  float fVal = 0.0f;
  int err = 0;

  fVal = *(float *)PRS_Parameters(sizeof(float));
  if(fVal == 0.0f)
    err = IDS_ERR_BUILTIN_LOG_ZERO;
  else
    fVal = (float)log(fVal);
  PRS_ReturnReal(fVal);

  return err;
}

/**********************************************************************
* Function:   PRSI_odd
*
* returns the boolean TRUE if the integer argument is odd, FALSE
* otherwise: Takes integer, returns boolean.
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_odd(void)
{
  long lVal = *(long*)PRS_Parameters(sizeof(long));
  return PRS_ReturnOrdinal(lVal&1);
}

/**********************************************************************
* Function:   PRSI_ord
*
* gets the ordinal value of an ordinal type or string
* Takes ordinal type or string, return integer.
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_ord(void)
{
  struct sParam
    {
    union
      {
      StringT *str;
      long ord;
      }data;
    TypeT* type;
    };
  struct sParam* params;
  long lVal = 0l;
  int err = 0;

  params = (struct sParam *)PRS_Parameters(sizeof(struct sParam));

  assert(params != NULL);
  assert(params->type != NULL);
  if(params->type->BasicType == StringType)
    {
    if(params->data.str->len > 0)
      lVal = (long)params->data.str->text[0];
    else
      err = IDS_ERR_BUILTIN_ORD_OF_EMPTY_STRING;
    PRS_DeleteString(params->data.str);
    }
  else if(PRSI_IsOrdinalType(params->type))
    lVal = params->data.ord;
  else
    err = IDS_ERR_BUILTIN_ORD_NOT_ORDINAL_OR_STRING;

  PRS_ReturnOrdinal(lVal);
  return err;

}

/**********************************************************************
* Function:   PRSI_page
*
* This terminates a block of output. In the event that write/writeln
* have been building up data in an output buffer this will flush
* the buffer to the output device.
*
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_page(void)
{
  if(lpfnUserOutput)
    (*lpfnUserOutput)(NULL);
  return 0;
}

/**********************************************************************
* Function:   PRSI_pred
*
* Gets the predecessor of an ordinal variable. Takes ordinal, returns
* ordinal.
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_pred(void)
{
  long lVal = *(long*)PRS_Parameters(sizeof(long));
  return PRS_ReturnOrdinal(lVal-1);
}

/**********************************************************************
* Function:   PRSI_round
*
* Converts a real to an integer by rounding.
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_round(void)
{
  float fVal = 0.0f;
  long lVal = 0l;

  fVal = *(float*)PRS_Parameters(sizeof(float));
  if(fVal < 0)
    lVal = (long)(fVal-0.5f);
  else
    lVal = (long)(fVal+0.5f);
  return PRS_ReturnOrdinal(lVal);
}

/**********************************************************************
* Function:   PRSI_sin
*
* Gets the sine of an angle in radians: takes a real, returns a real.
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_sin(void)
{
  float fVal = 0.0f;

  fVal = *(float *)PRS_Parameters(sizeof(float));
  fVal = (float)sin(fVal);
  return PRS_ReturnReal(fVal);
}


/**********************************************************************
* Function:   PRSI_sqr
*
* Gets the square of an integer or real. Takes an integer or real,
* returns same type
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_sqr(void)
{
  struct sParam
    {
    union
      {
      long lVal;
      float fVal;
      }val;
    TypeT* type;
    };

  struct sParam* params;
  int err = 0;

  params = (struct sParam*)PRS_Parameters(sizeof(struct sParam));
  assert(params != NULL);
  assert(params->type != NULL);
  if(params->type->BasicType == IntegerType)
    {
      PRS_ReturnOrdinal(params->val.lVal * params->val.lVal);
    }
  else if(params->type->BasicType == RealType)
    {
      PRS_ReturnReal(params->val.fVal * params->val.fVal);
    }
  else
    err = IDS_ERR_BUILTIN_ILLEGAL_TYPE_FOR_SQR;

  return err;


}


/**********************************************************************
* Function:   PRSI_sqrt
*
* returns the square root of a non-negative real number. Takes a
* real and returns a real.
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_sqrt(void)
{
  float fVal = 0.0f;
  int err = 0;

  fVal = *(float *)PRS_Parameters(sizeof(float));
  if(fVal >= 0.0f)
    fVal = (float)sqrt(fVal);
  else
    err = IDS_ERR_BUILTIN_SQRT_NEGATIVE;

  PRS_ReturnReal(fVal);

  return err;

}


/**********************************************************************
* Function:   PRSI_succ
*
* Gets the successor of an ordinal type. Takes an ordinal, returns an
* ordinal.
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_succ(void)
{
  long lVal = *(long*)PRS_Parameters(sizeof(long));
  return PRS_ReturnOrdinal(lVal+1);
}

/**********************************************************************
* Function:   PRSI_trunc
*
* Truncates a real to an integer towards zero. Takes a real, returns
* an integer.
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_trunc(void)
{
  float fVal = *(float*)PRS_Parameters(sizeof(float));
  return PRS_ReturnOrdinal((long)fVal);
}


/**********************************************************************
* Function:   PRSI_write
*
* Writes a single value to the output stream.
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_write(void)
{

  struct sParam
    {
    union
      {
      long lVal;
      float fVal;
      StringT* str;
      }val;
    TypeT* type;
    };

  struct sParam* params;
  IdentifierT* id = NULL;
  int err = 0;
  int len = 0;
  char szBuff[WRITE_BUFF_LEN];
  long lCount = 0;

  /* Pull off the argument count */
  lCount = *(long*)PRS_Parameters(sizeof(long));

  /* Loop, pulling off & processing the arguments 1 by 1 */
  while(lCount-- > 0)
    {
    params = (struct sParam*)PRS_Parameters(sizeof(struct sParam));

    assert(params != NULL);
    assert(params->type != NULL);
    switch (params->type->BasicType)
      {
      case IntegerType:
        len = sprintf(szBuff,"%ld",params->val.lVal);
        break;

      case RealType:
        len = sprintf(szBuff,"%g",params->val.fVal);
        break;

      case StringType:
        len = sprintf(szBuff,"%s",params->val.str->text);
        PRS_DeleteString(params->val.str);
        break;

      case EnumeratedType:
        /* Search through list of members for a match */
        for(id = params->type->data.enumeration.list; id != NULL; id = id->link)
          {
          if(id->value.ordinal == params->val.lVal)
            {
            len = sprintf(szBuff,"%s",id->name);
            break;
            }
          }
        break;

      default:
        err = IDS_ERR_BUILTIN_INVALID_WRITE_TYPE;
        break;

      }

    if(err == 0)
      {
      if(strlen(szWriteBuffer) + len >= WRITE_BUFF_LEN-1)
        err = IDS_ERR_BUILTIN_WRITE_BUFFER_OVERFLOW;
      else
        {
        strcat(szWriteBuffer,szBuff);
        }
      }
    }/* endwhile */
  return err;

}


/**********************************************************************
* Function:   PRSI_writeln
*
* Writes a single value to the output stream followed by a CR/LF.
* Parameters: NONE
*
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
static int PRSI_writeln(void)
{
  int err = PRSI_write();
  if(lpfnUserOutput != NULL)
    (*lpfnUserOutput)(szWriteBuffer);

  memset(szWriteBuffer,0,sizeof(szWriteBuffer));

  return err;
}



/**********************************************************************
* Function:   PRS_SetOutputFunction
*
* This sets up the function which will be used by write, writeln and
* page to output data.
* Parameters: lpfnNewFn is the new output function: it is a pointer
*             to a function taking a const string and returning void.
*
* Returns:    The old output function.
**********************************************************************/
PFCSV PRS_SetOutputFunction(PFCSV lpfnNewFn)
{
  PFCSV lpfnOldfn = lpfnUserOutput;
  lpfnUserOutput = lpfnNewFn;
  return lpfnOldfn;
}


/**********************************************************************
* Function:   PRS_Write
* Writes a string to the output stream
* Parameters: lpsz is a pointer to a null-terminated string
*
* Returns:    NOTHING
**********************************************************************/
void PRS_Write(char* lpsz)
{
  if(lpfnUserOutput != NULL)
    (*lpfnUserOutput)(lpsz);
  return;
}





