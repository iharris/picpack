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
uns8 byte_loc, bit_loc;

	//sure_2416_send_command(SURE_2416_CMD_SYS_DISABLE);
	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	
	// send WR command

	// send 1
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	
	// send 0
	clear_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

	// send 1
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

	// send mem address of zero
	clear_pin(sure_2416_data_port, sure_2416_data_pin);
	
	// write mem addr, bits 6 -> 0
	for(count = 0 ; count < 7 ; count++) {
			
		//change_pin_var(sure_2416_data_port, sure_2416_data_pin, test_bit(mem_addr, 6));
		// pulse wr
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
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
				set_pin(sure_2416_data_port, sure_2416_data_pin);
			} else {
				clear_pin(sure_2416_data_port, sure_2416_data_pin);
			}	
			clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
			set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
		}
	}
*/	uns8 xbyte = 0x2d;
	uns8 xbit = 0b00000001;
	
	for(x = 0 ; x < DRAW_PIXELS_WIDE  ; x++) {
		for(y = 0 ; y < DRAW_PIXELS_HIGH ; y++) {
		if (draw_buffer0[xbyte] & xbit) {
				set_pin(sure_2416_data_port, sure_2416_data_pin);
			} else {
				clear_pin(sure_2416_data_port, sure_2416_data_pin);
			}	
			clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
			set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
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
	// reset CS

	set_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	//sure_2416_send_command(SURE_2416_CMD_SYS_ENABLE);

	
}	


uns8 drv_get_pixel(uns8 x, uns8 y) {
	//sure_2416_get_pixel(x, y);
}

void drv_clear_screen() {
	//sure_2416_clear_screen();
}

void drv_setup_io() {
	pcd8544_setup_io();
}

void drv_init() {
	pcd8544_init();
}	

