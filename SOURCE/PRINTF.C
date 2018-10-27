#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "printf.h"
#include "prsif.h"

/**********************************************************************
* Function:   PRSI_printf
* replaces the standard printf but sends output to the main output
* Parameters: fmt is a format string, any other args as for printf
*
* Returns:    The number of chars output
**********************************************************************/
int PRSI_printf(char* fmt,...)
{
  char szBuff[128];
  va_list ap = NULL;
  int nchars = 0;

  assert(fmt != NULL);
  
  va_start(ap,fmt);

  nchars = vsprintf(szBuff,fmt,ap);

  PRS_Write(szBuff);

  va_end(ap);

  return nchars;
}
