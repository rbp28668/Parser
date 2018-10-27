#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "buffer.h"

#define BUFFSIZE 32768L           /* size of line buffer */
#define BUFFLINES 512             /* number of lines in it */

static char* pchMem = NULL;       /* Global memory block */
static char** ppchLines = NULL;   /* Ptrs to start of each line */
static char* pchBuff = NULL;      /* buffer for text data (pointed into by ppchLines) */
static int nLines = 0;            /* number of lines stored */
static int nLinePos = 0;          /* where the next line goes in ppchLines */
static int nWasted = 0;           /* wasted space at end of buffer */
static DWORD dwBytesUsed = 0;     /* no of bytes used in buffer */
static char* pchBuffHead = NULL;  /* head of buffer */


/************************************************************************
* Function:   InitOutputBuffer
* This initialises the output buffer.
* Parameters: NONE
* Returns:    TRUE for success, FALSE for failure
************************************************************************/
BOOL InitOutputBuffer(void)
{
  assert(pchMem == NULL);         /* problem if already allocated */
  
  pchMem = GlobalAllocPtr(GHND,BUFFSIZE + BUFFLINES * sizeof(char*));
  if(pchMem == NULL)
    return FALSE; /* failed */
    
  ppchLines = (char**)pchMem;
  pchBuff = pchMem + BUFFLINES * sizeof(char*);  
  pchBuffHead = pchBuff;
  dwBytesUsed = 0;
  nLines = 0;   
  return TRUE;
}

/************************************************************************
* Function:   DeleteOutputBuffer
* This destroys the output buffer. (N.B. safe to call this even if
* init failed)
* Parameters: NONE
* Returns:    NOTHING
************************************************************************/
void DeleteOutputBuffer(void)
{
  if(pchMem)
    GlobalFreePtr(pchMem);
  pchMem = NULL;
  ppchLines = NULL;
  pchBuff = NULL;
}

/************************************************************************
* Function:   BufferLine   
* This buffers a single line of text
* Parameters: pszLine is the line to buffer
* Returns:    NOTHING
************************************************************************/
void BufferLine(LPCSTR pszLine)
{
  int len = 0;
  char *pszOld = 0;
  ptrdiff_t ptdEndSpace = 0;
  int nTail = 0;
              
  len = strlen(pszLine)+1;
                
  /* Sort out wrap-around in text buffer */  
  ptdEndSpace = (pchBuff + BUFFSIZE) - pchBuffHead;
  if( ptdEndSpace < (ptrdiff_t)len)     /* not enough space at end*/
    {
    pchBuffHead = pchBuff;              /* so start again at beginning */
    nWasted = (int)ptdEndSpace;         /* and record the wasted space */
    }
    
  /* Get pointer to any string we are about to over-write,
  // and delete it.
  */
  pszOld = ppchLines[nLinePos];
  if(pszOld != NULL)
    {
    dwBytesUsed -= (strlen(pszOld) + 1);
    ppchLines[nLinePos] = NULL;
    --nLines;
    }
      
  /* Now, if we are running out of space, delete tail lines to free some up */
  nTail = (nLinePos - nLines + BUFFLINES) % BUFFLINES;
  while((DWORD)len > BUFFSIZE-(dwBytesUsed + nWasted))
    {
    pszOld = ppchLines[nTail];
    assert(pszOld!= NULL); /* problem if tail line empty */
    dwBytesUsed -= (strlen(pszOld) + 1);
    ppchLines[nTail] = NULL;
    --nLines;
    if(++nTail == BUFFLINES)
      nTail = 0;
    }

  /* Write the data into the buffer and bump the indices accordingly */      
  strcpy(pchBuffHead,pszLine);        
  ppchLines[nLinePos] = pchBuffHead;
  dwBytesUsed += len;
  pchBuffHead += len;                 /* won't wrap round: len was checked above */
  ++nLinePos;                         /* get new index in line table */
  if(nLinePos == BUFFLINES)
    nLinePos = 0;
  ++nLines;
  
  return;                     
}

/************************************************************************
* Function:   ClearBuffer
* This clears the output buffer to empty
* Parameters: NONE
* Returns:    NOTHING
************************************************************************/
void ClearBuffer(void)
{ 
  int i;
  
  nLines = 0;            /* number of lines stored */
  nLinePos = 0;          /* where the next line goes in ppchLines */
  nWasted = 0;           /* wasted space at end of buffer */
  dwBytesUsed = 0;        /* no of bytes used in buffer */
  pchBuffHead = pchBuff; /* head of buffer */
  
  for(i=0; i<BUFFLINES; ++i)
    ppchLines[i] = NULL;
  return;
}

/************************************************************************
* Function:   BufferedLine
* This gets a pointer to line in the output buffer.
* Parameters: idx indexes into the buffer. idx = 0 gives the oldest
*             line in the buffer, idx = BufferLineCount-1, the newest
* Returns:    A pointer to the line
************************************************************************/
LPCSTR BufferedLine(int idx)
{
  if(idx < 0 || idx >= nLines)
    {
    return NULL;
    }
    
  idx = (nLinePos - nLines + idx + BUFFLINES)%BUFFLINES;
  return ppchLines[idx];
}

/************************************************************************
* Function:   BufferLineCount
* This returns a count of the number of lines stored in the output
* buffer.
* Parameters: NONE
* Returns:    The line count
************************************************************************/
int BufferLineCount(void)
{
  return nLines;
}
