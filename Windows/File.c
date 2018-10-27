/* 
// File:        FILE.C
// 
// Prefix:
// 
// Description: File handling for PFA demonstration
//
*/

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "file.h"
 
static OPENFILENAME ofn ;
 
/************************************************************************
* Function:   FileInitialize
* This sets up the OPENFILENAME structure with default parameters which
* should be usable by most of the file dialogs
* Parameters: hwnd is the owner window handle
* Returns:    NOTHING
************************************************************************/
void FileInitialize(HWND hwnd)
{
/* filters: Note all one string */
  static char *szFilter = 
    { 
    "PASCAL Files (*.PAS)\0"  "*.pas\0"
    "All Files (*.*)\0"  "*.*\0"
    "\0"
    } ;
 
  ofn.lStructSize       = sizeof(OPENFILENAME);
  ofn.hwndOwner         = hwnd;
  ofn.hInstance         = NULL;
  ofn.lpstrFilter       = szFilter;
  ofn.lpstrCustomFilter = NULL;
  ofn.nMaxCustFilter    = 0;
  ofn.nFilterIndex      = 0;
  ofn.lpstrFile         = NULL;
  ofn.nMaxFile          = _MAX_PATH;
  ofn.lpstrFileTitle    = NULL;
  ofn.nMaxFileTitle     = 0;
  ofn.lpstrInitialDir   = NULL;
  ofn.lpstrTitle        = NULL;
  ofn.Flags             = 0;
  ofn.nFileOffset       = 0;
  ofn.nFileExtension    = 0;
  ofn.lpstrDefExt       = "txt";
  ofn.lCustData         = 0L;
  ofn.lpfnHook          = NULL;
  ofn.lpTemplateName    = NULL;
}

/************************************************************************
* Function:   FileOpenDlg
* This pops up the standard file open dialog. FileInitialise should 
* have been called previously.
* Parameters: hwnd is the handle of the owner window
*             lpstrFileName is used to return the filename
* Returns:    FALSE on error
************************************************************************/
BOOL FileOpenDlg(HWND hwnd, LPSTR lpstrFileName)
{
  FileInitialize(hwnd);
  ofn.hwndOwner         = hwnd;
  ofn.lpstrFile         = lpstrFileName;
  ofn.lpstrFileTitle    = NULL;
  ofn.Flags             = OFN_CREATEPROMPT;
 
  return GetOpenFileName(&ofn);
}

/************************************************************************
* Function:   FileSaveDlg
* Prompts the user for a save dialog.
* Parameters: hwnd is the handle of the owner window
*             lpstrFileName is used to return the filename
* Returns:    FALSE on error
************************************************************************/
BOOL FileSaveDlg(HWND hwnd, LPSTR lpstrFileName)
{
  FileInitialize(hwnd);
  ofn.hwndOwner       = hwnd;
  ofn.lpstrFile       = lpstrFileName;
  ofn.lpstrFileTitle  = NULL;
  ofn.Flags           = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
 
  return GetSaveFileName (&ofn);
}
 
/************************************************************************
* Function:   FileLength
* static function which returns the length of an open file
* Parameters: hFile is the file handle
* Returns:    The length in bytes.
************************************************************************/
static long FileLength(int hFile)
{
  long lCurrentPos = _llseek (hFile, 0L, SEEK_CUR);
  long lFileLength = _llseek (hFile, 0L, SEEK_END);
     
  _llseek (hFile, lCurrentPos, SEEK_SET);
 
  return lFileLength ;
}
 
/************************************************************************
* Function:   FileRead
* This reads a file into an edit control
* Parameters: hwndEdit is the window handle of the edit control
*             lpstrFileName is the path to the file
* Returns:    TRUE for success, FALSE for failure
************************************************************************/
BOOL FileRead(HWND hwndEdit, LPSTR lpstrFileName)
{
  long   lLength ;
  int    hFile ;
  LPSTR  lpstrBuffer ;
 
  if(-1 == (hFile = _lopen(lpstrFileName, OF_READ | OF_SHARE_DENY_WRITE)))
    return FALSE ;

  if ((lLength = FileLength (hFile)) >= 32000)
    {
    _lclose (hFile) ;
    return FALSE ;
    }
 
  if (NULL == (lpstrBuffer = GlobalAllocPtr(GHND, lLength + 1)))
    {
     _lclose (hFile) ;
    return FALSE ;
    }
 
  _lread(hFile, lpstrBuffer,(WORD) lLength) ;
  _lclose(hFile) ;
  lpstrBuffer[(WORD) lLength] = '\0' ;
 
  SetWindowText (hwndEdit, lpstrBuffer) ;
  GlobalFreePtr(lpstrBuffer) ;
  return TRUE ;
}

/************************************************************************
* Function:   FileWrite
* This writes the text from an edit window to a given file 
* Parameters: hwndEdit is the window handle of the edit control
*             lpstrFileName is the name of the file to write to
* Returns:    TRUE for success, FALSE for failur
************************************************************************/
BOOL FileWrite(HWND hwndEdit, LPSTR lpstrFileName)
{
  HANDLE hBuffer ;
  int    hFile ;
  LPSTR  lpstrBuffer ;
  WORD   wLength ;
 
  if(-1 == (hFile = _lopen(lpstrFileName, OF_WRITE | OF_SHARE_EXCLUSIVE)))
    if(-1 == (hFile = _lcreat(lpstrFileName, 0)))
      return FALSE ;
 
  wLength = (WORD)GetWindowTextLength(hwndEdit) ;
  hBuffer = Edit_GetHandle(hwndEdit);
  lpstrBuffer = (LPSTR)LocalLock(hBuffer) ;
 
  if (wLength != _lwrite(hFile, lpstrBuffer, wLength))
    {
    _lclose (hFile) ;
    return FALSE ;
    }
 
  _lclose(hFile) ;
  LocalUnlock(hBuffer) ;
 
  return TRUE ;
}

  
