/*
// File:          LEX.C
// 
// Prefix:        LEX_
// 
// Description:   Lexical analyser
//
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "prsif.h"
#include "lex.h"
#include "tokens.h"
#include "malloc.h"
#include "parser.h"
#include "plant.h"

#define BUFFLEN 255


/* Values which hold the default separators and escape characters */
static char LexStringDelim = '\'';
static char LexStringEscape = '\\';
static char LexCharDelim = '\'';
static char *LexCommentStart = "{";
static char *LexCommentEnd = "}";

/* Value to hold pushed back tokens */
static int PushedTokens[8];
static int PushedCount = 0;

/* Get more input function (same as fgets for parameters) */
static PFSIS LexReadBuffer = NULL;

/* Keyword list and its count */
static TOKMATCH *KeyWords = NULL;
static int KeywordCount = 0;

/* punctuation list and its count */
static TOKMATCH *Punctuation = NULL;
static int PunctuationCount = 0;

/* Input buffer and current position */
static char buff[BUFFLEN] = "";
static char *pos = buff;

/* Binary input flag and input file */
static int blBinary = FALSE;
static FILE* fpInput = NULL;

/* Store for tokens with value i.e. strings, numbers and identifiers */
static char LexString[BUFFLEN];
static double LexDouble;
static long LexLong;
static char LexChar;
static char LexIdentifier[BUFFLEN];

static TOKMATCH keywords[] =
{
  {"div",         TKDiv},
  {"mod",         TKMod},
  {"nil",         TKNil},
  {"in",          TKIn},
  {"or",          TKOr},
  {"and",         TKAnd},
  {"not",         TKNot},
  {"if",          TKIf},
  {"then",        TKThen},
  {"else",        TKElse},
  {"case",        TKCase},
  {"of",          TKOf},
  {"repeat",      TKRepeat},
  {"until",       TKUntil},
  {"while",       TKWhile},
  {"do",          TKDo},
  {"for",         TKFor},
  {"to",          TKTo},
  {"downto",      TKDownto},
  {"begin",       TKBegin},
  {"end",         TKEnd},
  {"with",        TKWith},
  {"goto",        TKGoto},
  {"const",       TKConst} ,
  {"var",         TKVar},
  {"type",        TKType},
  {"array",       TKArray},
  {"record",      TKRecord},
  {"set",         TKSet},
  {"file",        TKFile},
  {"function",    TKFunction},
  {"procedure",   TKProcedure},
  {"label",       TKLabel},
  {"packed",      TKPacked},
  {"program",     TKProgram},
  {NULL,-1}
};

/* List of punctuation for the parser */

static TOKMATCH punctuation[] =
{
  {"+",   TPPlus},
  {"-",   TPMinus},
  {"*",   TPTimes},
  {"/",   TPDivide},
  {"=",   TPEquals},
  {"<>",  TPNotEquals},
  {"<",   TPLessThan},
  {">",   TPGreaterThan},
  {"<=",  TPLessEquals},
  {">=",  TPGreaterEquals},
  {"(",   TPLeftParen},
  {")",   TPRightParen},
  {"[",   TPLeftBracket},
  {"]",   TPRightBracket},
  {":=",  TPAssignment},
  {".",   TPPeriod},
  {",",   TPComma},
  {":",   TPColon},
  {";",   TPSemicolon},
  {"..",  TPDotDot},
  {"^",   TPPointer},
  {"'",   TPQuote},
  {NULL,  -1}
};




/***********************************************************
* SkipSpaces
* Skips over any leading spaces in the input buffer
***********************************************************/
static void SkipSpaces(void)
{
  assert(pos != NULL);
  while (*pos && isspace(*pos))
    ++pos;
}

/***********************************************************
* isblank
* Returns true if a string only contains whitespace
***********************************************************/
static int isblank(char *pos)
{
  char *here = NULL;
  int blank = 1;
  
  assert(pos != NULL);
  
  for (here=pos; *here; ++here)
    {
    if(!isspace(*here))
      {
      blank = 0;
      break;
      }
    }
  return blank;
}

/***********************************************************
* isident
* Returns true only if a character is valid in an identifier
***********************************************************/
static int isident(char ch)
{
  return isalnum(ch)||(ch == '_');
}


