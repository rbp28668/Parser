/* 
// File:        ERROR.C
// 
// Prefix:      NONE
// 
// Description: Error handling for parser
//
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "prsif.h" /* parser interface */
#include "lex.h"
#include "malloc.h"
#include "gettext.h"


/* Function pointer to allow re-direction of error output */
static PFCSV errfn = NULL;
static char szErrFile[_MAX_PATH] = "PARSER.ERR";
static char buff[256] = "";

/**********************************************************************
* PRS_Error
* Displays a suitable error message
**********************************************************************/
void PRS_Error(int err,...)
{
  va_list ap;
  char *ipbuff = NULL;
  char *pos = NULL;
  int spaces;
  int i;
  char** msg = NULL;

  va_start(ap,err);

  msg = PRSI_GetText(szErrFile,err);
  if (msg != NULL)
    {
    vsprintf(buff,*msg,ap);
    free(msg);
    }
  else
    sprintf(buff,"%d",err);

  va_end(ap);

  if(errfn != NULL)
    (*errfn)(buff);

  PRSI_LexShowInput(&ipbuff,&pos);
  for(i=0; i < (int)strlen(ipbuff); ++i) /* strip any CRs etc */
    {
    if(iscntrl(ipbuff[i]))
      ipbuff[i] = ' ';
    }

  if(errfn != NULL)
    (*errfn)(ipbuff);

  spaces = (int)(pos-ipbuff);

  for (i=0;i<spaces;++i)
    buff[i] = ' ';

  buff[i++] = '^';
  buff[i++] = '\0';

  if(errfn != NULL)
    {
    (*errfn)(buff);
    (*errfn)(NULL);
    }
    
  return;
}

/**********************************************************************
* PRS_SetErrorHandler
* sets up an error handler for the parser
**********************************************************************/
PFCSV PRS_SetErrorHandler(PFCSV fn)
{
  PFCSV olderr = errfn;
  errfn = fn;
  return olderr;
}

/**********************************************************************
* PRSI_InitError
* Initialises the error routines.  In windows this finds the 
* application path: this should lead to the error file.
**********************************************************************/
void PRSI_InitError(HINSTANCE hInst)
{
  #ifdef PRS_WINDOWS
  
  char* pszSlash = NULL;
  
  if(GetModuleFileName(hInst,szErrFile,sizeof(szErrFile)))
    {
    /* Find the last forward or backslash in the app name */
    pszSlash = strrchr(szErrFile,'\\');
    if(pszSlash == NULL)
      pszSlash = strrchr(szErrFile, '/');
    
    /* chop of file name leaving path */      
    if(pszSlash != NULL)
      pszSlash[1] = '\0';
    else
      szErrFile[0] = '\0';

    strcat(szErrFile,"PARSER.ERR");    
    }
  
  #else
    strcpy(szErrFile,"PARSER.ERR");
  #endif
}
