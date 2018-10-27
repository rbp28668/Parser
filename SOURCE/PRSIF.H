/* PRSIF.H
// This file defines the interface to the Control Pascal Interpreter
// library.
*/

#ifndef _PRSIF_H
#define _PRSIF_H

#include "environ.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Typedef for pointer to function taking string (char*), int and returning 
// a string (char*). This is used for setting up the parser input function.
*/
typedef char* (*PFSIS)(char*,int);

/* Typedef for pointer to function taking and returning integer.
// This is used for the i-code fault handler.
*/
typedef int (*PFII)(int);

/* Typedef for pointer to void function returning int.
// This is used for extension functions.
*/
typedef int(*PFVI)(void);       

/* Typedef for pointer to function taking a const string and returning void.
// This is used for setting the error handler and for the printing function.
*/
typedef void(*PFCSV)(const char *); 

/* The different fault types for the I-Code interpreter */
enum fault
{
  NO_FAULT,
  HALT_FAULT,
  BOUNDS_FAULT,
  ZDIV_FAULT,
  EVAL_STACK_OVERFLOW_FAULT,
  EVAL_STACK_UNDERFLOW_FAULT,
  RETURN_STACK_OVERFLOW_FAULT,
  RETURN_STACK_UNDERFLOW_FAULT,
  NULL_CALL_FAULT,
  MISMATCH_CASE_FAULT,
  UNIMPLEMENTED_INSTR_FAULT,
  USER_FAULT
};


/* Functions to initialise and terminate the parser.
// PRS_InitialiseInterpreter should be the first function in the
// parser library that is called. Call PRS_TerminateInterpreter to
// release any resources used by the interpreter.
*/
extern int PRS_InitialiseInterpreter(HINSTANCE hInst);
extern void PRS_TerminateInterpreter(void);

/* Function to parse (and run if appropriate) a complete syntactic
// unit. For example a const, type or variable declaration, 
// a procedure or function definition or a simple statement.
// This returns 0 for success, TRUE if there was an error during
// parsing.
// *IsEOF is set to TRUE if it needs, but cannot get, additional input
// to complete the unit.
*/
extern int PRS_ParseUnit(int* IsEOF);


/* Function which will read a pascal file into the parser.  This
// will either be  a binary input file (created by
// PAS2BIN.EXE) or an ascii source file. Returns
// false if the file can be opened, true on error.
*/
extern int PRS_LoadFile(const char* lpszFile);

/* Function which sets up a function to get input for the parser.
// The input function takes a pointer to a character buffer and
// an integer which gives the buffer size.  The input function
// should read the input into the buffer and return a pointer to
// the buffer.  If there is no input then the input function
// should return NULL.
*/
extern PFSIS PRS_InputFunction(PFSIS fn);

/* Functions to format and send commands to the parser.
// PRS_FormatCommand accepts printf style formatting characters.
// PRS_SendCommand sends the string directly for parsing.
*/
extern void PRS_FormatCommand(const char* cmd,...);
extern void PRS_SendCommand(const char* cmd);

/* PRS_Forget removes the identifier given by name, and any
// identifiers declared after it, from the parser's symbol table.
// Any associated storeage is also freed.  PRS_Fence sets up 
// any global identifier so that it, and any identifier declared
// before it, cannot be forgotten. This should be used to protect
// identifiers declared by the application.
*/
extern int PRS_Forget(const char* name);
extern int PRS_Fence(const char* name);

/* PRS_SetErrorHandler allows an application to set up an error handler
// for parser errors.  The error handler will be passed a const string:
// it is up to the handler to display or log it.  The fault handler
// is called whenever the I-code processor detects a fault. These
// faults are all run-time faults, for example array bounds fault.
// The fault handler can take appropriate action for the fault and may
// suppress the fault by returning NO_FAULT. 
*/
extern PFCSV PRS_SetErrorHandler(PFCSV fn);
extern PFII PRS_SetFaultHandler(PFII fn);

/* These functions run the virtual machine. Either call PRS_RunVM() after
// PRS_ParseUnit returns 0 or instead of PRS_RunVM() call
// if(PRS_StartVM())
//   while(PRS_TickVM())
//     ;
*/

extern int PRS_RunVM(void);
extern int PRS_StartVM(void);
extern int PRS_TickVM(void);
extern int PRS_VMRunning(void);
extern void PRS_StopVM(void);


/* These functions allow an application to get the address or
// values of global variables in the parser.  These variables may
// be simple variables such as integers, floats or compound
// variables such as records and arrays.
*/

extern int PRS_VariableAddress(const char* lpszVar,void** lplpVoid);
extern int PRS_Real(const char* lpszVar,float* fVal);
extern int PRS_Ordinal(const char* lpszVar,long* lVal);
extern int PRS_String(const char* lpszVar, const char** lplpszVal);


/* PRS_SetCExtension allows an application to extend the function
// of the basic interperter.  It does this by tying a C function
// [ int fn(void) ] to a Pascal procedure or function declaration.
*/
extern int PRS_SetCExtension(PFVI lpfi,const char *lpszDecl);

/* These functions allow a C extension function to access parameters
// from the i-code interpeter stack and to return values.  The parameters
// should be accessed as a single structure.  The first parameter will
// be at the lowest address. Var parameters are passed as pointers, integer
// parameters as long and real parameters as float.
*/
extern void* PRS_Parameters(int size);
extern int PRS_ReturnReal(float fVal);
extern int PRS_ReturnOrdinal(long lVal);
extern int PRS_ReturnPtr(void* ptr);

/* PRS_Version returns the current parser version */
extern void PRS_Version(int* lpiMajor, int* lpiMinor);

/* PRS_SetOutputFunction sets the output function which is used
// to send data from write, writeln and page and PRS_Write which
// will send a string to this
*/
extern PFCSV PRS_SetOutputFunction(PFCSV lpfnNewFn);
extern void PRS_Write(char* lpsz);

/* Functions for examining the symbol table */
extern void PRS_ShowIdentifiers(int ShowLocals);
extern int PRS_WhatIs(const char* lpszIdent);


/* These are the declarations for the string handling. */

/* String structure. The interpreter passes StringT*
// as if they were strings themselves
*/
typedef struct s_string
{
  int ref;        /* reference count of string */
  int len;        /* Current length of string */
  int alloc;      /* number of bytes allocated */
  char *text;     /* the string data itself */
}StringT;

typedef struct s_string* PSTRING;

#define PRS_StringLength(s) ((s)->len)
extern PSTRING PRS_CreateString(const char * str);
extern PSTRING PRS_CreateStringOfLength(int len);
extern const char* PRS_StringContents(PSTRING pstr);
extern PSTRING PRS_SetString(PSTRING pstr, const char* str);
extern PSTRING PRS_CloneString(PSTRING pstr);
extern void PRS_DeleteString(PSTRING pstr);

#ifdef __cplusplus
};
#endif

#endif







