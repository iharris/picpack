//--------------------------------------------------------
// Pic Pack library
// 
// draw_drv_sure_6432.h
//
// Sure 6432 drivers for the Draw library
//
// Ian Harris 2009
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if it breaks, you get to keep both pieces" license.
//--------------------------------------------------------

#ifndef __draw_drv_sure_6432_h
#define __draw_drv_sure_6432_h

#include "sure_6432.h"
#include "config.h"
#include "draw\draw.h"
#include "draw\draw_screen_buffer.h"

#define set_pins_r1_g1_r2_g2() \
	set_pin(sure_6432_r1_port, sure_6432_r1_pin); \
	set_pin(sure_6432_r2_port, sure_6432_r2_pin); \
	set_pin(sure_6432_g1_port, sure_6432_g1_pin); \
	set_pin(sure_6432_g2_port, sure_6432_g2_pin)  

char buffer0[256];
char buffer1[256];
#if sure_6432_displays == 2
	char buffer2[256];
	char buffer3[256];
#endif
	
void drv_paint() {
	uns8 count;
	
	count = 0;
	
	do {
		buffer0[count] = draw_buffer0[count];
		buffer1[count] = draw_buffer1[count];
		#if sure_6432_displays == 2
			buffer2[count] = draw_buffer2[count];
			buffer3[count] = draw_buffer3[count];
		#endif
		count++;
	} while (count !=0);	
}

