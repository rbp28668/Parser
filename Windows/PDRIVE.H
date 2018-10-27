#ifndef _PDRIVE_H
#define _PDRIVE_H

/* Structure to hold a C function and its pascal declaration */
typedef struct s_FunctionTable
{
  PFVI Function;
  char* Declaration;
}FunctionTableT;

extern void SetupCFunctions(FunctionTableT* table);

extern int QuitInterpreter(void);

extern void SetupParserExtensions(void);
extern void SetupStringExtensions(void);
#endif
