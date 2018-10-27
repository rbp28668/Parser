/* 
// File:        LEX.C
// 
// Prefix:      Lex
// 
// Description: Include file for lexical analyser
//
*/


#ifndef _LEX_H
#define _LEX_H

#define LEX_MAGIC 0x40404040l /* magic number to mark binary file */
#define LEX_INTEL 0x4949      /* intel byte ordering: "II" */
#define LEX_MOTOROLA 0x4D4D   /* motorola byte ordering "MM" */

enum LexPredefinedToks
{
  TokUndefined,
  TokEof,
  TokErrNewlineInString,
  TokErrStringTooLong,
  TokErrEOFInString,
  TokErrFileReadInt,
  TokErrFileReadDouble,
  TokErrFileReadChar,
  TokErrFileReadEOF,
  TokErrEOFInIdent,
  TokErrIdentTooLong,
  TokString,
  TokInt,
  TokFloat,
  TokChar,
  TokIdentifier,
  TokFirstUser
};

typedef struct s_tokmatch
{
  char *text;
  int tok;
}TOKMATCH;

typedef struct s_bin_header
{
  long lMagic;
  short byteOrdering;
  short nSize;          /* of this header */
  short nVerMajor;
  short nVerMinor;
  short nVerSub;
  short nTimeSec;
  short nTimeMin;
  short nTimeHr;
  short nDateDay;
  short nDateMonth;
  short nDateYr;
  char achSpare[6];
}BIN_HEADER;


extern int PRSI_LexGetToken(void);
extern void PRSI_LexPushBackToken(int tok);
extern PFSIS PRS_InputFunction(PFSIS);

extern double PRSI_LexGetDouble(void);
extern long PRSI_LexGetLong(void);
extern char *PRSI_LexGetString(void);
extern char *PRSI_LexGetIdentifier(void);
extern char PRSI_LexGetChar(void);
extern const char *PRSI_LexTranslateToken(int tok);
extern int PRSI_LexIsAKeyword(const char* name);
extern void PRSI_LexShowInput(char **bptr,char **pptr);
extern void PRSI_LexFlushInput(void);
extern void PRSI_SetupLexAnalyser(void);
#endif