void drv_refresh() {

uns8 count;
uns8 x, y, inv_y;
uns16 buffer_loc;
uns8 byte_loc, bit_loc;
uns8 data_upper, data_lower;
uns8 count_byte_disp1;


	count_byte_disp1 = 0;				// Start upper at the top

//	count_lower = 64 * 2 * 16 / 8;	// Start lower at the second half
	#if sure_6432_displays == 2
		uns8 count_byte_disp2 = 0;
		//uns16 count_upper_disp2, count_lower_disp2;
	//	count_upper_disp2 = (64 * 2 * 16 / 8) * 2;
	//	count_lower_disp2 = (64 * 2 * 16 / 8) * 3;
	#endif
	for (y = 0; y < 16; y++) {
		
		for (x = 0; x < 16; x++) {
			data_upper = buffer0[count_byte_disp1];
			data_lower = buffer1[count_byte_disp1];
			
			set_pins_r1_g1_r2_g2();
			
			if (data_upper.0) { 
				clear_pin(sure_6432_r1_port, sure_6432_r1_pin);
			}
			if (data_upper.1) {
				clear_pin(sure_6432_g1_port, sure_6432_g1_pin);
			}
			if (data_lower.0) {
				clear_pin(sure_6432_r2_port, sure_6432_r2_pin);
			}
			if (data_lower.1) {
				clear_pin(sure_6432_g2_port, sure_6432_g2_pin);
			}
				
			clear_pin(sure_6432_s_port, sure_6432_s_pin);
			set_pin  (sure_6432_s_port, sure_6432_s_pin);
			
			set_pins_r1_g1_r2_g2();
			
			if (data_upper.2) { 
				clear_pin(sure_6432_r1_port, sure_6432_r1_pin);
			}
			if (data_upper.3) {
				clear_pin(sure_6432_g1_port, sure_6432_g1_pin);
			}
			if (data_lower.2) {
				clear_pin(sure_6432_r2_port, sure_6432_r2_pin);
			}
			if (data_lower.3) {
				clear_pin(sure_6432_g2_port, sure_6432_g2_pin);
			}
				
			clear_pin(sure_6432_s_port, sure_6432_s_pin);
			set_pin  (sure_6432_s_port, sure_6432_s_pin);
			
			set_pins_r1_g1_r2_g2();
			
			if (data_upper.4) { 
				clear_pin(sure_6432_r1_port, sure_6432_r1_pin);
			}
			if (data_upper.5) {
				clear_pin(sure_6432_g1_port, sure_6432_g1_pin);
			}
			if (data_lower.4) {
				clear_pin(sure_6432_r2_port, sure_6432_r2_pin);
			}
			if (data_lower.5) {
				clear_pin(sure_6432_g2_port, sure_6432_g2_pin);
			}
				
			clear_pin(sure_6432_s_port, sure_6432_s_pin);
			set_pin  (sure_6432_s_port, sure_6432_s_pin);
			
			set_pins_r1_g1_r2_g2();
			
			
			if (data_upper.6) { 
				clear_pin(sure_6432_r1_port, sure_6432_r1_pin);
			}
			if (data_upper.7) {
				clear_pin(sure_6432_g1_port, sure_6432_g1_pin);
			}
			if (data_lower.6) {
				clear_pin(sure_6432_r2_port, sure_6432_r2_pin);
			}
			if (data_lower.7) {
				clear_pin(sure_6432_g2_port, sure_6432_g2_pin);
			}
				
			clear_pin(sure_6432_s_port, sure_6432_s_pin);
			set_pin  (sure_6432_s_port, sure_6432_s_pin);
			
			count_byte_disp1++;
		} 	

	#if sure_6432_displays == 2
		for (x = 0; x < 16; x++) {
			data_upper = buffer2[count_byte_disp2];
			data_lower = buffer3[count_byte_disp2];
			
			set_pins_r1_g1_r2_g2();
			
			if (data_upper.0) { 
				clear_pin(sure_6432_r1_port, sure_6432_r1_pin);
			}
			if (data_upper.1) {
				clear_pin(sure_6432_g1_port, sure_6432_g1_pin);
			}
			if (data_lower.0) {
				clear_pin(sure_6432_r2_port, sure_6432_r2_pin);
			}
			if (data_lower.1) {
				clear_pin(sure_6432_g2_port, sure_6432_g2_pin);
			}
				
			clear_pin(sure_6432_s_port, sure_6432_s_pin);
			set_pin  (sure_6432_s_port, sure_6432_s_pin);
			
			set_pins_r1_g1_r2_g2();
			
			if (data_upper.2) { 
				clear_pin(sure_6432_r1_port, sure_6432_r1_pin);
			}
			if (data_upper.3) {
				clear_pin(sure_6432_g1_port, sure_6432_g1_pin);
			}
			if (data_lower.2) {
				clear_pin(sure_6432_r2_port, sure_6432_r2_pin);
			}
			if (data_lower.3) {
				clear_pin(sure_6432_g2_port, sure_6432_g2_pin);
			}
				
			clear_pin(sure_6432_s_port, sure_6432_s_pin);
			set_pin  (sure_6432_s_port, sure_6432_s_pin);
			
			set_pins_r1_g1_r2_g2();
			
			if (data_upper.4) { 
				clear_pin(sure_6432_r1_port, sure_6432_r1_pin);
			}
			if (data_upper.5) {
				clear_pin(sure_6432_g1_port, sure_6432_g1_pin);
			}
			if (data_lower.4) {
				clear_pin(sure_6432_r2_port, sure_6432_r2_pin);
			}
			if (data_lower.5) {
				clear_pin(sure_6432_g2_port, sure_6432_g2_pin);
			}
				
			clear_pin(sure_6432_s_port, sure_6432_s_pin);
			set_pin  (sure_6432_s_port, sure_6432_s_pin);
			
			set_pins_r1_g1_r2_g2();
			
			
			if (data_upper.6) { 
				clear_pin(sure_6432_r1_port, sure_6432_r1_pin);
			}
			if (data_upper.7) {
				clear_pin(sure_6432_g1_port, sure_6432_g1_pin);
			}
			if (data_lower.6) {
				clear_pin(sure_6432_r2_port, sure_6432_r2_pin);
			}
			if (data_lower.7) {
				clear_pin(sure_6432_g2_port, sure_6432_g2_pin);
			}
				
			clear_pin(sure_6432_s_port, sure_6432_s_pin);
			set_pin  (sure_6432_s_port, sure_6432_s_pin);
			
			count_byte_disp2++;
		} // x loop	
		
	#endif
			
		//serial_print_nl();
		set_pin(sure_6432_en_port, sure_6432_en_pin);	// turn enable off
		
		clear_pin(sure_6432_a_port, sure_6432_a_pin);
		clear_pin(sure_6432_b_port, sure_6432_b_pin);
		clear_pin(sure_6432_c_port, sure_6432_c_pin);
		clear_pin(sure_6432_d_port, sure_6432_d_pin);
		if (y.0) {
			set_pin(sure_6432_a_port, sure_6432_a_pin);
		}		
		if (y.1) {
			set_pin(sure_6432_b_port, sure_6432_b_pin);
		}	
		if (y.2) {
			set_pin(sure_6432_c_port, sure_6432_c_pin);
		}		
		if (y.3) {
			set_pin(sure_6432_d_port, sure_6432_d_pin);
		} 
		// latch data	
		set_pin(sure_6432_l_port, sure_6432_l_pin);
		clear_pin(sure_6432_l_port, sure_6432_l_pin);
		
		// enable display of line
		clear_pin(sure_6432_en_port, sure_6432_en_pin);
		//delay_us(200);
	} // y loop	
	delay_us(200);
	set_pin(sure_6432_en_port, sure_6432_en_pin);
}

