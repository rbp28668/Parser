#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <assert.h>
#include "output.h"
#include "input.h"
#include "frame.h"
#include "buffer.h"
#include "app.h"
#include "resource.h"

#ifdef _MSC_VER
#pragma warning ( disable : 4100 )  /* unreferenced formal parameter */
#endif

/* Forward declarations for callbacks and message handlers */
LRESULT CALLBACK OutputWndProc(HWND hWnd ,UINT message ,WPARAM wParam ,LPARAM lParam);
static BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct);
static void OnPaint(HWND hwnd);
static void OnSize(HWND hwnd, UINT state, int cx, int cy);
static void OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
static void OnHScroll(HWND hwnd, HWND hwndCtrl, UINT code, int pos);
static void OnVScroll(HWND hwnd, HWND hwndCtrl, UINT code, int pos);
static void OnInitMenuPopup(HWND hwnd, HMENU hMenu, int item, BOOL fSystemMenu);
static void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
static void OnDestroy(HWND hwnd);
static void OnMDIActivate(HWND hwnd, BOOL fActive, HWND hwndActivate, HWND hwndDeactivate);

static char szOutputClass[] = "Output";
static HWND hwndOutput = NULL;
static HMENU hMenu = 0;

static int cxChar, cxCaps, cyChar;
static int cxClient, cyClient;
static int nMaxWidth;
static int nVscrollPos, nVscrollMax;
static int nHscrollPos, nHscrollMax;

/**********************************************************************
* Function:   OutputFunction
* This handles the output from the parser.  The output is buffered
* and the window is invalidated to redraw the text
* Parameters: pszText is the line of text to draw, or NULL to clear
*             the page
*
* Returns:    NOTHING
**********************************************************************/
void OutputFunction(const char* pszText)
{
  int nOldScroll = 0;
  int cyLines = 0;
  RECT rect;
   
  if(pszText == NULL)
    {
    ClearBuffer();
    if(hwndOutput != NULL)
      InvalidateRect(hwndOutput,NULL,TRUE);
    }
  else
    {
    BufferLine(pszText);
    }
    
  if(hwndOutput != NULL)
    {
    nOldScroll = nVscrollPos;
    cyLines = cyClient/cyChar;

    nVscrollMax = max(0,BufferLineCount() - cyLines);
    nVscrollPos = nVscrollMax;
    
    SetScrollRange(hwndOutput,SB_VERT,0,nVscrollMax,FALSE);
    SetScrollPos(hwndOutput,SB_VERT,nVscrollPos,TRUE);
              
    if(BufferLineCount() <= cyLines)      /* if can display whole output in client area */
      {
      rect.top = (BufferLineCount() - 1) * cyChar;
      rect.bottom = rect.top + cyChar;
      rect.left = 0;
      rect.right = cxClient;
      }
    else
      {                    
      int nScroll = max(nVscrollPos - nOldScroll, 1);
      nScroll *= cyChar;
      ScrollWindow(hwndOutput, 0, -nScroll, NULL, NULL);

      rect.top = (cyLines - 1) * cyChar;
      rect.bottom = rect.top + cyChar;
      rect.left = 0;
      rect.right = cxClient;
      }
    InvalidateRect(hwndOutput,&rect,TRUE);
    UpdateWindow(hwndOutput);
    }
}

/************************************************************************
* Function:   OutputExists
* Determines whether there is an output window.
* Parameters: NONE
* Returns:    TRUE if the window exists, FALSE otherwise
************************************************************************/
BOOL OutputExists(void)
{
  return hwndOutput != NULL;
}

/************************************************************************
* Function:   InitOutput
* This initialises the output window stuff.
* Parameters: hInstance is the application instance handle
* Returns:    TRUE for success, FALSE for failure.
************************************************************************/
int InitOutput(HINSTANCE hInstance)
{
  int blErr = FALSE;
  WNDCLASS wndclass;
  
  wndclass.style         = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc   = OutputWndProc;
  wndclass.cbClsExtra    = 0;
  wndclass.cbWndExtra    = 0;
  wndclass.hInstance     = hInstance;
  wndclass.hIcon         = LoadIcon(hInstance,MAKEINTRESOURCE(ICO_OUTPUT));
  wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
  wndclass.lpszMenuName  = NULL;
  wndclass.lpszClassName = szOutputClass;

  if(!RegisterClass(&wndclass))
    {
    AppError(NULL,IDS_ERR_OUTPUT_REGISTER);
    return FALSE;
    }
    
  hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_OUTPUT));
  if(hMenu == NULL)
    {
    AppError(NULL,IDS_ERR_OUTPUT_MENU);
    return FALSE;
    }
  return TRUE;
}

