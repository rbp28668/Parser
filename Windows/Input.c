/* 
// File:        INPUT.C
// 
// Prefix:
// 
// Description: Handles the input window
//
*/

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "app.h"
#include "frame.h"
#include "input.h"
#include "output.h"
#include "file.h"
#include "find.h"
#include "resource.h"
#include "prsif.h"

#ifdef _MSC_VER
#pragma warning ( disable : 4100 )  /* unreferenced formal parameter */
#endif

#define EDITID 1    /* This is the ID given to the edit window */


/* Forward declarations for callbacks and message handlers */
LRESULT CALLBACK InputWndProc (HWND hwnd ,UINT message ,WPARAM wParam ,LPARAM lParam);
static BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct);
static void OnSetFocus(HWND hwnd, HWND hwndOldFocus);
static void OnSize(HWND hwnd, UINT state, int cx, int cy);
static void OnInitMenuPopup(HWND hwnd, HMENU hMenu, int item, BOOL fSystemMenu);
static void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
static BOOL OnQueryEndSession(HWND hwnd);
static void OnDestroy(HWND hwnd);
static LRESULT OnFindReplace(HWND hwnd, LPFINDREPLACE lpfr);
static void OnMDIActivate(HWND hwnd, BOOL fActive, HWND hwndActivate, HWND hwndDeactivate);

/* Statics for this window  */
static BOOL blExists = FALSE;           /* True when input window exists */
static HWND hwndEdit = 0;               /* Handle of edit control */
static HMENU hMenu = 0;                 /* Handle of input menu */
static char szInputClass[] = "PFAInput";/* class name for input window */
static char szFileName[64] = "";        /* file name file was loaded from */
static UINT msgFindReplace = 0;         /* message ID for find/replace */
static HWND hDlgModeless = 0;           /* handle for modeless dialogs */
static BOOL blModified = FALSE;         /* Set true if edit has modified text */
static WORD iOffset = 0;                /* of find/replace text */

static int iCurrentLine = 0;            /* Number of lines read from input */

/**********************************************************************
* Function:   InputFunction
* This is the input function which reads text from the input 
* buffer.
* Parameters: szBuff is a line buffer to write a line of text into
*             cbBuff is the buffer size
*
* Returns:    A pointer to the buffer if successfull, NULL if failed
**********************************************************************/
char* InputFunction(char* szBuff, int cbBuff)
{ 
  int nBytes = 0;
  assert(cbBuff > sizeof(WORD)); /* as we stuff length into buffer for EM_GETLINE */

  /* Try and read the current line from the edit window */
  nBytes = Edit_GetLine(hwndEdit,iCurrentLine,szBuff,cbBuff);
  
  /* terminate the string */
  if(nBytes < cbBuff)
    szBuff[nBytes] = '\0';
  else
    szBuff[cbBuff-1] = '\0';
  
  /* Next line */  
  ++iCurrentLine;
  
  /* Check for end of buffer */
  return (nBytes == 0 && iCurrentLine >= Edit_GetLineCount(hwndEdit)) ? NULL : szBuff;  
    
}

/**********************************************************************
* Function:   InputRun
* This runs the interpreter from the input buffer.
* Parameters: NONE
* Returns:    NOTHING
**********************************************************************/
void InputRun(void)
{ 
  int blIsEOF = FALSE;
  BOOL blErr = FALSE;
  int nGoodLines = 0;
  int idx;
  
  iCurrentLine = 0;     /* start at top of edit window */
  PRS_SendCommand("");  /* start with empty line: use callback for
                        // all the input
                        */
  
  while(!blErr && !blIsEOF)
    {
    blErr = PRS_ParseUnit(&blIsEOF);

    /* note where the last good line is, and run the VM if ok */
    if(!blErr)
      {
      nGoodLines = iCurrentLine;
      PRS_StartVM();
      AppMessagePump(); /* keep ticking VM as required*/
      }
    }
  
  /* Set selection up to end of last good line */
  idx = Edit_LineIndex(hwndEdit,nGoodLines);
  Edit_SetSel(hwndEdit,0,idx);
}

/************************************************************************
* Function:   InputExists
* Determines whether there is an input window.
* Parameters: NONE
* Returns:    TRUE if the window exists, FALSE otherwise
************************************************************************/
BOOL InputExists(void)
{
  return blExists;
}



