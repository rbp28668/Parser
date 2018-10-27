/* 
// File:        PSTRING.H
// 
// Prefix:      
// 
// Description: Pascal string handling declarations
//
*/

#ifndef _PSTRING_H
#define _PSTRING_H 1

/* Note that the definition of a string is included in the
// parser interface so to avoid having 2 definitions of the same
// thing we include it here.
*/
#include "prsif.h"

#if 0
typedef struct s_string
{
  int ref;        /* reference count of string */
  int len;        /* Current length of string */
  int alloc;      /* number of bytes allocated */
  char *text;     /* the string data itself */
}StringT;
#endif

extern int PRSI_StrCmp(void);
extern void PRSI_ReferenceString(PSTRING pstr);

#endif
