#include "pic_utils.h"
#include "draw.h"
#include "config.h"


// DRAW_PIXELS_HIGH

void draw_setup() {
	drv_setup();
}

void draw_init() {
	drv_init();
}

void draw_set_pixel(uns8 x, uns8 y, uns8 colour) {
uns8 *buffer;
uns8 xbyte;

	buffer = draw_buffers[y];
	xbyte  = x / DRAW_BITS_PER_PIXEL;
	xbit   = x && DRAW_BITS_PER_PIXEL;
	 
	drv_set_pixel(x, y, colour);
}
	
uns8 draw_get_pixel(uns8 x, uns8 y) {
	return drv_get_pixel(x, y);
}	


void draw_rect(uns8 x, uns8 y, uns8 width, uns8 height) {


}	