/************************************************************************
* Function:   CreateOutputWindow
* This creates the input window as an MDI child.
* Parameters: hwndClient is the MDI client window handle
* Returns:    The handle of the input window.
************************************************************************/
HWND CreateOutputWindow(HWND hwndClient)
{
#ifdef _WIN32
  hwndOutput = CreateMDIWindow( szOutputClass,
                                "Output",
                                WS_HSCROLL | WS_VSCROLL,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                hwndClient,
                                AppInstance(),
                                0);

#else /* Win16 */
  MDICREATESTRUCT mdicreate;
  
  assert(hwndOutput == NULL);  /* only want 1 output window */
  
  mdicreate.szClass = szOutputClass;
  mdicreate.szTitle = "Output";
  mdicreate.hOwner  = AppInstance();
  mdicreate.x       = CW_USEDEFAULT;
  mdicreate.y       = CW_USEDEFAULT;
  mdicreate.cx      = CW_USEDEFAULT;
  mdicreate.cy      = CW_USEDEFAULT;
  mdicreate.style   = WS_HSCROLL | WS_VSCROLL;
  mdicreate.lParam  = NULL;
  
  hwndOutput = (HWND)(WORD)SendMessage(hwndClient, WM_MDICREATE, 0
                              ,(long)(LPMDICREATESTRUCT)&mdicreate);

#endif
  return hwndOutput;
}

/**********************************************************************
* Function:   OutputKillMenu
* This destroys the output menu IFF it is not the current menu.
* Parameters: hmenuCurrent is the current menu
* Returns:    NOTHING
**********************************************************************/
void OutputKillMenu(HMENU hmenuCurrent)
{
  if(hMenu != hmenuCurrent)
    DestroyMenu(hMenu);
}

/************************************************************************
* Function:   OutputWndProc
* This is the window procedure for the output window.
* Parameters: hwnd is handle of window
*             message is the message ID
*             16 bits of message dependent stuff
*             32 bits of message dependent stuff
* Returns:    Depends of message
************************************************************************/
LRESULT CALLBACK OutputWndProc(HWND hWnd ,UINT message ,WPARAM wParam ,LPARAM lParam)
{
  LRESULT lrRet = 0;

  switch (message)
    {
    HANDLE_MSG(hWnd,WM_CREATE,OnCreate);
    HANDLE_MSG(hWnd,WM_PAINT,OnPaint);
    HANDLE_MSG(hWnd,WM_KEYDOWN,OnKey);
    HANDLE_MSG(hWnd,WM_HSCROLL,OnHScroll);
    HANDLE_MSG(hWnd,WM_VSCROLL,OnVScroll);
    HANDLE_MSG(hWnd,WM_MDIACTIVATE,OnMDIActivate);
    HANDLE_MSG(hWnd,WM_INITMENUPOPUP,OnInitMenuPopup);
    HANDLE_MSG(hWnd,WM_COMMAND,OnCommand);
    HANDLE_MSG(hWnd,WM_DESTROY,OnDestroy);
    
    case WM_SIZE: /* handle as special case: */
      HANDLE_WM_SIZE(hWnd,wParam,lParam,OnSize);
      break;      /* as pass data to DefMDIChildProc */
    
    default:
      break;
    }

  lrRet = DefMDIChildProc(hWnd, message, wParam, lParam);
  return lrRet;

}

/************************************************************************
* Function:   OnCreate   
* Called when the window is created. This creates the edit window
* in the client area.
* Parameters: hwnd is the window handle
*             lpCreateStruct is a pointer to the CREATESTRUCT for this
*             window
* Returns:    TRUE for success, FALSE for failure
************************************************************************/
static BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct)
{ 
  HDC hdc;
  TEXTMETRIC tm;
               
  hdc = GetDC(hwnd);
  
  GetTextMetrics(hdc, &tm);
  cxChar = tm.tmAveCharWidth;
  cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
  cyChar = tm.tmHeight + tm.tmExternalLeading;
  
  ReleaseDC(hwnd, hdc);
  nMaxWidth = 40 * cxChar + 22 * cxCaps;

  return TRUE;
}

