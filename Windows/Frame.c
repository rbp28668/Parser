#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <assert.h>
#include "resource.h"
#include "frame.h"
#include "input.h"
#include "output.h"
#include "app.h"


static HWND hwndFrame = 0;          /* window handle for the frame window */
static HWND hwndClient = 0;         /* and its child client window */
static HMENU hFrameMenu = 0;        /* initial menu */
static char szFrameClass[] = "PFAFrame";

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct);
static BOOL OnCommand(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static void OnInitMenuPopup(HWND hwnd, HMENU hMenu, int item, BOOL fSystemMenu);
static BOOL OnQueryEndSession(HWND hwnd);
static void OnDestroy(HWND hwnd);
BOOL CALLBACK CloseEnumProc(HWND hwnd, LPARAM lParam);

/************************************************************************
* Function:   InitFrame
* This initialises the frame window stuff.
* Parameters: hInstance is the application instance handle
* Returns:    TRUE for success, FALSE for failure.
************************************************************************/
int InitFrame(HINSTANCE hInstance)
{
  WNDCLASS wndclass;

  wndclass.style         = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc   = FrameWndProc;
  wndclass.cbClsExtra    = 0;
  wndclass.cbWndExtra    = 0;
  wndclass.hInstance     = hInstance;
  wndclass.hIcon         = LoadIcon(hInstance,MAKEINTRESOURCE(ICO_MAIN));
  wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
  wndclass.lpszMenuName  = NULL ;
  wndclass.lpszClassName = szFrameClass ;

  if(!RegisterClass(&wndclass))
    {
    AppError(NULL,IDS_ERR_FRAME_REGISTER);
    return FALSE;  /* failed */
    }

  hFrameMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_FRAME)) ;
  if(hFrameMenu == NULL)
    {
    AppError(NULL,IDS_ERR_FRAME_NULL_MENU);
    return FALSE;
    }

  return TRUE;  
}


/************************************************************************
* Function:   CreateFrame
* This draws the frame window.
* Parameters: hInstance is the application instance handle
* Returns:    The frame window handle
************************************************************************/
HWND CreateFrame(HINSTANCE hInstance)
{
  assert(hwndFrame == 0); /* can't create more than 1 */
  hwndFrame = CreateWindow(szFrameClass, "PFA Demonstration",
                           WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                           CW_USEDEFAULT, CW_USEDEFAULT,
                           CW_USEDEFAULT, CW_USEDEFAULT,
                           NULL, hFrameMenu, hInstance, NULL);

  if(hwndFrame == NULL)
    {
    AppError(NULL,IDS_ERR_FRAME_CREATE);
    }
    
  /* hwndClient is set up by OnCreate */
  return hwndFrame;
}

/************************************************************************
* Function:   ShowFrame
* This shows the frame window
* Parameters: nCmdShow determines how the window should show itself.
* Returns:    NOTHING
************************************************************************/
void ShowFrame(int nCmdShow)
{
  assert(hwndFrame != NULL);
  ShowWindow(hwndFrame, nCmdShow);
  UpdateWindow(hwndFrame);
}

/************************************************************************
* Function:   FrameClientWindow
* This gets the handle of the frame's client window
* Parameters: NONE
* Returns:    The client window handle
************************************************************************/
HWND FrameClientWindow(void)
{
  assert(hwndClient != NULL);
  return hwndClient;
}

/************************************************************************
* Function:   FrameAttachMenu
* This attaches the menus to the main frame window
* Parameters: hmenu is the menu to attach
*             iWindowMenuPos is the index of the Window popup menu (so that the
*             MDI stuff can attach a list of current windows)
* Returns:    NOTHING
************************************************************************/
void FrameAttachMenu(HMENU hmenu, int iWindowMenuPos)
{
  assert(hmenu != NULL);
  assert(hwndClient != NULL);
  assert(hwndFrame != NULL);

#ifdef _WIN32
  SendMessage(hwndClient, WM_MDISETMENU,hmenu,GetSubMenu(hmenu,iWindowMenuPos)); 
#else /* win16*/
  SendMessage(hwndClient, WM_MDISETMENU, 0, MAKELPARAM(hmenu,GetSubMenu(hmenu,iWindowMenuPos)));
#endif

  DrawMenuBar(hwndFrame);
  return;
}

