/* ERROR.H
*  Parser error handling functions
*/

#ifndef _PERROR_H
#include "environ.h"
#include "errcode.h"

extern void PRS_Error(int err,...);
extern void PRSI_InitError(HINSTANCE hInst);

#define _PERROR_H 1
#endif


