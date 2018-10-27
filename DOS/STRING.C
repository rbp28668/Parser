#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "prsif.h"
#include "pdrive.h"

static int STR_Length(void);
static int STR_Clear(void);
static int STR_IsEmpty(void);
static int STR_Concat(void);
static int STR_Left(void);
static int STR_Mid(void);
static int STR_Right(void);
static int STR_SpanIncluding(void);
static int STR_SpanExcluding(void);
static int STR_IsIn(void);
static int STR_MakeUpper(void);
static int STR_MakeLower(void);

/* Function table: this maps C functions to their pascal declarations */
static FunctionTableT FunctionTable[] =
{
  {STR_Length,        "function str_length(str : string) : integer"},
  {STR_Clear,         "procedure str_clear(var str : string)"},
  {STR_IsEmpty,       "function str_is_empty(str : string) : boolean"},
  {STR_Concat,        "function str_concat(first,second : string) : string"},
  {STR_Left,          "function str_left(str : string; i : integer) : string"},
  {STR_Mid,           "function str_mid(str : string;"
                        " count,pos : integer) : string"},
  {STR_Right,         "function str_right(str : string; i : integer) : string"},
  {STR_SpanIncluding, "function str_span_including"
                          "( str, pattern : string) : string"},
  {STR_SpanExcluding, "function str_span_excluding"
                          "( str, pattern : string) : string"},
  {STR_IsIn,          "function str_is_in(str, pattern : string) : boolean"},
  {STR_MakeUpper,     "procedure str_upper(var str : string)"},
  {STR_MakeLower,     "procedure str_lower(var str : string)"},
  {NULL,NULL}
};

/**********************************************************************
* Function:   STR_Length
* function str_length(str : string) : integer
* This gets the length of a string
* Parameters: NONE
* Returns:    a fault code: 0 for success, non-zero for failure.
**********************************************************************/
static int STR_Length(void)
{
  PSTRING str = *(PSTRING*)PRS_Parameters(sizeof(PSTRING));
  PRS_ReturnOrdinal(PRS_StringLength(str));
  PRS_DeleteString(str);
  return 0;
}

/**********************************************************************
* Function:   STR_Clear
* procedure str_clear(var str : string)
* This clears the given string to 0 length.
* Parameters: NONE
* Returns:    a fault code: 0 for success, non-zero for failure.
**********************************************************************/
static int STR_Clear(void)
{
  PSTRING str = NULL;
  PSTRING* pStr = (PSTRING*)  PRS_Parameters(sizeof(PSTRING*));
  str = PRS_SetString(str,"");
  *pStr = str;
  return 0;
}

/**********************************************************************
* Function:   STR_IsEmpty
* function str_is_empty(str : string) : boolean
* This determines if a string is empty (zero length) or not.
* Parameters: NONE
* Returns:    a fault code: 0 for success, non-zero for failure.
**********************************************************************/
static int STR_IsEmpty(void)
{
  PSTRING* pString = PRS_Parameters(sizeof(PSTRING));
  PRS_ReturnOrdinal((PRS_StringLength(*pString)==0)?1:0);
  PRS_DeleteString(*pString);
  return 0;
}

/**********************************************************************
* Function:   STR_Concat
* function str_concat(first,second : string) : string
* This function returns a string which consists of its 2 operands
* concatenated together.
* Parameters: NONE
* Returns:    a fault code: 0 for success, non-zero for failure.
**********************************************************************/
static int STR_Concat(void)
{
  int fault = NO_FAULT;
  PSTRING str = NULL;
  PSTRING* pString = PRS_Parameters(2*sizeof(PSTRING));
  str = PRS_CreateStringOfLength(PRS_StringLength(pString[0])
                                + PRS_StringLength(pString[1]));

  if(str != NULL)
    {
    strcpy(str->text,pString[0]->text);
    strcat(str->text,pString[1]->text);
    str->len = strlen(str->text);
    }
  else
    fault = USER_FAULT;

  PRS_ReturnPtr(str);

  PRS_DeleteString(pString[1]);
  PRS_DeleteString(pString[0]);
  return fault;
}