/***********************************************************
* FillBuffer
* If the input buffer is empty or only contain whitespace
* then this tries to read from the input stream until end
* of file or it finds a line without only whitespace.
***********************************************************/
static int FillBuffer(void)
{
  char *str = NULL;
  
  assert(pos != NULL);
  
  SkipSpaces();
  while (isblank(pos) || strstr(pos,LexCommentStart)==pos)
    {
    while(isblank(pos))
      {
      /* If the input file pointer is non-null then read from
      // the input. If null then look for a non-null LexReadBuffer
      // function.  If this is null as well then return EOF
      */
      str = NULL;
      if(fpInput != NULL)
        str = fgets(buff,sizeof(buff),fpInput);
      else if(LexReadBuffer != NULL)
        str = (*LexReadBuffer)(buff,sizeof(buff));
      if(str == NULL)
        return(EOF);

      pos = buff;
      }
    SkipSpaces();
    if(strstr(pos,LexCommentStart) == pos)
      {
      while (strstr(pos,LexCommentEnd) == NULL)
        {
        str = NULL;
        if(fpInput != NULL)
          str = fgets(buff,sizeof(buff),fpInput);
        else if(LexReadBuffer != NULL)
          str = (*LexReadBuffer)(buff,sizeof(buff));
        if(str == NULL)
          return(EOF);
        pos = buff;
        }
      pos = strstr(pos,LexCommentEnd) + strlen(LexCommentEnd);
      SkipSpaces();
      }
    }
  return(0);
}


/***********************************************************
* EscapedChar
* Converts an escape sequence into a single character
***********************************************************/
static char EscapedChar(void)
{
  char ch;
  
  assert(*pos == LexStringEscape);

  ++pos;

  if(*pos == 'n')
    {
    ch = '\n'; ++pos;
    }
  else if (*pos == 'r')
    {
    ch = '\r'; ++pos;
    }
  else if (*pos == 'a')
    {
    ch = '\a'; ++pos;
    }
  else if (isdigit(*pos))
    {
    ch = (char)strtoul(pos,&pos,0);
    }
  else
    {
    ch = *pos++;
    }

  return ch;

}


/***********************************************************
* ParseString
* Parses a string in the input buffer
***********************************************************/
static int ParseString(void)
{
  char *dest = NULL;
  int count;

  assert(*pos == LexStringDelim);

  dest = LexString;
  ++pos;      /* skip over leading delim */

  count = 0;
  while (*pos != LexStringDelim)
    {
    if(*pos == '\0')
      return TokErrNewlineInString;

    if(*pos == LexStringEscape)
      *dest++ = EscapedChar();
    else
      *dest++ = *pos++;

    ++count;
    if(count == sizeof(LexString))
      {
      /* Read up to end of string */
      while(*pos != LexStringDelim && *pos != '\0')
        ++pos;
      if(*pos == '"')
        ++pos;

      return TokErrStringTooLong;
      }
    }
  *dest = '\0';
  ++pos;  /* skip over trailing string delimiter */
  return TokString;
}

/***********************************************************
* ParseNumber
* parses an unsigned number in the input buffer. This will
* distinguish between an integer and a floating point
* number depending on whether a decimal point is present.
***********************************************************/
static int ParseNumber(void)
{
  char *ptr = pos;
  int tok = TokUndefined;

  assert(ptr != NULL);

  while (*ptr && isdigit(*ptr))
    ++ptr;

  if(*ptr == '.' && isdigit(*(ptr+1)))
    {
    LexDouble = strtod(pos,&pos);
    tok = TokFloat;
    }
  else
    {
    LexLong = strtol(pos,&pos,10);
    tok = TokInt;
    }
  return tok;
}



/***********************************************************
* ParseChar
* parses a single character i.e. 'A'
***********************************************************/
static int ParseChar(void)
{
  char ch;
  
  assert(*pos == LexCharDelim);

  ++pos;

  if(*pos == LexStringEscape)
    ch = EscapedChar();
  else
    ch = *pos++;

  /* Suck up to trailing delimeter */
  while(*pos != LexCharDelim)
    {
    if(*pos == '\0')
      return(TokUndefined);
    ++pos;
    }
  ++pos;

  LexChar = ch;
  return TokChar;

}


/***********************************************************
* ParseKeyword
* Looks for & parses any keywords
***********************************************************/
static int ParseKeyword(void)
{
  int tok = TokIdentifier;
  char chOld;
  char *ptr = pos;
  int i;

  assert(ptr != NULL);

  /* chop off identifier string */
  while(isident(*ptr))
    ++ptr;

  chOld = *ptr;
  *ptr = '\0';

  /* Now search for this string in keyword list */
  for (i=0;i<KeywordCount;++i)
    {
    if(strcmp(pos,KeyWords[i].text) == 0)
      {
      tok = KeyWords[i].tok;
      break;
      }
    }

  if(tok == TokIdentifier)  /*keyword not found */
    {
    strcpy(LexIdentifier,pos);
    }

  *ptr = chOld; /* restore char at end of keyword/identifier */
  pos = ptr;

  return tok;

}