/************************************************************************
* Function:   OnPaint   
* Paints the client area of the window
* Parameters: hwnd is the window handle
* Returns:    NOTHING
************************************************************************/
static void OnPaint(HWND hwnd)
{
  HDC hdc;
  int nPaintBeg = 0;
  int nPaintEnd = 0;
  PAINTSTRUCT ps;
  int x,y;
  const char* pszLine = 0;
  int i=0;
  
  hdc = BeginPaint(hwnd, &ps);

  nPaintBeg = max(0, nVscrollPos + ps.rcPaint.top / cyChar);
  nPaintEnd = min(BufferLineCount(), nVscrollPos + ps.rcPaint.bottom / cyChar +1);

  x = cxChar * (nHscrollPos);
  y = cyChar * (nPaintBeg - nVscrollPos);
  for (i = nPaintBeg; i < nPaintEnd; ++i)
    {
    pszLine = BufferedLine(i);
    TextOut(hdc, x, y, pszLine, lstrlen(pszLine));
    y += cyChar;
    }
  EndPaint(hwnd, &ps);
  return;
}

/************************************************************************
* Function:   OnSize
* This is called when the window size changes.  It recalculate the
* scroll parameters
* Parameters: hwnd is the handle of the window
*             state is the new state (e.g. minimised) of the window
*             cx,cy are the new sizes of the window client area
* Returns:    NOTHING
************************************************************************/
static void OnSize(HWND hwnd, UINT state, int cx, int cy)
{
  cxClient = cx;
  cyClient = cy;
  
  nVscrollMax = max(0, BufferLineCount() - cyClient / cyChar);
  nVscrollPos = min(nVscrollPos, nVscrollMax);
  
  SetScrollRange(hwnd, SB_VERT, 0, nVscrollMax, FALSE);
  SetScrollPos(hwnd, SB_VERT, nVscrollPos, TRUE);
  
  nHscrollMax = max(0, 2 + (nMaxWidth - cxClient) / cxChar);
  nHscrollPos = min(nHscrollPos, nHscrollMax);
  
  SetScrollRange(hwnd, SB_HORZ, 0, nHscrollMax, FALSE);
  SetScrollPos(hwnd, SB_HORZ, nHscrollPos, TRUE);
              
  return;              
}

/************************************************************************
* Function:   OnHScroll
* Handles horizontal scroll messages.
* Parameters: hwnd is the window handle.
*             hwndCtrl will be unused as the messages should come from
*             the window's own scroll bars.
*             code gives the scroll request as a SB_ constant
*             pos gives the current position of the thumb for
*             SB_THUMBTRACK and SB_THUMBPOSITION.
* Returns:    NOTHING
************************************************************************/
static void OnHScroll(HWND hwnd, HWND hwndCtrl, UINT code, int pos)
{
  int nHscrollInc = 0;
  switch (code)
    {
    case SB_LINEUP:         nHscrollInc = -1;                 break;
    case SB_LINEDOWN:       nHscrollInc = 1;                  break;
    case SB_PAGEUP:         nHscrollInc = -8;                 break;
    case SB_PAGEDOWN:       nHscrollInc = 8;                  break;
    case SB_THUMBPOSITION:  nHscrollInc = pos - nHscrollPos;  break;
    default:                nHscrollInc = 0;                  break;
    }
    
  nHscrollInc = max(-nHscrollPos, min(nHscrollInc, nHscrollMax - nHscrollPos));
  
  if (nHscrollInc != 0)
    {
    nHscrollPos += nHscrollInc;
    ScrollWindow(hwnd, -cxChar * nHscrollInc, 0, NULL, NULL);
    SetScrollPos(hwnd, SB_HORZ, nHscrollPos, TRUE);
    }
  return;
}

/************************************************************************
* Function:   OnKey
* Handles key codes and converts them into scroll messages.
* Parameters: hwnd is the window handle
*             vk is the key code
*             fDown is the key transition flag: TRUE for KEYDOWN, false
*             for KEYUP
*             cRepeat is the repeat count
*             flags is the key flags word
* Returns:    NOTHING
************************************************************************/
static void OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
  if(fDown)
    {
    switch (vk)
      {
      case VK_HOME:   SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0L);      break;
      case VK_END:    SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0L);   break;
      case VK_PRIOR:  SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0L);   break;
      case VK_NEXT:   SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0L); break;
      case VK_UP:     SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);   break;
      case VK_DOWN:   SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L); break;
      case VK_LEFT:   SendMessage(hwnd, WM_HSCROLL, SB_PAGEUP, 0L);   break;
      case VK_RIGHT:  SendMessage(hwnd, WM_HSCROLL, SB_PAGEDOWN, 0L); break;
      }
    }
  return;
}

