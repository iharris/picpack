#include "pic_utils.h"
#include "draw.h"
#include "config.h"
#include "draw\draw_screen_buffer.h"

// DRAW_PIXELS_HIGH

void draw_setup() {
	//drv_setup();
}

void draw_init() {
	//drv_init();
}

void draw_set_pixel(uns8 x, uns8 y, uns8 colour) {
uns8 *buffer;
uns8 xbyte, xbit;

	buffer = draw_buffers[y];
	xbyte  = x / DRAW_BITS_PER_PIXEL;
	xbit   = x & DRAW_BITS_PER_PIXEL;
	 
	if (colour) {
		set_bit(buffer[xbyte], xbit);
	} else {
		clear_bit(buffer[xbyte], xbit);
	}	
}
	
uns8 draw_get_pixel(uns8 x, uns8 y) {
	return 0;
}	


void draw_rect(uns8 x, uns8 y, uns8 width, uns8 height) {


}	

