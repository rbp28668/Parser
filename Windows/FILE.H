#ifndef _FILE_H
#define _FILE_H
void FileInitialize(HWND hwnd);
BOOL FileOpenDlg(HWND hwnd, LPSTR lpstrFileName);
BOOL FileSaveDlg(HWND hwnd, LPSTR lpstrFileName);
BOOL FileRead (HWND hwndEdit, LPSTR lpstrFileName);
BOOL FileWrite(HWND hwndEdit, LPSTR lpstrFileName);

#endif

