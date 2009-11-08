//--------------------------------------------------------
// Pic Pack library
// 
// draw.h
//
// Graphics routines
//
// Ian Harris 2009
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

#define DRAW_HW_Y_ORIGIN TOP_LEFT
// or BOTTOM_LEFT

#define DRAW_HW_BUFFER_ORIENTATION VERTICAL
// or HORIZONTAL


//#define DRAW_DEBUG
*/


#ifndef __draw_h
#define __draw_h

#include "pic_utils.h"

#define HORIZONTAL 0
#define VERTICAL   1

#define TOP_LEFT	0
#define BOTTOM_LEFT	1
 
#define DRAW_PIXELS_PER_BYTE (8 / DRAW_BITS_PER_PIXEL)

#ifdef DRAW_DEBUG
	#include "pic_serial.h"
#endif	

/* struct to hold font information */

	

void draw_setup_io();
void draw_init();

void draw_clear_screen();

void draw_set_pixel(uns8 x, uns8 y, uns8 colour);
uns8 draw_get_pixel(uns8 x, uns8);
#define draw_paint() drv_paint()

void draw_line(uns8 x0, uns8 y0, uns8 x1, uns8 y1, uns8 colour);
void draw_circle(int x_centre, int y_centre, int r, uns8 colour);
void draw_circle2(int x_centre, int y_centre, int r, uns8 colour);
void draw_rect(uns8 x, uns8 y, uns8 width, uns8 height, uns8 colour);
void draw_print_str(uns8 x, uns8 y, uns8 width, uns8 start_pixel, uns8 colour, char *str);
void draw_print_buffer();

// driver routines

void drv_paint();
// For backwards compatibility:
#define drv_setup() drv_setup_io()
void drv_setup_io();
void drv_init();
void drv_refresh();

#endif