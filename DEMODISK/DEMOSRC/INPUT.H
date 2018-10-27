#ifndef _INPUT_H
#define _INPUT_H

extern char* InputFunction(char* szBuff, int cbBuff);
extern void InputRun(void);

extern BOOL InputExists(void);
extern int InitInput(HINSTANCE hInstance);
extern HWND CreateInputWindow(HWND hwndClient);
extern void InputKillMenu(HMENU hmenuCurrent);

#endif
