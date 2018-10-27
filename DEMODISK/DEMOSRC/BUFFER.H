#ifndef _BUFFER_H
#define _BUFFER_H

extern BOOL InitOutputBuffer(void);
extern void DeleteOutputBuffer(void);
extern void BufferLine(LPCSTR pszLine);
extern void ClearBuffer(void);
extern LPCSTR BufferedLine(int idx);
extern int BufferLineCount(void);

#endif