/***********************************************************
* ParsePunctuation
* Looks for and parses any punctuation
***********************************************************/
static int ParsePunctuation(void)
{
  int i;
  int tok = TokUndefined;

  for (i=0;i<PunctuationCount;++i)
    {
    if(strncmp(pos,Punctuation[i].text,strlen(Punctuation[i].text)) == 0)
      {
      tok = Punctuation[i].tok;
      pos += strlen(Punctuation[i].text);
      break;
      }
    }
  return tok;
}

/***********************************************************
* PRS_LoadFile
* Opens a  file and reads from it.  This uses the magic
* number at the start of the file to work out whether the
* file is binary or text.
* Parameters: lpszFile is the name of the file to read from
* Returns:    0 for success, non-zero for failure.
***********************************************************/
int PRS_LoadFile(const char* lpszFile)
{
  BIN_HEADER header;
  FILE* oldfp = fpInput;
  int oldBin = blBinary;
  int err = FALSE;
  int IsEOF = FALSE;
  int fault = NO_FAULT;
  int blSaved = FALSE;
                     
  assert(lpszFile != NULL);
                       
  /* If the vritual machine is running then we need to save the
  // currently executing code before reading in the new stuff
  // from file
  */
  if(PRS_VMRunning())
    {
    err = PRSI_SaveCode();
    blSaved = !err;
    }

  /* Now go ahead and try to read the file.  First try for binary
  // then as per normal ascii file.
  */
  if(!err)
    {
    fpInput = fopen(lpszFile,"rb");
    blBinary = FALSE;
    if(fpInput != NULL)
      {
      if(fread(&header,sizeof(header),1,fpInput) == 1)
        {
        if(header.lMagic == LEX_MAGIC)  /* then must be binary file */
          blBinary = TRUE;
        }

      if(!blBinary)
        {
        fpInput = freopen(lpszFile,"rt",fpInput);
        }
      }

    /* Read in and process the file contents */
    if(fpInput != NULL)
      {
      while (!IsEOF && !PRS_ParseUnit(&IsEOF))
        {
        fault = PRS_RunVM();
        if(fault != HALT_FAULT && fault != NO_FAULT)
          break;
        }
      fclose(fpInput);
      }
    else
      err = TRUE;

    /* Restore old fpInput & blBinary in case this was recursive call */
    fpInput = oldfp;
    blBinary = oldBin;
    }

  if(blSaved)
    PRSI_RestoreCode();

  return err;
}

/***********************************************************
* LexReadBinary
* Reads a token out of a binary token file.
***********************************************************/
static int LexReadBinary(void)
{
  int tok;
  int ch;
  int iCount;

  assert(fpInput != NULL);

  tok = fgetc(fpInput);

  switch(tok)
    {
    case EOF:
      tok = TokEof;
      break;

    case TokString:
      iCount = 0;
      do
        {
        ch = fgetc(fpInput);
        if(ch == EOF)
          {
          tok = TokErrEOFInString;
          break;
          }

        if(iCount < BUFFLEN-1)
          LexString[iCount++] = (char)ch;
        else
          {
          tok = TokErrStringTooLong;
          break;
          }
        }
      while(ch != 0);
      break;

    case TokInt:
      if( 1 != fread(&LexLong,sizeof(long),1,fpInput))
        tok = TokErrFileReadInt;
      break;

    case TokFloat:
      if( 1 != fread(&LexDouble,sizeof(double),1,fpInput))
        tok = TokErrFileReadDouble;
      break;

    case TokChar:
      ch = fgetc(fpInput);
      if(EOF == ch)
        tok = TokErrFileReadChar;
      else
        LexChar = (char)ch;
      break;

    case TokIdentifier:
      iCount = 0;
      do
        {
        ch = fgetc(fpInput);
        if(ch == EOF)
          {
          tok = TokErrEOFInIdent;
          break;
          }

        if(iCount < BUFFLEN-1)
          LexIdentifier[iCount++] = (char)ch;
        else
          {
          tok = TokErrIdentTooLong;
          break;
          }
        }
      while(ch != 0);
      break;

    default:
      /* pass back token without further processing */
      break;
    }

 return tok;
}
/***********************************************************
* PRSI_LexPushBackToken
* Pushes a token back so that it will be re-read on the
* next call to LexGetToken().
* Parameters:  tok is the token to be pushed.
* Returns:     NOTHING
***********************************************************/
void PRSI_LexPushBackToken(int tok)
{
  if(tok != TokEof)
    PushedTokens[PushedCount++] = tok;
}