void drv_paint_works() {

uns8 count;
uns8 x, y, inv_y;
uns16 buffer_loc;
uns8 byte_loc, bit_loc;
uns8 data_upper, data_lower;
uns8 count_byte;

	
	count_byte = 0;				// Start upper at the top
	
//	count_lower = 64 * 2 * 16 / 8;	// Start lower at the second half
	#if sure_6432_displays == 2
		uns8 count_byte_disp2 = 0;
		//uns16 count_upper_disp2, count_lower_disp2;
	//	count_upper_disp2 = (64 * 2 * 16 / 8) * 2;
	//	count_lower_disp2 = (64 * 2 * 16 / 8) * 3;
	#endif
	
	for (y = 0; y < 16; y++) {
		
		for (x = 0; x < 16; x++) {
			data_upper = draw_buffer0[count_byte];
			data_lower = draw_buffer1[count_byte];
			for (bit_loc = 0; bit_loc < 4; bit_loc++) {
				if (data_upper.0) {
					clear_pin(sure_6432_r1_port, sure_6432_r1_pin);
				} else {
					set_pin(sure_6432_r1_port, sure_6432_r1_pin);
				}
				if (data_upper.1) {
					clear_pin(sure_6432_g1_port, sure_6432_g1_pin);
				} else {
					set_pin(sure_6432_g1_port, sure_6432_g1_pin);
				}
				if (data_lower.0) {
					clear_pin(sure_6432_r2_port, sure_6432_r2_pin);
				} else {
					set_pin(sure_6432_r2_port, sure_6432_r2_pin);
				}
				if (data_lower.1) {
					clear_pin(sure_6432_g2_port, sure_6432_g2_pin);
				} else {
					set_pin(sure_6432_g2_port, sure_6432_g2_pin);
				}
				
				data_upper = data_upper >> 2;
				data_lower = data_lower >> 2;	
				clear_pin(sure_6432_s_port, sure_6432_s_pin);
				set_pin  (sure_6432_s_port, sure_6432_s_pin);
			}
			count_byte++;
		}	

		#if sure_6432_displays == 2
			for (x = 0; x < 16; x++) {
				data_upper = draw_buffer2[count_byte_disp2];
				data_lower = draw_buffer3[count_byte_disp2];
				for (bit_loc = 0; bit_loc < 4; bit_loc++) {
					if (data_upper.0) {
						clear_pin(sure_6432_r1_port, sure_6432_r1_pin);
	
					} else {
						set_pin(sure_6432_r1_port, sure_6432_r1_pin);
					}
					if (data_upper.1) {
						clear_pin(sure_6432_g1_port, sure_6432_g1_pin);
					} else {
						set_pin(sure_6432_g1_port, sure_6432_g1_pin);
					}
					if (data_lower.0) {
						clear_pin(sure_6432_r2_port, sure_6432_r2_pin);

					} else {
						//serial_putc('0');
						set_pin(sure_6432_r2_port, sure_6432_r2_pin);
					}
					if (data_lower.1) {
						clear_pin(sure_6432_g2_port, sure_6432_g2_pin);
						//serial_putc('1');
					} else {
						//serial_putc('0');
						set_pin(sure_6432_g2_port, sure_6432_g2_pin);
					}
					
					data_upper = data_upper >> 2;
					data_lower = data_lower >> 2;	
					clear_pin(sure_6432_s_port, sure_6432_s_pin);
					set_pin  (sure_6432_s_port, sure_6432_s_pin);
				}
				count_byte_disp2++;
			}	
		
		#endif
			
		//serial_print_nl();
		set_pin(sure_6432_en_port, sure_6432_en_pin);	// turn enable off
		
		if (y.0) {
			set_pin(sure_6432_a_port, sure_6432_a_pin);
		} else {
			clear_pin(sure_6432_a_port, sure_6432_a_pin);
		}	
		if (y.1) {
			set_pin(sure_6432_b_port, sure_6432_b_pin);
		} else {
			clear_pin(sure_6432_b_port, sure_6432_b_pin);
		}	
		if (y.2) {
			set_pin(sure_6432_c_port, sure_6432_c_pin);
		} else {
			clear_pin(sure_6432_c_port, sure_6432_c_pin);
		}	
		if (y.3) {
			set_pin(sure_6432_d_port, sure_6432_d_pin);
		} else {
			clear_pin(sure_6432_d_port, sure_6432_d_pin);
		}	
		
		// latch data	
		set_pin(sure_6432_l_port, sure_6432_l_pin);
		clear_pin(sure_6432_l_port, sure_6432_l_pin);
		
		// enable display of line
		clear_pin(sure_6432_en_port, sure_6432_en_pin);
		//delay_us(200);
	}
}	

