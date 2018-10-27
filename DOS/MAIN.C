
#include <stdio.h>
#include <string.h>
#include "malloc.h"
#include "expand.h"
#include "prsif.h"
#include "parser.h"
#include "ident.h"
#include "pdrive.h"
#include "screen.h"
#include "tile.h"
#include "mouse.h"
#include "key.h"
#include "get.h"

/* Handles for tiles */
int hError;
int hOutput;
int hInput;

#ifdef _MSC_VER

void __cdecl _assert(void *pvCond, void *pvFile, unsigned uiLine)
{
  char szBuff[128];
  sprintf(szBuff,"assertion %s failed at %s, %u", pvCond, pvFile, uiLine);
  scr_tab(0,0);
  scr_colour(red,blue);
  scr_text(szBuff);
  scr_colour(white,blue);
}

#else

void _Cdecl _FARFUNC __assertfail( char _FAR *__msg,
                                   char _FAR *__cond,
                                   char _FAR *__file,
                                   int __line)
{
  char szBuff[128];
  sprintf(szBuff,__msg,__cond,__file,__line);
  scr_tab(0,0);
  scr_colour(red,blue);
  scr_text(szBuff);
  scr_colour(white,blue);
}
#endif


void ErrorFunction(const char* str)
{
  int wide,high;
  if(str != NULL)       
    {
    tile_inq_size(hError,&wide,&high);
    tile_scroll(hError,1,TRUE);
    tile_phantom_tab(hError,high-1,0);
    tile_text(hError,(char*)str);
    }
}

void OutputFunction(const char* str)
{
  int wide,high;
  if(str == NULL)         /* Page */
    tile_clear(hOutput);
  else
    {
    tile_inq_size(hOutput,&wide,&high);
    tile_scroll(hOutput,1,TRUE);
    tile_phantom_tab(hOutput,high-1,0);
    tile_text(hOutput,(char*)str);

    #if 1
    {
    FILE* outfp = fopen("DUMP.TMP","a+");
    if(outfp)
      {
      fprintf(outfp,"%s\n",str);
      fclose(outfp);
      }
    }
    #endif

    }
}

/* Function to read the next line from the input */
char* MoreInput(char* pszBuff,int nMaxChars)
{
  int key;

  tile_title(hInput,"More Input");
  strcpy(pszBuff,"");
  tile_clear(hInput);
  key = get_string(hInput,pszBuff);

  switch(key)
    {
    case HOME:
      return NULL;

    case NEWLINE:
      if(strlen(pszBuff) == 0)
        return NULL;
      else
        return pszBuff;
    }
  return NULL;
}


int main(int argc,char *argv[])
{
  int i;
  char **files = NULL;
  char **here = NULL;
  int key;
  char szBuff[128];
  PFSIS oldipfn;
  PFCSV oldopfn;
  PFCSV olderrfn;
  int IsEOF = FALSE;
  int fault;

  scr_settext();
  scr_colour(white,blue);

  if(check_mouse())
    {
    mouse_limits(0,0,SCR_WIDE-1,SCR_HIGH-1);
    mouse_slow(4);
    }

  hError = tile_create(SCR_WIDE-2,3,TRUE);
  hOutput = tile_create(SCR_WIDE-2,SCR_HIGH-10,TRUE);
  hInput = tile_create(SCR_WIDE-2,1,TRUE);

  tile_clear(hError);
  tile_clear(hOutput);
  tile_clear(hInput);

  tile_title(hError,"Error");
  tile_title(hOutput,"Output");
  tile_title(hInput,"Input");

  tile_paste(hError,0,0);
  tile_paste(hOutput,5,0);
  tile_paste(hInput,SCR_HIGH-3,0);

  scr_cursor(OFF);

  PRS_InitialiseInterpreter(0);

  oldopfn = PRS_SetOutputFunction(OutputFunction);
  olderrfn = PRS_SetErrorHandler(ErrorFunction);

  SetupParserExtensions();
  SetupStringExtensions();

  /* Load up any files specified on the command line */
  for (i=1; i<argc; ++i)
    {
    files = Expand(argv[i]);
    if(files != NULL)
      {
      for(here = files; *here!=NULL; ++here)
        {
        PRS_LoadFile(*here);
        }
      free(files);
      }
    }

  /* and now prompt for input from the user */
  szBuff[0] = '\0';
  oldipfn = PRS_InputFunction(MoreInput);
  do
    {
    tile_title(hInput,"ÍÍÍInputÍÍÍ");
    strcpy(szBuff,"");
    tile_clear(hInput);
    key = get_string(hInput,szBuff);
    if(key == NEWLINE)
      {
      PRS_SendCommand(szBuff);
      if(!PRS_ParseUnit(&IsEOF))
        {
        if(!PRS_StartVM())
          {
          do
            {
            fault = PRS_TickVM();
            if(key_test())
              {
              if(key_read() == END)
                {
                PRS_StopVM();
                break;
                }
              }
            }
          while(fault == NO_FAULT);
          }
        }

      }
    }
  while (!QuitInterpreter() && key != HOME);

  PRS_InputFunction(oldipfn); /* restore old input function */
  PRS_SetOutputFunction(oldopfn);
  PRS_SetErrorHandler(olderrfn);
  PRS_TerminateInterpreter();

  tile_kill(hInput);
  tile_kill(hOutput);
  tile_kill(hError);

  scr_stop();
  return 0;
}