/***********************************************************
* PRSI_LexGetToken
* Parses, and returns the type of, the first token in
* the input buffer. This token is removed from the buffer.
* Parameters:     NONE
* Returns:        The first token in the input buffer.
***********************************************************/
int PRSI_LexGetToken(void)
{
  int tok;
  int found = 0;

  /* Handle any stored token */
  if( PushedCount > 0)
    {
    tok = PushedTokens[--PushedCount];
    found = 1;
    }

  /* If a binary input file is open then try to read from it */
  if(!found && blBinary)
    {
    tok = LexReadBinary();
    found = 1;
    }

  if(!found)
    {
    if(FillBuffer() == EOF)
      {
      tok = TokEof;
      found = 1;
      }
    }

  if (!found)
    {
    assert(pos != NULL);
    if(*pos == LexStringDelim)
      tok = ParseString();
    else if (isdigit(*pos))
      tok = ParseNumber();
    else if (*pos == LexCharDelim)
      tok = ParseChar();
    else if (isident(*pos))
      tok = ParseKeyword();
    else
      tok = ParsePunctuation();
    }

  /* PRSI_printf("Token: %s\n",LexTranslateToken(tok)); */
  return tok;
}



/***********************************************************
* PRS_SendCommand
* Allows a system to set up a string for parsing. This
* bypasses the normal (file?) input function and allows
* other code to send strings down for parsing.
* Parameters:   str is the command string
* Returns:      NOTHING
***********************************************************/
void PRS_SendCommand(const char* cmd)
{
  assert(cmd != NULL);
  strncpy(buff,cmd,sizeof(buff));
  buff[sizeof(buff)-1] = '\0';
  pos = buff;
  return;
}

/***********************************************************
* PRS_FormatCommand
* This formats a command (using printf format specifiers)
* and sends it to the parser for processing.
***********************************************************/
void PRS_FormatCommand(const char* cmd,...)
{
  va_list args;
  char buff[256];

  assert(cmd != NULL);
  
  va_start(args,cmd);
  vsprintf(buff,cmd,args);
  PRS_SendCommand(buff);
  va_end(args);

  return;
}

/***********************************************************
* LexSetKeywords
* This sets up the list of keywords the parser will
* recognise.
* Parameters: list is the list of keywords. It should have
*             a terminating entry with a NULL text field.
* Returns:    NOTHING
***********************************************************/
static void LexSetKeywords(TOKMATCH *list)
{
  int count = 0;
  int i,j;
  TOKMATCH temp;
  TOKMATCH *here = NULL;

  KeyWords = list;
  
  assert(list != NULL);

  /* First, count the number of keywords */
  for(here = list;here->text != NULL;++here)
    {
    ++count;
    }

  KeywordCount = count;

  /* Now make sure that any keyword that is the prefix of
  *  another, occurs after the other.
  */

  for (i=0;i<count;++i)
    {
    for (j=i+1;j<count;++j)
      {
      if (strncmp(list[i].text, list[j].text, strlen(list[i].text)) == 0)
        {
        temp = list[i];
        list[i] = list[j];
        list[j] = temp;
        }
      }
    }
  return;
}


/***********************************************************
* LexSetPunctuation
* This sets up the list of punctuation that the parser will
* recognise
* Parameters: list is the list of punctuation. It should
*             have a terminating entry with a NULL text
*             field.
* Returns:    NOTHING
***********************************************************/
static void LexSetPunctuation(TOKMATCH *list)
{
  int count = 0;
  int i,j;
  TOKMATCH temp;
  TOKMATCH *here = NULL;

  assert(list != NULL);
  
  Punctuation = list;

  for(here = list;here->text != NULL;++here)
    {
    ++count;
    }
  PunctuationCount = count;
  /* Now make sure that any keyword that is the prefix of
  *  another, occurs after the other.
  */

  for (i=0;i<count;++i)
    {
    for (j=i+1;j<count;++j)
      {
      if (strncmp(list[i].text, list[j].text, strlen(list[i].text)) == 0)
        {
        temp = list[i];
        list[i] = list[j];
        list[j] = temp;
        }
      }
    }
  return;
}

/***********************************************************
* PRS_InputFunction
* Allows the parser to set up a function that will be used
* by the lexical analyser to get its input.
* Parameters: fn is the input function. It takes a pointer
*             to a buffer and the buffer length. It returns
*             NULL if it cannot read any other input (ie
*             on EOF)
* Returns:    the old input function
***********************************************************/
PFSIS PRS_InputFunction(PFSIS fn)
{
  PFSIS oldfn = NULL;
  oldfn = LexReadBuffer;
  LexReadBuffer = fn;
  return oldfn;
}