void drv_paint_test() {

uns8 count;
uns8 x, y, inv_y;
uns16 buffer_loc;
uns8 byte_loc, bit_loc;
uns8 data_upper, data_lower;
uns16 count_upper, count_lower;

	
	count_upper = 0;				// Start upper at the top
	count_lower = 64 * 2 * 16 / 8;	// Start lower at the second half
	
					clear_pin(sure_6432_r1_port, sure_6432_r1_pin);
					clear_pin(sure_6432_g1_port, sure_6432_g1_pin);
					clear_pin(sure_6432_r2_port, sure_6432_r2_pin);
					clear_pin(sure_6432_g2_port, sure_6432_g2_pin);
	for (y = 0; y < 16; y++) {
		for (x = 0; x < 64; x++) {
				
				set_pin(sure_6432_s_port, sure_6432_s_pin);
				clear_pin  (sure_6432_s_port, sure_6432_s_pin);
			}
		
		set_pin(sure_6432_en_port, sure_6432_en_pin);	// turn enable off
		
		if (y.0) {
			set_pin(sure_6432_a_port, sure_6432_a_pin);
		} else {
			clear_pin(sure_6432_a_port, sure_6432_a_pin);
		}	
		if (y.1) {
			set_pin(sure_6432_b_port, sure_6432_b_pin);
		} else {
			clear_pin(sure_6432_b_port, sure_6432_b_pin);
		}	
		if (y.2) {
			set_pin(sure_6432_c_port, sure_6432_c_pin);
		} else {
			clear_pin(sure_6432_c_port, sure_6432_c_pin);
		}	
		if (y.3) {
			set_pin(sure_6432_d_port, sure_6432_d_pin);
		} else {
			clear_pin(sure_6432_d_port, sure_6432_d_pin);
		}	
		
		// latch data	
		set_pin(sure_6432_l_port, sure_6432_l_pin);
		delay_us(10);
		clear_pin(sure_6432_l_port, sure_6432_l_pin);
		
		// enable display of line
		clear_pin(sure_6432_en_port, sure_6432_en_pin);
		//delay_ms(5);
		delay_us(10);
	}
}	



uns8 drv_get_pixel(uns8 x, uns8 y) {
	//ht1632_get_pixel(x, y);
}

void drv_clear_screen() {
	//ht1632_clear_screen();
}

void drv_setup_io() {
	sure_6432_setup_io();
}

void drv_init() {
	sure_6432_init();
}	

#endif
