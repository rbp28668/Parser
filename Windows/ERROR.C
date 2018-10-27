/* 
// File:        ERROR.C
// 
// Prefix:
// 
// Description: Error handling function for PFA demo
//
*/

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include "error.h"
#include "app.h"
 

static char szBuff[1024] = "";
 
/**********************************************************************
* Function:   ErrorFunction
* This is the PFA error handler.  It buffers error strings with
* CRs until it gets a NULL string.  Then it displays the whole buffer.
* Parameters: pszErr is the error string or NULL to signal the end
*             of the error strings.
* Returns:    NOTHING
**********************************************************************/
void ErrorFunction(const char* pszErr)
{
  if(pszErr)
    {
    strcat(szBuff,pszErr);
    strcat(szBuff,"\r");
    }
  else
    {
    MessageBox(NULL,szBuff,AppName(),MB_OK | MB_ICONSTOP);
    strcpy(szBuff,"");
    }
    
}