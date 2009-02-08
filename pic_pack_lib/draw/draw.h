//--------------------------------------------------------
// Pic Pack library
// 
// draw.h
//
// Graphics routines
//
// Ian Harris 2008
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if it breaks, you get to keep both pieces" license.
//--------------------------------------------------------

/* Include these in your config.h

// - - - - - - - - - - - - - - - - - - - - 
// Draw defines
// - - - - - - - - - - - - - - - - - - - - 

#define DRAW_PIXELS_HIGH 24
#define DRAW_PIXELS_WIDE 16
#define DRAW_BITS_PER_PIXEL 1
//#define DRAW_DEBUG
*/


#ifndef __draw_h
#define __draw_h

#ifdef DRAW_DEBUG
	#include "pic_serial.h"
#endif	

// these routines in the driver
void draw_setup();
void draw_init();

void draw_clear_screen();

void draw_set_pixel(uns8 x, uns8 y, uns8 colour);
uns8 draw_get_pixel(uns8 x, uns8);

void draw_line(uns8 x1, uns8 y1, uns8 x2, uns8 y2);
void draw_circle(uns8 x, uns8 y, uns8 r);
void draw_rect(uns8 x, uns8 y, uns8 width, uns8 height);

void drv_setup();
void drv_init();
void drv_set_pixel(uns8 x, uns8 y, uns8 colour);
uns8 drv_get_pixel(uns8 x, uns8);

// internal routines


#endif