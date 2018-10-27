#define STRICT
#include <windows.h>
#include <windowsx.h> 
#include <string.h>
#include <assert.h>
#include "resource.h"
#include "prsif.h"
#include "frame.h"
#include "input.h"
#include "output.h"
#include "error.h"
#include "app.h"
#include "buffer.h"
#include "pdrive.h"
      
#ifdef _MSC_VER
#pragma warning ( disable : 4100 )  /* unreferenced formal parameter */
#endif
      
#define UNSPECIFIED "Unspecified error"

static HANDLE hInst = 0;
static char szAppName[] = "PFA";
static HACCEL hAccel;
static HWND hwndFrame = NULL;
static BOOL blRunning = FALSE;
static BOOL blPaused = FALSE;

/************************************************************************
* Function:   AppInstance
* This gets the application instance
* Parameters: NONE
* Returns:    the application instance
************************************************************************/
HANDLE AppInstance(void)
{
  return hInst;
}

/************************************************************************
* Function:   AppName
* This gets a pointer to the application name
* Parameters: NONE
* Returns:    a pointer to the application name
************************************************************************/
LPCSTR AppName(void)
{
  return szAppName;
}

/************************************************************************
* Function:   AppError
* This puts up a error message
* Parameters: hwnd is the calling window (or null)
*             uires is the message resource index
* Returns:    NOTHING
************************************************************************/
void AppError(HWND hwnd,UINT uires)
{
  char szBuff[128];
  if(LoadString(hInst,uires,szBuff,sizeof(szBuff)) == 0)
    strcpy(szBuff,UNSPECIFIED);
  MessageBox(hwnd,szBuff,AppName(),MB_OK | MB_ICONSTOP);
}

/************************************************************************
* Function:   AppWarning
* This puts up a warning message
* Parameters: hwnd is the calling window (or null)
*             uires is the message resource index
* Returns:    NOTHING
************************************************************************/
void AppWarning(HWND hwnd,UINT uires)
{
  char szBuff[128];
  if(LoadString(hInst,uires,szBuff,sizeof(szBuff)) == 0)
    strcpy(szBuff,UNSPECIFIED);
  MessageBox(hwnd,szBuff,AppName(),MB_OK | MB_ICONEXCLAMATION);
}

/************************************************************************
* Function:   AppAbout
* This puts up a message box with the program details
* Parameters: hwnd is the calling window (or NULL)
* Returns:    NOTHING
************************************************************************/
void AppAbout(HWND hwnd)
{
  MessageBox(hwnd
    ,"Pascal For Applications\r"
     "Demonstration Application\r"
     "(C) Cambridge Cybernetics Ltd 1995,1996\r"
     "Interpreter version 1.2"
    ,AppName(),MB_OK | MB_ICONINFORMATION);
} 