/**********************************************************************
* Function:   STR_Left
* function str_left(str : string; i : integer) : string
* This function returns a string which is the first i characters
* of the given string.  If i is greater than the total number
* of characters in the string then the whole string is returned.
* If i is <= 0 then a zero length string is returned.
* Parameters: NONE
* Returns:    a fault code: 0 for success, non-zero for failure.
**********************************************************************/
static int STR_Left(void)
{
  typedef struct s_params
    {
    PSTRING str;
    long lLeft;
    }PARAMS;

  PARAMS* pParams = NULL;
  PSTRING str = NULL;
  int newlen = 0;
  int fault = NO_FAULT;

  pParams = PRS_Parameters(sizeof(PARAMS));

  /* new string has size which is the smaller of the existing
  // string or the value given for the "left" part.
  */
  if(pParams->str->len < pParams->lLeft)  /* then use whole string */
    newlen = pParams->str->len;
  else if (pParams->lLeft <= 0)
    newlen = 0;
  else
    newlen = (int)pParams->lLeft;

  /* create the return string */
  str = PRS_CreateStringOfLength(newlen);

  /* copy enough into it. */
  if(str)
    {
    strncpy(str->text,pParams->str->text,newlen);
    str->text[newlen] = '\0';
    str->len = newlen;
    }
  else
    fault = USER_FAULT;

  PRS_ReturnPtr(str);

  PRS_DeleteString(pParams->str);

  return fault;
}

/**********************************************************************
* Function:   STR_Mid
* function str_mid(str : string; count,pos : integer) : string
* This function returns the string which consists of the count
* characters starting at pos in the given string.
* Parameters: NONE
* Returns:    a fault code: 0 for success, non-zero for failure.
**********************************************************************/
static int STR_Mid(void)
{
  typedef struct s_params
    {
    PSTRING str;
    long lPos;
    long lCount;
    }PARAMS;

  PARAMS* pParams = NULL;
  int fault = NO_FAULT;
  PSTRING str = NULL;

  pParams = PRS_Parameters(sizeof(PARAMS));

  if(pParams->lPos < 0)
    {
    pParams->lCount += pParams->lPos;
    if(pParams->lCount < 0)
      pParams->lCount = 0;
    pParams->lPos = 0;
    }

  if(pParams->lCount + pParams->lPos > pParams->str->len)
    {
    pParams->lCount = pParams->str->len - pParams->lPos;
    if(pParams->lCount < 0)
      pParams->lCount = 0;
    }

  str = PRS_CreateStringOfLength((int)pParams->lCount);
  if(str)
    {
    strncpy(str->text
            ,pParams->str->text+(int)pParams->lPos
            ,(int)pParams->lCount);
    str->text[(int)pParams->lCount] = '\0';
    }
  else
    fault = USER_FAULT;

  PRS_ReturnPtr(str);

  PRS_DeleteString(pParams->str);

  return fault;
}

/**********************************************************************
* Function:   STR_Right
* function str_right(str : string; i : integer) : string
* This function returns a string which is the last i characters
* of the given string.  If i is greater than the total number
* of characters in the string then the whole string is returned.
* If i is <= 0 then a zero length string is returned.
* Parameters: NONE
* Returns:    a fault code: 0 for success, non-zero for failure.
**********************************************************************/
static int STR_Right(void)
{
  typedef struct s_params
    {
    PSTRING str;
    long lRight;
    }PARAMS;

  PARAMS* pParams = NULL;
  PSTRING str = NULL;
  int newlen = 0;
  int offset = 0;
  int fault = NO_FAULT;

  pParams = PRS_Parameters(sizeof(PARAMS));

  /* new string has size which is the smaller of the existing
  // string or the value given for the "right" part.
  */
  if(pParams->str->len < pParams->lRight)  /* then use whole string */
    {
    newlen = pParams->str->len;
    offset = 0;
    }
  else if (pParams->lRight <= 0)
    {
    newlen = 0;
    offset = 0;
    }
  else
    {
    newlen = (int)pParams->lRight;
    offset = pParams->str->len - (int)pParams->lRight;
    }
  /* create the return string */
  str = PRS_CreateStringOfLength(newlen);

  /* copy enough into it. */
  if(str)
    {
    strncpy(str->text,pParams->str->text+offset,newlen);
    str->len = newlen;
    }
  else
    fault = USER_FAULT;

  PRS_ReturnPtr(str);

  PRS_DeleteString(pParams->str);

  return fault;
}