/************************************************************************
* Function:   OnVScroll
* Handles vertical scroll messages.
* Parameters: hwnd is the window handle.
*             hwndCtrl will be unused as the messages should come from
*             the window's own scroll bars.
*             code gives the scroll request as a SB_ constant
*             pos gives the current position of the thumb for
*             SB_THUMBTRACK and SB_THUMBPOSITION.
* Returns:    NOTHING
************************************************************************/
static void OnVScroll(HWND hwnd, HWND hwndCtrl, UINT code, int pos)
{
  int nVscrollInc = 0;
  
  switch (code)
    {
    case SB_TOP:        nVscrollInc = -nVscrollPos;                 break;
    case SB_BOTTOM:     nVscrollInc = nVscrollMax - nVscrollPos;    break;
    case SB_LINEUP:     nVscrollInc = -1;                           break;
    case SB_LINEDOWN:   nVscrollInc = 1;                            break;
    case SB_PAGEUP:     nVscrollInc = min(-1, -cyClient / cyChar);  break;
    case SB_PAGEDOWN:   nVscrollInc = max(1, cyClient / cyChar);    break;
    case SB_THUMBTRACK: nVscrollInc = pos - nVscrollPos;            break;
    default:            nVscrollInc = 0;                            break;
    }
    
  nVscrollInc = max(-nVscrollPos, min(nVscrollInc, nVscrollMax - nVscrollPos));
  
  if (nVscrollInc != 0)
    {
    nVscrollPos += nVscrollInc;
    ScrollWindow(hwnd, 0, -cyChar * nVscrollInc, NULL, NULL);
    SetScrollPos(hwnd, SB_VERT, nVscrollPos, TRUE);
    UpdateWindow(hwnd);
    }
  return;
}

/************************************************************************
* Function:   OnInitMenuPopup
* This is called just before any of the popup menus are displayed. It
* handles the enabling/disabling of the menu items.
* Parameters: hwnd is the window handle of the window which owns the menu
*             hMenu is the handle of the menu.
*             item is the index of the popup mwnu
*             fSystemMenu is TRUE iff the system menu is about to be
*             displayed
* Returns:    NOTHING
************************************************************************/
static void OnInitMenuPopup(HWND hwnd, HMENU hMenu, int item, BOOL fSystemMenu)
{
  /* Don't bother with system menus. */
  if(fSystemMenu)
    return;
    
  switch(item) 
    {
    case 0:     /* File Menu */
      /* ID_FILE_OUTPUT */
      EnableMenuItem(hMenu, ID_FILE_INPUT, InputExists() ? MF_GRAYED : MF_ENABLED);
      break;
    }
}

/************************************************************************
* Function:   OnCommand
* This handles any commands sent to the window.
* Parameters: hwnd is the window handle
*             id is the command ID
*             hwndCtl if non-zero specifies the control sending the
*             message.
*             codeNotify is 1 if the message is from a menu, 0 if
*             from an accelerator, the notification message if the
*             message is from a control
* Returns:    NOTHING
************************************************************************/
static void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
  /* handled by frame window */
}

/************************************************************************
* Function:   OnDestroy
* This is called when the input window is destroyed.
* Parameters: hwnd is the window handle
* Returns:    NOTHING
************************************************************************/
static void OnDestroy(HWND hwnd)
{
  hwndOutput = NULL;  
  return;
}

/************************************************************************
* Function:   OnMDIActivate
* 
* Parameters: hwnd is the window handle
*             fActive signals whether the window is being activate (TRUE)
*             or deativeated (FALSE).
*             hwndActivate is the handle of the chile window being
*             activated.
*             hwndDeactivate is the handle of the child window being
*             deactivated.
* Returns:    NOTHING
************************************************************************/
static void OnMDIActivate(HWND hwnd, BOOL fActive, HWND hwndActivate, HWND hwndDeactivate)
{
  if(fActive)
    FrameAttachMenu(hMenu,OUTPUT_MENU_WINDOW_POS);
  else
    FrameRemoveMenu();
  return;

}