/************************************************************************
* Function:   FrameRemoveMenu
* This un-attaches the child window menus from the frame window and 
* restores the initial menus.
* Parameters: NONE
* Returns:    NOTHING
************************************************************************/
void FrameRemoveMenu(void)
{
  assert(hwndClient != NULL);
  assert(hwndFrame != NULL);
  assert(hFrameMenu  != NULL);
#ifdef _WIN32
  SendMessage(hwndClient, WM_MDISETMENU, hFrameMenu, GetSubMenu(hFrameMenu,0));
#else
  SendMessage(hwndClient, WM_MDISETMENU, 0, MAKELPARAM(hFrameMenu,GetSubMenu(hFrameMenu,0)));
#endif
  DrawMenuBar(hwndFrame) ;
}

/**********************************************************************
* Function:   FrameKillMenu
* This destroys the frame menu IFF it is not the current menu.
* Parameters: hmenuCurrent is the current menu
* Returns:    NOTHING
**********************************************************************/
static void FrameKillMenu(HMENU hmenuCurrent)
{
  if(hFrameMenu != hmenuCurrent)
    DestroyMenu(hFrameMenu);
}

/************************************************************************
* Function:   FrameClass
* Gets a pointer to the string which gives the frame class
* Parameters: NONE
* Returns:    The frame class
************************************************************************/
LPCSTR FrameClass(void)
{
  return szFrameClass;
}
 
/**********************************************************************
* Function:   FramePostMessage
* Posts a message to the frame window
* Parameters: message, wParam, lParam as per standard window proc
* Returns:    NOTHING
**********************************************************************/
void FramePostMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
  PostMessage(hwndFrame,message,wParam,lParam);
} 

/************************************************************************
* Function:   FrameWndProc
* This is the window proc for the MDI frame window
* Parameters: hwnd is the window handle
*             message is the message ID
*             wParam and lParam are 16 and 32 bit parameters for the
*             message
* Returns:    depends on message
************************************************************************/
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch(message)
    {
    HANDLE_MSG(hwnd,WM_CREATE,OnCreate);
    HANDLE_MSG(hwnd,WM_DESTROY,OnDestroy);
    HANDLE_MSG(hwnd,WM_INITMENUPOPUP,OnInitMenuPopup);
    
    case WM_QUERYENDSESSION:
    case WM_CLOSE:
      if(!OnQueryEndSession(hwnd))
        return 0;
      break;

    case WM_COMMAND:
      if(!OnCommand(hwnd,message,wParam,lParam))
        return 0;
      break;
    }
  return DefFrameProc(hwnd, hwndClient, message, wParam, lParam);
}    

/************************************************************************
* Function:   OnCreate
* This handles the creation of the client window.
* Parameters: hwnd is the handle of the frame window
*             lpCreateStruct is a pointer to a structure containing
*             the window creation parameters.
* Returns:    TRUE for success, FALSE for failure
************************************************************************/
static BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct)
{
  CLIENTCREATESTRUCT clientcreate;
  
  assert(hwnd != NULL);
  assert(lpCreateStruct != NULL);
  assert(hFrameMenu != NULL);
  
  clientcreate.hWindowMenu  = GetSubMenu(hFrameMenu,0);
  clientcreate.idFirstChild = ID_FIRST_CHILD ;
  hwndClient = CreateWindow("MDICLIENT", NULL,
                              WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
                              0, 0, 0, 0, hwnd, (HMENU)1, AppInstance(),
                              (LPSTR) &clientcreate) ;
  if(hwndClient)
    return TRUE; /* ok */
  else
    {
    AppError(hwnd,IDS_ERR_FRAME_CREATE_CLIENT);
    return FALSE; /* fail create */
    }
}

