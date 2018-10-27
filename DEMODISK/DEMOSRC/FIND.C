#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <string.h>
#include <assert.h>
#include "find.h"

#define MAX_STRING_LEN 256
 
static char szFindText[MAX_STRING_LEN];
static char szReplText[MAX_STRING_LEN];
 
/************************************************************************
* Function:   FindFindDlg
* 
* Parameters:
* Returns:
************************************************************************/
HWND FindFindDlg(HWND hwnd)
{
  static FINDREPLACE fr;
  
  fr.lStructSize      = sizeof(FINDREPLACE);
  fr.hwndOwner        = hwnd;
  fr.hInstance        = NULL;
  fr.Flags            = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD;
  fr.lpstrFindWhat    = szFindText;
  fr.lpstrReplaceWith = NULL;
  fr.wFindWhatLen     = sizeof(szFindText);
  fr.wReplaceWithLen  = 0;
  fr.lCustData        = 0;
  fr.lpfnHook         = NULL;
  fr.lpTemplateName   = NULL;
  
  return FindText(&fr);
}
 
/************************************************************************
* Function:   FindReplaceDlg
* 
* Parameters:
* Returns:
************************************************************************/
HWND FindReplaceDlg(HWND hwnd)
{
  static FINDREPLACE fr;
   
  fr.lStructSize      = sizeof(FINDREPLACE);
  fr.hwndOwner        = hwnd;
  fr.hInstance        = NULL;
  fr.Flags            = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD;
  fr.lpstrFindWhat    = szFindText;
  fr.lpstrReplaceWith = szReplText;
  fr.wFindWhatLen     = sizeof(szFindText);
  fr.wReplaceWithLen  = sizeof(szReplText);
  fr.lCustData        = 0;
  fr.lpfnHook         = NULL;
  fr.lpTemplateName   = NULL;
  
  return ReplaceText(&fr);
}
 
/************************************************************************
* Function:   FindFindText
* 
* Parameters:
* Returns:
************************************************************************/
BOOL FindFindText(HWND hwndEdit, WORD* piSearchOffset, LPFINDREPLACE lpfr)
{
  WORD         iPos;
  HLOCAL      hLocal;
  LPSTR       lpstrDoc;
  LPSTR       lpstrPos;
 
  /* Get a pointer to the edit document */
  hLocal   = Edit_GetHandle(hwndEdit);
  lpstrDoc = (LPSTR)LocalLock(hLocal);
 
  /* Search the document for the find string */
  lpstrPos = _fstrstr (lpstrDoc + *piSearchOffset, lpfr->lpstrFindWhat);
  LocalUnlock (hLocal);

  /* Return an error code if the string cannot be found */
  if (lpstrPos == NULL)
    return FALSE;

  /* Find the position in the document and the new start offset */
  iPos = (WORD)(lpstrPos - lpstrDoc);
  *piSearchOffset = iPos + _fstrlen(lpfr->lpstrFindWhat);
 
  /* Select the found text */
  Edit_SetSel(hwndEdit,iPos,*piSearchOffset);
 
  return TRUE;
}
 
/************************************************************************
* Function:   FindNextText
* 
* Parameters:
* Returns:
************************************************************************/
BOOL FindNextText(HWND hwndEdit, WORD* piSearchOffset)
{
  FINDREPLACE fr;

  fr.lpstrFindWhat = szFindText;
 
  return FindFindText(hwndEdit, piSearchOffset, &fr);
}
 
/************************************************************************
* Function:   FindReplaceText
* 
* Parameters:
* Returns:
************************************************************************/
BOOL FindReplaceText(HWND hwndEdit, WORD* piSearchOffset, LPFINDREPLACE lpfr)
{
  /* Find the text */
  if (!FindFindText(hwndEdit, piSearchOffset, lpfr))
    return FALSE;
 
  /* Replace it */
  Edit_ReplaceSel(hwndEdit, lpfr->lpstrReplaceWith);

  return TRUE;
}
 
/************************************************************************
* Function:   FindValidFind
* Determines whether there is anything stored in the find string
* Parameters: NONE
* Returns:    TRUE if the find string is valid FALSE otherwise
************************************************************************/
BOOL FindValidFind(void)
{
  return *szFindText != '\0';
}

  
