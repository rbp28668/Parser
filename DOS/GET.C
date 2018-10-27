/*
File:                GET.C

*/
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "screen.h"
#include "tile.h"
#include "key.h"
#include "get_key.h"
#include "mouse.h"
#include "malloc.h"

#define LENGTH_LEN 24
#define YES 1
#define NO 0
#define ON 1
#define OFF 0


/************************************************************/
/** GET_STRING is the editing part of all the other    **/
/** gets.                                                  **/
/************************************************************/
#pragma optimize ("l",off)
int get_string(int handle, char *string)
{
  int wide,high;           /* size of tile */
  int s_row,s_col;      /* position on screen */
  int cpos;           /* current cursor pos */
  int key;           /* current key press */
  int stop;          /* stops entry when set true */
  char work[SCR_WIDE+1];    /* working buffer */
  int first_key;        /* true if no keys hit yet */
  int insertmode;        /* true if insert mode */
  int nchars;          /* number of chars in buffer */
  int mx,my;          /* mouse coordinates */
  int left,right;        /* mouse buttons */
  int i;

  tile_inq_size(handle,&wide,&high);
  tile_inq_posn(handle,&s_row,&s_col);
  scr_colour(white,blue);
  tile_tab(handle,0,0);
  tile_text(handle,string);
  tile_tab(handle,0,0);
  scr_cursor(ON);

  strncpy(work,string,sizeof(work)-1);
  work[sizeof(work)-1]='\0';

  cpos=0;
  stop=0;
  nchars=strlen(work);
  insertmode=0;
  first_key=1;
  key=0;

  while(!stop)
    {
    key=get_key(0);
    switch(key)
      {
      case MOUSE:
        mouse_buttons(&left,&right);

        if(right)
          {
          while(read_mouse(&mx,&my));   /* wait until released */
          key=PAGEUP;
          stop=1;
          break;
          }

        get_click_position(&mx,&my);    
        if(mx<=s_col) break;
        if(mx> (s_col+wide)) break;
        if(my < (s_row+1)) break;
        if(my> (s_row+high)) break;

        key=NEWLINE;          /* lhs click same as newline*/
        strcpy(string,work);
        stop=1;
        while(read_mouse(&mx,&my));   /* wait until released */
        break;

      case NEWLINE:
        strcpy(string,work);
        stop=1;
        break;
        case CURSOR_RIGHT:
        if(cpos < (wide-1)) ++cpos;
        tile_tab(handle,0,cpos);
        break;
      case CURSOR_LEFT:
        if(cpos > 0) --cpos;
        tile_tab(handle,0,cpos);
        break;
      case HOME:
      case PAGEUP:
        stop=1;
        break;
      case INSERT:
        insertmode=!insertmode;
        break;
      case BS:           /* back delete */
        if(cpos<=0)
          putchar('\a');
        else
          {
          for(i=cpos;i<nchars;++i)
            {
            tile_char(handle,0,i-1,work[i]);
            work[i-1]=work[i];
            }
          --cpos;
          --nchars;
          work[nchars]='\0';
          tile_char(handle,0,nchars,' ');
          tile_tab(handle,0,cpos);
          }
        break;
      case DELETE:        /* forward delete */
        if(cpos>=nchars)
          putchar('\a');
        else
          {
          for(i=cpos;i<(nchars-1);++i)
            {
            tile_char(handle,0,i,work[i+1]);
            work[i]=work[i+1];
            }
          --nchars;
          work[nchars]='\0';
          tile_char(handle,0,nchars,' ');
          }
        break;
      default:
        if(isprint(key))
          {
          if(first_key)    /* if first key clear buffer */
            {
            memset(work,' ',wide);
            work[wide]='\0';
            tile_tab(handle,0,0);
            tile_text(handle,work);
            tile_tab(handle,0,0);
            cpos=0;
            nchars=0;
            }
          if(insertmode)
            {
            if(nchars >= wide)    /* if buffer full */
              {
              putchar('\a');    /* beep */
              break;        /* and abort */
              }
            for(i=nchars;i>cpos;--i)  /* move string up */
              {
              tile_char(handle,0,i,work[i-1]);
              work[i]=work[i-1];
              }
            ++nchars;
            }
          work[cpos]=(char)key;
          tile_char(handle,0,cpos,key);
          if(cpos<(wide-1))
            {
            if(cpos >=nchars)
              ++nchars;
            ++cpos;
            }
          tile_tab(handle,0,cpos);
          work[nchars]='\0';
          }/* endif isprint(key) */
      }/* endswitch */
      first_key=0;
    }/* endwhile*/
  scr_cursor(OFF);
  return(key);
}
#pragma optimize ("l",on)