/***********************************************************
* PRSI_LexGetDouble
* Returns the value of the last floating point number
* found.
* Parameters: NONE
* Returns:    The last floating point number found
***********************************************************/
double PRSI_LexGetDouble(void)
{
  return LexDouble;
}

/***********************************************************
* PRSI_LexGetLong
* Returns the value of the last integer found.
* Parameters: NONE
* Returns:    the last integer found.
***********************************************************/
long PRSI_LexGetLong(void)
{
  return LexLong;
}

/***********************************************************
* PRSI_LexGetString
* Gets a pointer to the last string found
* Parameters: NONE
* Returns:    a pointer to the last string found
***********************************************************/
char *PRSI_LexGetString(void)
{
  return LexString;
}

/***********************************************************
* PRSI_LexGetIdentifier
* Gets a pointer to the last identifier found
* Parameters:    NONE
* Returns:       a pointer to the last identifier found
***********************************************************/
char *PRSI_LexGetIdentifier(void)
{
  return LexIdentifier;
}

/***********************************************************
* PRSI_LexGetChar
* Gets the last character found
* Parameters:   NONE
* Returns:      the last character found.
***********************************************************/
char PRSI_LexGetChar(void)
{
  return LexChar;
}

/***********************************************************
* PRSI_LexTranslateToken
* Takes a token and translates it into an ascii form.
* Parameters: tok is the token to be translated
* Returns:    a string
***********************************************************/
const char *PRSI_LexTranslateToken(int tok)
{
  int i;
  char *result = NULL;
  int found = FALSE;
  
  switch(tok)
    {
    case TokUndefined:          result = "UNDEFINED";                             break;
    case TokEof:                result = "END OF FILE";                           break;
    case TokErrNewlineInString: result = "Newline in string";                     break;
    case TokErrStringTooLong:   result = "String too long";                       break;
    case TokErrEOFInString:     result = "End of input in string";                break;
    case TokErrFileReadInt:     result = "Could not read integer";                break;
    case TokErrFileReadDouble:  result = "Could not read double";                 break;
    case TokErrFileReadChar:    result = "Could not read char";                   break;
    case TokErrFileReadEOF:     result = "End of file";                           break;
    case TokErrEOFInIdent:      result = "End of file while reading identifier";  break;
    case TokErrIdentTooLong:    result = "Identifier too long";                   break;
    case TokString:             result = "STRING";                                break;
    case TokInt:                result = "INTEGER";                               break;
    case TokFloat:              result = "FLOAT";                                 break;
    case TokChar:               result = "CHAR";                                  break;
    case TokIdentifier:         result = "IDENTIFIER";                            break;

    default:
      found = 0;
      for(i=0;KeyWords[i].text != NULL;++i)
        {
        if(KeyWords[i].tok == tok)
          {
          result = KeyWords[i].text;
          found = 1;
          break;
          }
        }
      if (!found)
        {
        for(i=0;Punctuation[i].text != NULL;++i)
          {
          if(Punctuation[i].tok == tok)
            {
            result = Punctuation[i].text;
            found = 1;
            break;
            }
          }
        }
    if(!found)
      result = "NOT A TOKEN";
    }
  return result;
}

/***********************************************************
* PRSI_LexIsAKeyword
* Returns true IFF the name passed is a keyword
***********************************************************/
int PRSI_LexIsAKeyword(const char* name)
{
  int i;
  int found = 0;
  
  assert(name != NULL);
  
  for (i=0;i<KeywordCount;++i)
    {
    if(strcmp(name,KeyWords[i].text) == 0)
      {
      found = 1;
      break;
      }
    }
  return found;
}

/***********************************************************
* PRSI_LexShowInput
* returns a pointer to the input buffer and the position
* in it where the error occured.
***********************************************************/
void PRSI_LexShowInput(char **bptr,char **pptr)
{ 
  assert(bptr != NULL);
  assert(pptr != NULL);
  
  *bptr = buff;
  *pptr = pos;
}

/***********************************************************
* PRSI_LexFlushInput
* Clears out the input buffer. Used to help error recovery.
***********************************************************/
void PRSI_LexFlushInput(void)
{
  *buff = '\0';     /* clear the buffer */
  pos = buff;
  PushedCount = 0;  /* and dump any pushed tokens */
  return;
}


/***********************************************************
* PRSI_SetupLexAnalyser
* This initialises the lexical analyse for the parser.
***********************************************************/
void PRSI_SetupLexAnalyser(void)
{

  LexSetKeywords(keywords);
  LexSetPunctuation(punctuation);

  return;
}



