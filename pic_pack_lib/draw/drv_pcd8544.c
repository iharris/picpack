//--------------------------------------------------------
// Pic Pack library
// 
// draw_drv_pcd8544.c
//
// PCD8544 (Nokia 3310) drivers for the Draw library
//
// Ian Harris 2009
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if it breaks, you get to keep both pieces" license.
//--------------------------------------------------------



#include "pcd8544.h"
#include "draw\draw.h"
#include "draw\draw_screen_buffer.h"

void drv_paint() {

uns8 count;
uns8 x, y, inv_y;
uns16 buffer_loc;
uns16 byte_loc;
uns8  bit_loc, byte_out;

serial_print_str("paint! ");

// set x,y location to 0
pcd8544_send_command(0x80);

pcd8544_send_command(0x40);


byte_out = 0;

for(x = 0 ; x < DRAW_PIXELS_WIDE  ; x++) {
			
	for(y = 0 ; y < DRAW_PIXELS_HIGH ; y++) {
			buffer_loc = y * DRAW_PIXELS_WIDE + x;
			byte_loc = buffer_loc / DRAW_PIXELS_PER_BYTE;
			bit_loc = buffer_loc & (DRAW_PIXELS_PER_BYTE -1);
			
			byte_out = byte_out >> 1;
			if (test_bit(get_draw_buffer(byte_loc), bit_loc)) {
				byte_out.7  = 1;
			} else {
				byte_out.7  = 0;
			}
			if ((y & 0b00000111) == 0b00000111) {
				pcd8544_send_data(byte_out);
			}	
		}
	}
}	



void drv_setup_io() {
	pcd8544_setup_io();
}

void drv_init() {
	pcd8544_init();
}	

