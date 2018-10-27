#ifndef _FIND_H
#define _FIND_H
extern HWND FindFindDlg(HWND hwnd);
extern HWND FindReplaceDlg(HWND hwnd);
extern BOOL FindFindText(HWND hwndEdit, WORD* piSearchOffset, LPFINDREPLACE lpfr);
extern BOOL FindNextText(HWND hwndEdit, WORD* piSearchOffset);
extern BOOL FindReplaceText(HWND hwndEdit, WORD* piSearchOffset, LPFINDREPLACE lpfr);
extern BOOL FindValidFind(void);

#endif