/************************************************************************
* Function:   InitInput
* This initialises the input window stuff.
* Parameters: hInstance is the application instance handle
* Returns:    TRUE for success, FALSE for failure.
************************************************************************/
int InitInput(HINSTANCE hInstance)
{
  int blErr = FALSE;
  WNDCLASS wndclass ;
  
  wndclass.style         = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc   = InputWndProc;
  wndclass.cbClsExtra    = 0;
  wndclass.cbWndExtra    = 0;
  wndclass.hInstance     = hInstance;
  wndclass.hIcon         = LoadIcon(hInstance,MAKEINTRESOURCE(ICO_INPUT));
  wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
  wndclass.lpszMenuName  = NULL;
  wndclass.lpszClassName = szInputClass;

  if(!RegisterClass(&wndclass))
    {
    AppError(NULL,IDS_ERR_INPUT_REGISTER);
    return FALSE;
    }

  hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_INPUT));
  if(hMenu == NULL)
    {
    AppError(NULL,IDS_ERR_INPUT_MENU);
    return FALSE;
    }
  
  return TRUE;
}

/************************************************************************
* Function:   CreateInputWindow
* This creates the input window as an MDI child.
* Parameters: hwndClient is the MDI client window handle
* Returns:    The handle of the input window.
************************************************************************/
HWND CreateInputWindow(HWND hwndClient)
{
#ifndef _WIN32
  MDICREATESTRUCT mdicreate;
#endif

  HWND hwndChild = 0;
  
  assert(hwndClient != NULL);
  assert(!blExists);
  
#ifdef _WIN32
  hwndChild = CreateMDIWindow( szInputClass,
                                "Command",
                                0,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                hwndClient,
                                AppInstance(),
                                0);

#else

  mdicreate.szClass = szInputClass;
  mdicreate.szTitle = "Command";
  mdicreate.hOwner  = AppInstance();
  mdicreate.x       = CW_USEDEFAULT;
  mdicreate.y       = CW_USEDEFAULT;
  mdicreate.cx      = CW_USEDEFAULT;
  mdicreate.cy      = CW_USEDEFAULT;
  mdicreate.style   = 0;
  mdicreate.lParam  = NULL;
  
  hwndChild = (HWND)(WORD)SendMessage(hwndClient, WM_MDICREATE, 0
                              ,(long)(LPMDICREATESTRUCT)&mdicreate);
                              
#endif
  if(hwndChild != NULL)
    blExists = TRUE;
  else
    AppError(NULL,IDS_ERR_INPUT_CREATE);
    
  return hwndChild;
}

/**********************************************************************
* Function:   InputKillMenu
* This destroys the input menu IFF it is not the current menu.
* Parameters: hmenuCurrent is the current menu
* Returns:    NOTHING
**********************************************************************/
void InputKillMenu(HMENU hmenuCurrent)
{
  if(hMenu != hmenuCurrent)
    DestroyMenu(hMenu);
}

