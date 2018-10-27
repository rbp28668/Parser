/* APP.H
// Header for application functions
*/
#ifndef _APP_H
#define _APP_H
              
extern HANDLE AppInstance(void);
extern LPCSTR AppName(void);
extern void AppError(HWND hwnd,UINT uires);
extern void AppWarning(HWND hwnd,UINT uires);
extern void AppAbout(HWND hwnd);
extern void AppMessagePump(void);
extern BOOL AppProcessing(void);
extern void AppPause(BOOL blYes);
extern BOOL AppIsPaused(void);
 
#endif