/************************************************************************
* Function:   WinMain
* This is the main entry point for the program
* Parameters: hInstance is the application instance
*             hPrevInstance is the previous instance
*             lpszCmdLine is a pointer to the command line
*             nCmdShow is the state the window should be drawn in
* Returns:    0
************************************************************************/
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszCmdLine, int nCmdShow)
{
  MSG msg;
  BOOL blFail = FALSE;
  PFSIS oldipfn = NULL;
  PFCSV oldopfn = NULL;
  PFCSV olderrfn = NULL; 

  if(hPrevInstance)
    {
    /* Bring previous instance to top */
    HWND FirsthWnd, FirstChildhWnd;
    FirsthWnd = FindWindow(FrameClass(),NULL);
   
    FirstChildhWnd = GetLastActivePopup(FirsthWnd);
    BringWindowToTop(FirsthWnd);         /* bring main window to top */
 
    if (FirsthWnd != FirstChildhWnd)
      BringWindowToTop(FirstChildhWnd); /* a pop-up window is active
                                        *  bring it to the top too */
    return 0;
    }
  
  /* Remember handle so AppInstance can get at it */
  hInst = hInstance;
  
  /* Set up the output buffer */
  if(!InitOutputBuffer())
    {
    AppError(NULL,IDS_ERR_MAIN_OUTPUT_BUFFER);
    blFail = TRUE;
    }

  /* Do all the interpreter initialising stuff */  
  if(!blFail)
    {
    if(PRS_InitialiseInterpreter(hInstance))
      {
      AppError(NULL,IDS_ERR_MAIN_INIT_INTERP);
      blFail = TRUE;
      }
    }  
  
  if(!blFail)
    {    
    oldopfn = PRS_SetOutputFunction(OutputFunction);
    olderrfn = PRS_SetErrorHandler(ErrorFunction);
    oldipfn = PRS_InputFunction(InputFunction);

    SetupParserExtensions();
    SetupStringExtensions();
    }
  
  /* Register all the window classes */
  if(!(InitFrame(hInstance)
    && InitInput(hInstance)
    && InitOutput(hInstance)))
    blFail = TRUE;

  if(!blFail)
    {
    hAccel = LoadAccelerators(hInstance,"PFAAccel");
    if(hAccel == NULL)
      AppWarning(NULL,IDS_ERR_MAIN_ACCEL);
    }
  
  if(!blFail)
    {    
    hwndFrame = CreateFrame(hInstance);
    if(hwndFrame == NULL)
      blFail = TRUE;
    }
  
  /* Show the window : but maximise if not
  // minimised.
  */   
  if(!blFail)
    {
    if(nCmdShow == SW_SHOWNORMAL)
      nCmdShow = SW_SHOWMAXIMIZED;
    ShowFrame(nCmdShow);
    }
    
  /* set up initial windows */
  if(!blFail)
    {
    PostMessage(hwndFrame,WM_COMMAND,ID_FILE_OUTPUT,0L);
    PostMessage(hwndFrame,WM_COMMAND,ID_FILE_INPUT,0L);
    PostMessage(hwndFrame,WM_COMMAND,ID_WINDOW_TILE,0L);
    }
    
  /* If everythink ok, then enter main message loop */
  if(!blFail)
    {
    while(GetMessage (&msg, NULL, 0, 0))
      {
      if(!TranslateMDISysAccel(FrameClientWindow(),&msg)
         && !TranslateAccelerator(hwndFrame,hAccel,&msg))
        {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        }
      }
    }
  
  PRS_InputFunction(oldipfn); /* restore old input function */
  PRS_SetOutputFunction(oldopfn);
  PRS_SetErrorHandler(olderrfn);

  PRS_TerminateInterpreter();
  
  DeleteOutputBuffer();
  
  if(blFail)
    return 0;
  else
    return msg.wParam;
}

/**********************************************************************
* Function:   AppMessagePump
* This is a secondary message pump which will process all the messages
* and return, either when the system gets a WM_QUIT or if the VM
* stops running.
* Parameters: NONE
* Returns:    NOTHING
**********************************************************************/
void AppMessagePump(void)
{
  MSG msg;
  assert(blRunning == FALSE);
  
  blRunning = TRUE;
  while(PRS_VMRunning())
    {
    /* Process all messages except WM_QUIT */
    while(PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
      {
      if(msg.message == WM_QUIT)
        break;
      else
        GetMessage(&msg, NULL, 0, 0);

      if(!TranslateMDISysAccel(FrameClientWindow(),&msg)
         && !TranslateAccelerator(hwndFrame,hAccel,&msg))
        {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        }
      }
      
    if(msg.message == WM_QUIT)
      break;

    /* Now Tick the VM but only if nothing has stopped it in the message loop */
    if(blPaused)
      WaitMessage();
    else
      {    
      if(PRS_VMRunning()) 
        PRS_TickVM();
      else
        break;
      }
    }
  blRunning = FALSE;
  return;
}

/**********************************************************************
* Function:   AppProcessing
* This allows other code to detemine whether the secondary message
* pump (i.e. the interpreter) is processing.
* Parameters: NONE
* Returns:    TRUE if processing, FALSE if not
**********************************************************************/
BOOL AppProcessing(void)
{
  return blRunning;
}

/**********************************************************************
* Function:   AppPause
* This pauses the interpreter
* Parameters: blYes pauses if TRUE resumes if FALSE
* Returns:    NOTHING
**********************************************************************/
void AppPause(BOOL blYes)
{
  blPaused = blYes;
}

/**********************************************************************
* Function:   AppIsPaused
* Determines whether the interpreter is paused
* Parameters: NONE
* Returns:    NOTHING
**********************************************************************/
BOOL AppIsPaused(void)
{
  return blPaused;
}  
