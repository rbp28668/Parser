/* FRAME.H
// Declarations for frame window functions
*/
#ifndef _FRAME_H
#define _FRAME_H
extern int InitFrame(HINSTANCE hInstance);
extern HWND CreateFrame(HINSTANCE hInstance);
extern void ShowFrame(int nCmdShow);
extern HWND FrameClientWindow(void);
extern void FrameAttachMenu(HMENU hmenu, int iWindowMenuPos);
extern void FrameRemoveMenu(void);
extern LPCSTR FrameClass(void);
extern void FramePostMessage(UINT message, WPARAM wParam, LPARAM lParam);
#endif


