#ifndef _OUTPUT_H
#define _OUTPUT_H

extern void OutputFunction(const char* pszText);
extern BOOL OutputExists(void);
extern int InitOutput(HINSTANCE hInstance);
extern HWND CreateOutputWindow(HWND hwndClient);
extern void OutputKillMenu(HMENU hmenuCurrent);

#endif


