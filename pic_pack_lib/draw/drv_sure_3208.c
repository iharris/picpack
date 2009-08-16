//--------------------------------------------------------
// Pic Pack library
// 
// draw_drv_sure_3207.h
//
// Sure 3208 drivers for the Draw library
//
// Ian Harris 2008
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if it breaks, you get to keep both pieces" license.
//--------------------------------------------------------

#ifndef __draw_drv_sure_2416_h
#define __draw_drv_sure_2416_h

#include "ht1632.h"
#include "draw\draw.h"
#include "draw\draw_screen_buffer.h"

void drv_paint() {

uns8 count;
uns8 x, y, inv_y;
uns16 buffer_loc;
uns8 byte_loc, bit_loc;
uns8 data;

	//ht1632_send_command(HT1632_CMD_SYS_DISABLE);
	clear_pin(ht1632_cs1_port, ht1632_cs1_pin);
	
	// send WR command

	// send 1
	set_pin  (ht1632_data_port, ht1632_data_pin);
	// pulse wr
	clear_pin(ht1632_wr_port, ht1632_wr_pin);
	set_pin  (ht1632_wr_port, ht1632_wr_pin);
	
	// send 0
	clear_pin  (ht1632_data_port, ht1632_data_pin);
	// pulse wr
	clear_pin(ht1632_wr_port, ht1632_wr_pin);
	set_pin  (ht1632_wr_port, ht1632_wr_pin);

	// send 1
	set_pin  (ht1632_data_port, ht1632_data_pin);
	// pulse wr
	clear_pin(ht1632_wr_port, ht1632_wr_pin);
	set_pin  (ht1632_wr_port, ht1632_wr_pin);

	// send mem address of zero
	clear_pin(ht1632_data_port, ht1632_data_pin);
	
	// write mem addr, bits 6 -> 0
	for(count = 0 ; count < 7 ; count++) {
			
		//change_pin_var(ht1632_data_port, ht1632_data_pin, test_bit(mem_addr, 6));
		// pulse wr
		clear_pin(ht1632_wr_port, ht1632_wr_pin);
		set_pin  (ht1632_wr_port, ht1632_wr_pin);
		// shift mem addr along

	}
/*
	for(x = 0 ; x < DRAW_PIXELS_WIDE  ; x++) {
		for(y = 0 ; y < DRAW_PIXELS_HIGH ; y++) {
			inv_y = DRAW_PIXELS_HIGH - 1 - y;
			buffer_loc = inv_y * DRAW_PIXELS_WIDE + x;
			byte_loc = buffer_loc / DRAW_PIXELS_PER_BYTE;
			bit_loc = buffer_loc & (DRAW_PIXELS_PER_BYTE -1);
			if (test_bit(draw_buffer0[byte_loc], bit_loc)) {
				set_pin(ht1632_data_port, ht1632_data_pin);
			} else {
				clear_pin(ht1632_data_port, ht1632_data_pin);
			}	
			clear_pin(ht1632_wr_port, ht1632_wr_pin);
			set_pin  (ht1632_wr_port, ht1632_wr_pin);
		}
	}
*/	
	buffer_loc = 0;
	for (count = 0; count < 32; count++) {
		data = draw_buffer0[count];
		for (bit_loc = 0; bit_loc < 8; bit_loc++) {
			if (data.0) {
				set_pin(ht1632_data_port, ht1632_data_pin);
			} else {
				clear_pin(ht1632_data_port, ht1632_data_pin);
			}
			data = data >> 1;	
			clear_pin(ht1632_wr_port, ht1632_wr_pin);
			set_pin  (ht1632_wr_port, ht1632_wr_pin);
		}	
	}	

	/*uns8 xbyte = 0x2d;
	uns8 xbit = 0b00000001;
	
	for(x = 0 ; x < DRAW_PIXELS_WIDE  ; x++) {
		for(y = 0 ; y < 16 ; y++) {
		if (draw_buffer0[xbyte] & xbit) {
				set_pin(ht1632_data_port, ht1632_data_pin);
			} else {
				clear_pin(ht1632_data_port, ht1632_data_pin);
			}	
			clear_pin(ht1632_wr_port, ht1632_wr_pin);
			set_pin  (ht1632_wr_port, ht1632_wr_pin);
			if (xbyte < 3) {
				if (xbit == 0b10000000) {
					xbyte = xbyte + 0x2e;
					xbit = 0b00000001;
				} else {
					xbit = xbit << 1;
					xbyte = xbyte + 0x2d;
				}
			} else {		
				xbyte = xbyte - 3;
			}	
		}
	}
*/	// reset CS

	set_pin(ht1632_cs1_port, ht1632_cs1_pin);
	//ht1632_send_command(HT1632_CMD_SYS_ENABLE);

	
}	


uns8 drv_get_pixel(uns8 x, uns8 y) {
	//ht1632_get_pixel(x, y);
}

void drv_clear_screen() {
	//ht1632_clear_screen();
}

void drv_setup_io() {
	ht1632_setup_io();
}

void drv_init() {
	ht1632_init();
}	

#endif
