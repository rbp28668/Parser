/* ENVIRON.H
// This file defines all the compilation environment stuff
*/

#ifndef _ENVIRON_H
#define _ENVIRON_H

/* If compiling for Windows MSC defines _WINDOWS and Borland _Windows */
#if defined _WINDOWS || defined _Windows

#define PRS_WINDOWS 1

#define STRICT 
#include <windows.h>

#else /* not in windows */

#define HINSTANCE int
enum bool {FALSE,TRUE};

#endif

#endif


