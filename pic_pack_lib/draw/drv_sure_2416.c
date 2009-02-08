//--------------------------------------------------------
// Pic Pack library
// 
// draw_drv_sure_2416.h
//
// Sure 2416 drivers for the Draw library
//
// Ian Harris 2008
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if it breaks, you get to keep both pieces" license.
//--------------------------------------------------------

#ifndef __draw_drv_sure_2416_h
#define __draw_drv_sure_2416_h

#include "sure_2416.h"
#include "draw\draw.h"

void drv_set_pixel(uns8 x, uns8 y, uns8 colour) {
	sure_2416_set_pixel(x, y, colour);
}

uns8 drv_get_pixel(uns8 x, uns8 y) {
	sure_2416_get_pixel(x, y);
}

void drv_clear_screen() {
	sure_2416_clear_screen();
}

void drv_setup() {
	sure_2416_setup();
}

void drv_init() {
	sure_2416_init();
}	

#endif