/************************************************************************
* Function:   InputWndProc
* This is the window procedure for the input window.
* Parameters: hwnd is handle of window
*             message is the message ID
*             16 bits of message dependent stuff
*             32 bits of message dependent stuff
* Returns:    Depends of message
************************************************************************/
LRESULT CALLBACK InputWndProc(HWND hWnd ,UINT message ,WPARAM wParam ,LPARAM lParam)
{
  LRESULT lrRet = 0;

  switch (message)
    {
    HANDLE_MSG(hWnd,WM_CREATE,OnCreate);
    HANDLE_MSG(hWnd,WM_SETFOCUS,OnSetFocus);
    HANDLE_MSG(hWnd,WM_MDIACTIVATE,OnMDIActivate);
    HANDLE_MSG(hWnd,WM_INITMENUPOPUP,OnInitMenuPopup);
    HANDLE_MSG(hWnd,WM_COMMAND,OnCommand);
    HANDLE_MSG(hWnd,WM_QUERYENDSESSION,OnQueryEndSession);
    HANDLE_MSG(hWnd,WM_DESTROY,OnDestroy);
    
    case WM_SIZE: /* handle as special case: */
      HANDLE_WM_SIZE(hWnd,wParam,lParam,OnSize);
      break;      /* as pass data to DefMDIChildProc */
    
    default:
      if(message == msgFindReplace)
        {
        OnFindReplace(hWnd, (LPFINDREPLACE)lParam);
        return 0;
        }
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
* Returns:    TRUE for success, FALSE for failure.
************************************************************************/
static BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct)
{
  TEXTMETRIC tm;
  HDC hdc;
  WORD wUnits;
  HFONT hFont = NULL;
  
  /* Create the edit control child window */
  hwndEdit = CreateWindow ("edit", NULL
                              ,WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL
                              | WS_BORDER | ES_LEFT | ES_MULTILINE
                              | ES_NOHIDESEL | ES_AUTOHSCROLL | ES_AUTOVSCROLL
                              ,0, 0, 0, 0
                              ,hwnd, (HMENU)EDITID, AppInstance(), NULL) ;

  if(hwndEdit == NULL)
    {
    AppError(hwnd,IDS_ERR_INPUT_CREATE );
    return FALSE; /* failed */
    }
    
  Edit_LimitText(hwndEdit,32000);
  
  /* Now make sure the edit box has a fixed pitch font */
  hFont = (HFONT)GetStockObject(ANSI_FIXED_FONT);
  SendMessage(hwndEdit,WM_SETFONT,(WPARAM)hFont,(LPARAM)0);
  
  /* Sort out tab stops for the dialog box */
  /* These are set in Dialog Units (DUs) so need to
  // do a bit of mucking about to get and convert
  // the average character width to DUs
  */
               
  hdc = GetDC(hwnd);
  
  GetTextMetrics(hdc, &tm);
  wUnits = LOWORD(GetDialogBaseUnits()); /* x pixels in 1 du * 4 */
  wUnits = 4 * tm.tmAveCharWidth / wUnits;  /* ave width of char in DUs */
  wUnits *= 2;  /* tab stops on 2 char bdys */
  
  Edit_SetTabStops(hwndEdit,1,&wUnits);
  
  ReleaseDC(hwnd, hdc);
  
  /* Initialize common dialog box stuff */
 
  FileInitialize(hwnd) ;
  msgFindReplace = RegisterWindowMessage(FINDMSGSTRING) ;
 
  return TRUE; /* ok */  
}

/************************************************************************
* Function:   OnSetFocus
* This is called when the window receives the focus
* Parameters: hwnd is the window handle
*             hwndOldFocus is the handle of the window which is losing
*             the focus.
* Returns:    NOTHING
************************************************************************/
static void OnSetFocus(HWND hwnd, HWND hwndOldFocus)                  
{ 
  assert(hwndEdit != NULL);
  SetFocus(hwndEdit) ;
  return;
}

/************************************************************************
* Function:   OnSize
* This is called when the window size changes.  It updates the size
* of the child edit window to fill the client area.
* Parameters: hwnd is the handle of the window
*             state is the new state (e.g. minimised) of the window
*             cx,cy are the new sizes of the window client area
* Returns:    NOTHING
************************************************************************/
static void OnSize(HWND hwnd, UINT state, int cx, int cy)
{
  assert(hwndEdit != NULL);
  MoveWindow(hwndEdit, 0, 0, cx,cy, TRUE) ;
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
  WORD wEnable = FALSE;
  DWORD dwSelect = 0;
  
  /* Don't bother with system menus. */
  if(fSystemMenu)
    return;
    
  switch(item) 
    {
    case 0:     /* File Menu */
      /* ID_FILE_OUTPUT */
      EnableMenuItem(hMenu, ID_FILE_OUTPUT, OutputExists() ? MF_GRAYED : MF_ENABLED);
      
      /* ID_FILE_OPEN */
      /* ID_FILE_CLOSE */
      /* ID_FILE_SAVEAS */
      EnableMenuItem(hMenu, ID_FILE_SAVEAS
        ,Edit_GetModify(hwndEdit) ? MF_ENABLED : MF_GRAYED);
      
      /* ID_FILE_LOAD */
      /* ID_FILE_EXIT */
      break;
      
    case 1:     /* Edit menu */
      /* ID_EDIT_UNDO: Enable Undo if edit control can do it */
      EnableMenuItem (hMenu, ID_EDIT_UNDO
        ,Edit_CanUndo(hwndEdit) ? MF_ENABLED : MF_GRAYED) ;
      
      /*ID_EDIT_CUT. ID_EDIT_COPY, ID_EDIT_DELETE: Enable if text selected */
      dwSelect = Edit_GetSel(hwndEdit);
      wEnable = (WORD)((HIWORD(dwSelect) != LOWORD(dwSelect)) ? MF_ENABLED : MF_GRAYED);
      
      EnableMenuItem(hMenu, ID_EDIT_CUT,  wEnable);
      EnableMenuItem(hMenu, ID_EDIT_COPY, wEnable);
      EnableMenuItem(hMenu, ID_EDIT_DELETE,  wEnable);

      
      /* ID_EDIT_PASTE: Enable if text is in the clipboard */
      EnableMenuItem(hMenu, ID_EDIT_PASTE
              ,IsClipboardFormatAvailable (CF_TEXT) 
              ? MF_ENABLED : MF_GRAYED);
      
      break;                                                           

    case 2:     /* Search menu */
      /* Enable Find, Next, and Replace if modeless
      * dialogs are not already active */
      wEnable = (WORD)((hDlgModeless == NULL) ? MF_ENABLED : MF_GRAYED);
      
      EnableMenuItem(hMenu, ID_SEARCH_FIND,    wEnable);
      EnableMenuItem(hMenu, ID_SEARCH_FINDNEXT,wEnable);
      EnableMenuItem(hMenu, ID_SEARCH_REPLACE, wEnable);
      break;
    
    case 3: /* Run menu */
      /* ID_RUN_RUN: Can only run if there is something in the buffer to run */ 
      wEnable = MF_GRAYED;
      
      if(!AppProcessing()) /* don't start new task while old one running*/
        {
        if(Edit_GetLineCount(hwndEdit) > 1)
          wEnable = MF_ENABLED;
        else
          {
          /* See if there are any characters on the line */
          char szBuff[16];
          if(Edit_GetLine(hwndEdit,0,szBuff,sizeof(szBuff)) > 0)
            wEnable = MF_ENABLED;
          }
        }            
      EnableMenuItem(hMenu, ID_RUN_RUN, wEnable);
      
      /* ID_RUN_PAUSE */
      wEnable = (WORD) (AppIsPaused() ? MF_CHECKED : MF_UNCHECKED);
      wEnable |= MF_BYCOMMAND;
      CheckMenuItem(hMenu, ID_RUN_PAUSE, wEnable);
      
      wEnable = (WORD)((AppProcessing()) ? MF_ENABLED : MF_GRAYED);
      EnableMenuItem(hMenu, ID_RUN_PAUSE, wEnable);
      
      /* ID_RUN_BREAK */
      wEnable = (WORD)((AppProcessing()) ? MF_ENABLED : MF_GRAYED);
      EnableMenuItem(hMenu, ID_RUN_BREAK, wEnable);
      break;
            
    case 4: /* Window menu */
      break;
    }
  return;
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
  /* Handle notification messages from the edit control */
  if (hwndCtl && id == EDITID)
    {
    switch(codeNotify)
      {
      case EN_UPDATE:
        blModified = TRUE ;
        return;
        
      case EN_ERRSPACE:
      case EN_MAXTEXT:
        AppError(hwnd,IDS_ERR_INPUT_EDIT_SPACE);
        return;
      }
    return;
    }
  
  switch(id)
    {
    /* File menu */
    case ID_FILE_OPEN:
      if(FileOpenDlg(hwnd, szFileName))
        {
        if(!FileRead(hwndEdit, szFileName))
          {
          AppError(hwnd,IDS_ERR_INPUT_FILE_READ);
          szFileName[0] = '\0' ;
          }
        }
  
      blModified = FALSE;
      return;
      
    case ID_FILE_CLOSE:
     /* handled by frame */
     return;
     
    case ID_FILE_SAVEAS:
      if(FileSaveDlg(hwnd, szFileName))
        {
        if(FileWrite (hwndEdit, szFileName))
          {
          blModified = FALSE ;
          return;
          }
        else
          AppError(hwnd,IDS_ERR_INPUT_SAVE_FAIL);
        }
      return;
      
    case ID_FILE_LOAD:
      {
      char szFileName[256];
      strcpy(szFileName,"*.PAS");
      if(FileOpenDlg(hwnd, szFileName))
        {
        if(PRS_LoadFile(szFileName))
          {
          AppError(hwnd,IDS_ERR_INPUT_FILE_LOAD);
          }
        }
      return;
      }
    case ID_FILE_EXIT: /* handled by frame */ return;
    
    /* Edit menu */
    case ID_EDIT_UNDO:      Edit_Undo(hwndEdit);                    return;
    case ID_EDIT_CUT:       SendMessage(hwndEdit, WM_CUT, 0, 0L);   return;
    case ID_EDIT_COPY:      SendMessage(hwndEdit, WM_COPY, 0, 0L);  return;
    case ID_EDIT_PASTE:     SendMessage(hwndEdit, WM_PASTE, 0, 0L); return;
    case ID_EDIT_DELETE:    SendMessage(hwndEdit, WM_CLEAR, 0, 0L); return;
    case ID_EDIT_SELECTALL: Edit_SetSel(hwndEdit, 0, 32767);        return;
    
    /* Search menu */
    case ID_SEARCH_FIND:
      assert(hDlgModeless ==0);
      iOffset = HIWORD(Edit_GetSel(hwndEdit));
      hDlgModeless = FindFindDlg(hwnd) ;
      return;

    case ID_SEARCH_FINDNEXT:
      assert(hDlgModeless == 0);
      iOffset = HIWORD(Edit_GetSel(hwndEdit));
  
      if(FindValidFind())
        FindNextText(hwndEdit, &iOffset) ;
      else
        hDlgModeless = FindFindDlg(hwnd) ;
      return;
    
    case ID_SEARCH_REPLACE:
      assert(hDlgModeless == 0);
      iOffset = HIWORD(Edit_GetSel(hwndEdit));
      hDlgModeless = FindReplaceDlg(hwnd) ;
      return;
    
    /* Run menu */
    case ID_RUN_RUN:
      assert(!AppProcessing());
      InputRun();  
      return;
      
    case ID_RUN_PAUSE:
      assert(AppProcessing());
      AppPause(!AppIsPaused());
      return;
      
    case ID_RUN_BREAK:
      assert(AppProcessing());
      PRS_StopVM(); /* will terminate VM & hence message pump */
      return;
    
      
    /* Window menu */
    case ID_WINDOW_CASCADE:   /* handled by frame */ return;
    case ID_WINDOW_TILE:      /* handled by frame */ return;
    case ID_WINDOW_ARRANGEICONS: /* handled by frame */ return;
    case ID_WINDOW_CLOSEALL:  /* handled by frame */ return;
    
    /* Help menu */
    case ID_HELP_ABOUT:       /* handled by frame */ return;
    }
  return;
}

/************************************************************************
* Function:   OnQueryEndSession
* This is called if someone has asked windows to shut down.
* Parameters: hwnd is the window handle
* Returns:    TRUE if windows can terminate, FALSE otherwise.
************************************************************************/
static BOOL OnQueryEndSession(HWND hwnd)
{
  return TRUE; /*  let windows close */
}

/************************************************************************
* Function:   OnDestroy
* This is called when the input window is destroyed.
* Parameters: hwnd is the window handle
* Returns:    NOTHING
************************************************************************/
static void OnDestroy(HWND hwnd)
{
  blExists = FALSE;
  return;
}

/************************************************************************
* Function:   OnFindReplace
* Handles find and replace messages.
* Parameters: hWnd is the window handle
*             lpfr is a pointer to a find/replace structure
* Returns:    0;
************************************************************************/
static LRESULT OnFindReplace(HWND hWnd, LPFINDREPLACE lpfr)
{
  if(lpfr->Flags & FR_DIALOGTERM)
       hDlgModeless = NULL ;

  if(lpfr->Flags & FR_FINDNEXT)
    if(!FindFindText(hwndEdit, &iOffset, lpfr))
      AppWarning(hWnd,IDS_ERR_INPUT_TEXT_NOT_FOUND);     

  if (lpfr->Flags & FR_REPLACE ||
      lpfr->Flags & FR_REPLACEALL)
    if(!FindReplaceText (hwndEdit, &iOffset, lpfr))
      AppWarning(hWnd,IDS_ERR_INPUT_TEXT_NOT_FOUND);     

  if (lpfr->Flags & FR_REPLACEALL)
    while (FindReplaceText(hwndEdit, &iOffset, lpfr));
 
 return 0;
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
    FrameAttachMenu(hMenu,INPUT_MENU_WINDOW_POS);
  else
    FrameRemoveMenu();
  return;
}
