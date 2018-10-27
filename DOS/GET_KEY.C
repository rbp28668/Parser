/*
File:            GET_KEY.C

Function:        Main key/poll loop

*/

#include "key.h"
#include "screen.h"
#include "time.h"

#include "mouse.h"
#include "get_key.h"

#define EVER ;;

static int buttons;
static int x,y;


/********************************************************************/
/** GET_KEY is in fact the main polling loop of the system. It     **/
/** checks keyboard, mouse (if present) and anything else I may    **/
/** think of. Some keystrokes (e.g. F1 for help) are intercepted   **/
/** here so that the menus do not need to trap them specifically.  **/
/********************************************************************/
int get_key(int delay)
{
  int oldx,oldy;
  int key;
  time_t now,then;

  if(check_mouse())
    {
    buttons=read_mouse(&oldx,&oldy);
    scr_flip_char(oldy,oldx);
    }

  time(&then);
  for(EVER)        /* Main poll loop */
    {
    if(check_mouse())
      {
      buttons=read_mouse(&x,&y);
      if((x != oldx) || (y != oldy))
        {
        scr_flip_char(y,x);
        scr_flip_char(oldy,oldx);
        oldx=x;
        oldy=y;
        }
      if(buttons)
         {
        key = MOUSE;
        break;
        }
      }

    if(key_test())
      {
      key=key_read();
      /* do any special key processing here */
      break;
      }

    time(&now);
    if( (delay > 0) && ((int)(now-then) > delay))
      {
      key=TIMEOUT;
      break;
      }
    }

  if(check_mouse())
    scr_flip_char(oldy,oldx);

  return(key);
}

/********************************************************************/
/** GET_CLICK_POSITION allows the user to interrogate the mouse    **/
/** if get_key() returns MOUSE as a key-press.                     **/
/********************************************************************/
int get_click_position(int *mx,int *my)
{
  *mx = x;
  *my = y;
  return(buttons);
}