/************************************************************************
* Function:   OnCommand
* Handles frame window commands. Note that this is different from the
* normal OnCommand as this needs to pass some of the commands to
* DefFrameProc.
* Parameters: 
* Returns:    TRUE if the message needs to be passed on to DefFrameProc.
************************************************************************/
static BOOL OnCommand(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HWND hwndChild = (HWND)0;
  WNDENUMPROC lpfnEnum = 0;
  
  switch(wParam)
    {
    case ID_FILE_INPUT:      /* Create an input child window */
      assert(hwndClient != NULL);
      CreateInputWindow(hwndClient);
      return FALSE;
    
    case ID_FILE_OUTPUT:      /* create an output child window */
      assert(hwndClient != NULL);
      CreateOutputWindow(hwndClient);
      return FALSE;

    case ID_FILE_CLOSE:         /* Close the active window */
      assert(hwndClient != NULL);
      hwndChild = (HWND)LOWORD(SendMessage(hwndClient, WM_MDIGETACTIVE, 0, 0L));
      if(hwndChild != NULL)
        {
        if(SendMessage(hwndChild, WM_QUERYENDSESSION, 0, 0L))
          SendMessage(hwndClient, WM_MDIDESTROY, (WPARAM)hwndChild, 0L);
        }
      return FALSE;
        
    case ID_FILE_EXIT:          /* Exit the program */
      SendMessage(hwnd, WM_CLOSE, 0, 0L);
      return FALSE;

    /* Messages for arranging windows */
    case ID_WINDOW_TILE:
      SendMessage(hwndClient, WM_MDITILE, 0, 0L);
      return FALSE;

    case ID_WINDOW_CASCADE:
      SendMessage(hwndClient, WM_MDICASCADE, 0, 0L);
      return FALSE;

    case ID_WINDOW_ARRANGEICONS:
      SendMessage(hwndClient, WM_MDIICONARRANGE, 0, 0L);
      return FALSE;

    case ID_WINDOW_CLOSEALL:      /* Attempt to close all children */
      lpfnEnum = (WNDENUMPROC)MakeProcInstance((FARPROC)CloseEnumProc,AppInstance());
      EnumChildWindows(hwndClient, lpfnEnum, 0L);
      FreeProcInstance((FARPROC)lpfnEnum);
      return FALSE;
    
    case ID_HELP_ABOUT:     /* help/about: handle of behalf of all children */
      hwndChild = (HWND)LOWORD(SendMessage(hwndClient, WM_MDIGETACTIVE, 0, 0L));
      if(!IsWindow(hwndChild))
        hwndChild = hwndFrame;
      AppAbout(hwndChild);
      return FALSE;
    
    default:           /* Pass to active child */
      hwndChild = (HWND)LOWORD(SendMessage(hwndClient, WM_MDIGETACTIVE, 0, 0L));

      if(IsWindow(hwndChild))
        SendMessage(hwndChild, WM_COMMAND, wParam, lParam);
      break; 
    }
    
  return TRUE;    /* caller should call DefFrameProc */
}

/************************************************************************
* Function:   OnInitMenuPopup
* This forwards the initmenupopup message to the active window
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
  HWND hwndChild;
  hwndChild = (HWND)LOWORD(SendMessage(hwndClient, WM_MDIGETACTIVE, 0, 0L));

  if(IsWindow(hwndChild))
     SendMessage(hwndChild, WM_INITMENUPOPUP, (WPARAM)hMenu, MAKELPARAM(item,fSystemMenu));
  
}
/************************************************************************
* Function:   OnQueryEndSession
* Another slightly non-standard handler.  This sends the frame window
* a message to close all the child windows.  It then gets the first
* child of the client window: if that is non-null then one of the
* child windows didn't close. 
* Parameters: hwnd is the window handle
* Returns:    FALSE if handled, TRUE if DefFrameProc should be called.
************************************************************************/
static BOOL OnQueryEndSession(HWND hwnd)
{
  SendMessage(hwnd, WM_COMMAND, ID_WINDOW_CLOSEALL, 0L) ;
  if (NULL != GetWindow(hwndClient, GW_CHILD))
    return FALSE;
  else
    return TRUE;
}

/************************************************************************
* Function:   OnDestroy
* This is called when the frame window is destroyed.  It gets the
* system to shut down.
* Parameters: hwnd is the frame window handle
* Returns:    NOTHING
************************************************************************/
static void OnDestroy(HWND hwnd)
{
  HMENU hCurrentMenu = GetMenu(hwnd);
  
  /* Remove the menus (if they are not current) */
  FrameKillMenu(hCurrentMenu);
  InputKillMenu(hCurrentMenu);
  OutputKillMenu(hCurrentMenu);
  
  PostQuitMessage(0);
}

/************************************************************************
* Function:   CloseEnumProc
* This runs is called for all the client's child windows to see if 
* they can close. If so, they are destroyed.
* Parameters: hwnd is the window handle
*             lParam is not used
* Returns:    1 
************************************************************************/
BOOL CALLBACK CloseEnumProc(HWND hwnd, LPARAM lParam)
{
  if(GetWindow(hwnd, GW_OWNER))         /* check for icon title */
    return 1;
  
  /* Ask the client to restore this window */
  SendMessage(GetParent(hwnd), WM_MDIRESTORE, (WPARAM)hwnd, 0L);
  
  /* Check if it can end */
  if(!SendMessage(hwnd, WM_QUERYENDSESSION, 0, 0L))
    return 1;

  /* if not ask client to destroy it */
  SendMessage(GetParent(hwnd), WM_MDIDESTROY, (WPARAM)hwnd, 0L);
    return 1;
}