/**********************************************************************
* Function:   STR_SpanIncluding
* function str_span_including( str, pattern : string) : string
* This returns a string which consists of the leftmost part of
* the string which consists of characters which are in the pattern.
* (note if the leftmost character in the string is not in the pattern
* then it returns an empty string).
* Parameters: NONE
* Returns:    a fault code: 0 for success, non-zero for failure.
**********************************************************************/
static int STR_SpanIncluding(void)
{
  typedef struct s_params
    {
    PSTRING str;
    PSTRING pattern;
    }PARAMS;

  PARAMS* pParams = NULL;
  PSTRING str = NULL;
  int nChars = 0;
  char* pstr = NULL;
  int fault = NO_FAULT;

  pParams = PRS_Parameters(sizeof(PARAMS));

  /* Count the initial run of characters in str which exist
  // in pattern.
  */
  pstr = pParams->str->text;
  nChars = 0;
  while(*pstr && strchr(pParams->pattern->text,*pstr) != NULL)
    {
    ++pstr;
    ++nChars;
    }
  assert(nChars <= pParams->str->len);

  /* Now we have the number of chars in the string, create it
  // and copy across the right number of chars.
  */
  str = PRS_CreateStringOfLength(nChars);
  if(str)
    {
    strncpy(str->text,pParams->str->text,nChars);
    str->text[nChars] = '\0';
    }
  else
    fault = USER_FAULT;

  PRS_ReturnPtr(str);

  PRS_DeleteString(pParams->str);
  PRS_DeleteString(pParams->pattern);
  return fault;
}

/**********************************************************************
* Function:   STR_SpanExcluding
* function str_span_excluding( str, pattern : string) : string
* This returns a string which consists of the leftmost part of
* the string which consists of characters which are not in the pattern.
* (note if the leftmost character in the string is in the pattern
* then it returns an empty string).
* Parameters: NONE
* Returns:    a fault code: 0 for success, non-zero for failure.
**********************************************************************/
static int STR_SpanExcluding(void)
{
  typedef struct s_params
    {
    PSTRING str;
    PSTRING pattern;
    }PARAMS;

  PARAMS* pParams = NULL;
  PSTRING str = NULL;
  int nChars = 0;
  char* pstr = NULL;
  int fault = NO_FAULT;

  pParams = PRS_Parameters(sizeof(PARAMS));

  /* Count the initial run of characters in str which do
  // not exist in pattern.
  */
  pstr = pParams->str->text;
  nChars = 0;
  while(*pstr && strchr(pParams->pattern->text,*pstr) == NULL)
    {
    ++pstr;
    ++nChars;
    }
  assert(nChars <= pParams->str->len);

  /* Now we have the number of chars in the string, create it
  // and copy across the right number of chars.
  */
  str = PRS_CreateStringOfLength(nChars);
  if(str)
    {
    strncpy(str->text,pParams->str->text,nChars);
    str->text[nChars] = '\0';
    }
  else
    fault = USER_FAULT;

  PRS_ReturnPtr(str);

  PRS_DeleteString(pParams->str);
  PRS_DeleteString(pParams->pattern);
  return fault;
}

/**********************************************************************
* Function:   STR_IsIn
* function str_is_in(str, pattern : string) : boolean
* Returns true if the pattern is in the given string.
* Parameters: NONE
* Returns:    a fault code: 0 for success, non-zero for failure.
**********************************************************************/
static int STR_IsIn(void)
{
  typedef struct s_params
    {
    PSTRING str;
    PSTRING pattern;
    }PARAMS;

  PARAMS* pParams = NULL;

  pParams = PRS_Parameters(sizeof(PARAMS));

  if(strstr(pParams->str->text,pParams->pattern->text) != NULL)
    PRS_ReturnOrdinal(1);
  else
    PRS_ReturnOrdinal(0);

  PRS_DeleteString(pParams->str);
  PRS_DeleteString(pParams->pattern);
  return 0;
}

/**********************************************************************
* Function:   STR_MakeUpper
* procedure str_upper(var str : string)
* This sets all the letters in the string to upper case
* Parameters: NONE
* Returns:    a fault code: 0 for success, non-zero for failure.
**********************************************************************/
static int STR_MakeUpper(void)
{
  PSTRING** ppString = PRS_Parameters(sizeof(PSTRING*));
  PSTRING str = **ppString;
  strupr(str->text);
  **ppString = str;
  return 0;
}

/**********************************************************************
* Function:   STR_MakeLower
* procedure str_lower(var str : string)
* This sets all the letters in the string to lower case
* Parameters: NONE
* Returns:    a fault code: 0 for success, non-zero for failure.
**********************************************************************/
static int STR_MakeLower(void)
{
  PSTRING** ppString = PRS_Parameters(sizeof(PSTRING*));
  PSTRING str = **ppString;
  strlwr(str->text);
  **ppString = str;
  return 0;
}

/**********************************************************************
* Function:   SetupStringExtensions
* This adds string extensions to the parser.
* Parameters: NONE
* Returns:    NOTHING
**********************************************************************/
void SetupStringExtensions(void)
{
  SetupCFunctions(FunctionTable);
}
