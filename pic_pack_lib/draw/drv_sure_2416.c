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
#include "draw\draw_screen_buffer.h"

void drv_paint() {

uns8 count;
uns8 x, xbit, xbyte, y;
uns8 * buffer;

	sure_2416_send_command(SURE_2416_CMD_LEDS_OFF);
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
	xbit = 0;
	xbyte = 0;
	for(x = 0 ; x < DRAW_PIXELS_WIDE  ; x++) {
		for(y = 0 ; y < DRAW_PIXELS_HIGH ; y++) {
			buffer = draw_buffers[y];
			if (test_bit(buffer[xbyte], xbit)) {
				set_pin(sure_2416_data_port, sure_2416_data_pin);
			} else {
				clear_pin(sure_2416_data_port, sure_2416_data_pin);
			}	
			clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
			set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
		}
		xbit++;
		if (xbit == 8) {
			xbit = 0;
			xbyte++;
		}	
	}
	// reset CS

	set_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	sure_2416_send_command(SURE_2416_CMD_LEDS_ON);

	
}	


uns8 drv_get_pixel(uns8 x, uns8 y) {
	//sure_2416_get_pixel(x, y);
}

void drv_clear_screen() {
	//sure_2416_clear_screen();
}

void drv_setup() {
	sure_2416_setup();
}

void drv_init() {
	sure_2416_init();
}	

#endif
