//--------------------------------------------------------
// Pic Pack library
// 
// draw_drv_sure_2416.h
//
// Sure 2416 drivers for the Draw library
//
// Ian Harris 2009
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if it breaks, you get to keep both pieces" license.
//--------------------------------------------------------

#ifndef __draw_drv_sure_2416_h
#define __draw_drv_sure_2416_h

#include "config.h"
#include "ht1632.h"
#include "draw\draw.h"
#include "draw\draw_screen_buffer.h"

#if ht1632_displays > 1

void drv_paint_display(uns16 buffer_loc) {

uns8 count;
uns8 x, y, inv_y;
uns8 byte_loc, bit_loc;
uns8 data;

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
		// pulse wr
		clear_pin(ht1632_wr_port, ht1632_wr_pin);
		set_pin  (ht1632_wr_port, ht1632_wr_pin);
	}

	for (count = 0; count < 48; count++) {
		data = draw_buffer0[buffer_loc];
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
		buffer_loc++;	
	}	
	
}	

void drv_paint() {

uns16 buffer_loc;

	buffer_loc = 0;
		
	#if ht1632_displays > 0	
		clear_pin(ht1632_cs1_port, ht1632_cs1_pin);
		drv_paint_display(buffer_loc);
		set_pin(ht1632_cs1_port, ht1632_cs1_pin);
	#endif
	#if ht1632_displays > 1
		buffer_loc = buffer_loc + 48;
		clear_pin(ht1632_cs2_port, ht1632_cs2_pin);
		drv_paint_display(buffer_loc);
		set_pin(ht1632_cs2_port, ht1632_cs2_pin);
	#endif
	#if ht1632_displays > 2
		buffer_loc = buffer_loc + 48;
		clear_pin(ht1632_cs3_port, ht1632_cs3_pin);
		drv_paint_display(buffer_loc);
		set_pin(ht1632_cs3_port, ht1632_cs3_pin);
	#endif
	#if ht1632_displays > 3
		buffer_loc = buffer_loc + 48;
		clear_pin(ht1632_cs4_port, ht1632_cs4_pin);
		drv_paint_display(buffer_loc);
		set_pin(ht1632_cs4_port, ht1632_cs4_pin);
	#endif
}

#else

void drv_paint() {

uns8 count;
uns8 x, y, inv_y;
uns16 buffer_loc;
uns8 byte_loc, bit_loc;
uns8 data;

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
			
		// pulse wr
		clear_pin(ht1632_wr_port, ht1632_wr_pin);
		set_pin  (ht1632_wr_port, ht1632_wr_pin);
	}

	buffer_loc = 0;
	for (count = 0; count < 48; count++) {
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
	
	// reset CS

	set_pin(ht1632_cs1_port, ht1632_cs1_pin);
}	
#endif

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
	// 2416 board is configured as 16 COMMONs
	ht1632_init(HT1632_CMD_PMOS_16_COMMON);
}	

#endif
