/*
File:              MOUSE.H

Program/rev.:      Aerofoil 0.0

Function:          Mouse handling

Author:            Bruce
              
Date:              c. Sept 91

Routines:

*/
#ifndef _MOUSE_H
#define _MOUSE_H

extern	int check_mouse(void );
extern	int read_mouse(int *x,int *y);
extern	void mouse_limits(int minx,int miny,int maxx,int maxy);
extern	void mouse_slow(int speed);
extern	void mouse_buttons(int *left,int *right);

#endif
