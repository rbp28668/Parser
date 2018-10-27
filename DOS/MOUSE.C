/*
File:              MOUSE.C

Program/rev.:      Aerofoil 0.0

Function:          Mouse handling

Author:            Bruce
              
Date:              c. Sept 91

Routines:

*/

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <limits.h>
#include "mouse.h"

typedef struct s_mouse
{
  int x;
  int y;
  int left;
  int right;
  int minx;
  int miny;
  int maxx;
  int maxy;
  int slow;
}MOUSE;

union REGS regs;

static MOUSE mouse=
  {
  0,            /* x */
  0,            /* y */
  0,            /* left */
  0,            /* right */
  INT_MIN,      /* minx */
  INT_MIN,      /* miny */
  INT_MAX,      /* maxx */
  INT_MAX,      /* maxy */
  1             /* slow */
  };

static int has_mouse = 0;
static int mouse_checked = 0;

/****************************************************************/
/** CHECK_MOUSE returns non-zero if a mouse is present, zero   **/
/** if the mouse is not installed.                             **/
/****************************************************************/
int check_mouse(void)
{
  if (!mouse_checked)
    {
    regs.x.ax = 0;
    int86(0x33,&regs,&regs);
    has_mouse = regs.x.ax;
    mouse_checked = 1;
    }
  return has_mouse;
}

/****************************************************************/
/** READ_MOUSE updates the current mouse coordinates and       **/
/** returns non-zero if a button was pressed.                  **/
/****************************************************************/
int read_mouse(int *x,int *y)
{
  regs.x.ax = 11;
  int86(0x33,&regs,&regs);
  mouse.x += regs.x.cx;
  mouse.y += regs.x.dx;

  mouse.x = min(mouse.x,mouse.maxx*mouse.slow);
  mouse.x  = max(mouse.x,mouse.minx*mouse.slow);
  mouse.y = min(mouse.y,mouse.maxy*mouse.slow);
  mouse.y = max(mouse.y,mouse.miny*mouse.slow);

  *x=mouse.x/mouse.slow;
  *y=mouse.y/mouse.slow;

  regs.x.ax = 3;
  int86(0x33,&regs,&regs);
  mouse.left =   regs.h.bl & 1;
  mouse.right = (regs.h.bl & 2) >> 1;
  return(regs.h.bl & 0x03);
}

/****************************************************************/
/** MOUSE_LIMITS sets limits to the values reported by         **/
/** read_mouse().                                              **/
/****************************************************************/
void mouse_limits(int minx,int miny,int maxx, int maxy)
{
  mouse.minx=minx;
  mouse.miny=miny;
  mouse.maxx=maxx;
  mouse.maxy=maxy;
  return;
}

/****************************************************************/
/** MOUSE_SLOW slows down the mouse by the given factor.       **/
/****************************************************************/
void mouse_slow(int speed)
{
  mouse.slow = speed;
  return;
}

/****************************************************************/
/** MOUSE_BUTTONS returns the buttons pressed during the last  **/
/** call to read_mouse();                                      **/
/****************************************************************/
void mouse_buttons(int *left,int *right)
{
  *left=mouse.left;
  *right=mouse.right;
  return;
}

