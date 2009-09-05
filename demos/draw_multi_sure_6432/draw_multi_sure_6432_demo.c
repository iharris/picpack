//-----------------------------------------------------
// Pic Pack library
// 
// draw_multi_sure_6432.c
//
// Demonstration routines showing the draw library
// used with the Sure Electronics 6432 board
//
// Ian Harris 2009
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if it breaks, you get to keep both pieces" license.
//-----------------------------------------------------

// Pic Pack includes
#include "config.h"
#include "pic_utils.h"
#include "pic_serial.h"
#include "draw\draw.h"
// System includes
#include "string.h"
#include "stdlib.h"
#include "sure_6432.h"
#include "draw\draw_screen_buffer.h"

long fpu_readLong(void)
{
    long t;
    t =12345;
    return t;
}

void flash() {
long x;
	x = fpu_readLong();
	lata.0 = 1;
	delay_ms(200);
	lata.0 = 0;
	delay_ms(200);
}	

// Interrupt routine - - - - - - - - - -
void interrupt() {
	
	serial_handle_tx_isr();
	serial_handle_rx_isr();
}


// configure_system
//
// Get the pic ready for everything we want to do

void configure_system() {
	    
	kill_interrupts();	// turn off interrupts just in case

	turn_analog_inputs_off();	// kill those pesky analogue inputs
	
	
	serial_setup(BRGH_HIGH_SPEED, SPBRG_19200);

	turn_peripheral_ints_on();
	turn_global_ints_on();

	draw_setup_io();
	draw_init();

}




void main() {

	trisa = 0;
	flash();
	configure_system();
	flash();
	delay_ms(100);

	serial_print_str("\n\nPicPack Sure 6432 Led display demo\n");
	serial_print_str( "\n<");
	serial_print_str(__TIME__);
	serial_putc(' ');
	serial_print_str(__DATE__);
	serial_print_str(">\n");

	serial_print_str("total buffer = ");
	serial_print_int(DRAW_TOTAL_BUFFER_SIZE);
	serial_print_nl();
	serial_print_str("wide = ");
	serial_print_int(DRAW_PIXELS_WIDE);
	serial_print_nl();
	serial_print_str("high = ");
	serial_print_int(DRAW_PIXELS_HIGH);
	serial_print_nl();
	serial_print_str("ppb = ");
	serial_print_int(DRAW_PIXELS_PER_BYTE);
	serial_print_nl();
	while (1) {
		/*delay_ms(250);
		
		draw_set_pixel(0,0,1);
		draw_paint();
		
		delay_s(1);
		draw_set_pixel(23,0,1);
		draw_paint();
		
		delay_s(1);
		draw_set_pixel(23,15,1);
		draw_paint();

		delay_s(1);
		draw_set_pixel(0,15,1);
		draw_paint();

		delay_s(1);
		draw_set_pixel(0,0,0);
		draw_paint();

		delay_s(1);
		draw_set_pixel(23,0,0);
		draw_paint();
		
		delay_s(1);
		draw_set_pixel(23,15,0);
		draw_paint();

		delay_s(1);
		draw_set_pixel(0,15,0);
		draw_paint();

		delay_s(1);
		draw_line(0, 0, 23, 15, 1);
		draw_paint();

		delay_s(1);
		draw_line(0, 15, 23, 0, 1);
		draw_paint();

		delay_s(1);
		draw_line(0, 0, 23, 15, 0);
		draw_paint();

		delay_s(1);
		draw_line(0, 15, 23, 0, 0);
		draw_paint();

		delay_s(1);
		draw_circle(12, 8, 1, 1);
		draw_paint();

		delay_s(1);
		draw_circle(12, 7, 2, 1);
		draw_paint();

		delay_s(1);
		draw_circle(12, 7, 3, 1);
		draw_paint();

		delay_s(1);
		draw_circle(12, 7, 4, 1);
		draw_paint();
		delay_s(1);
		draw_circle(12, 7, 5, 1);
		draw_paint();
		delay_s(1);
		draw_circle(12, 7, 6, 1);
		draw_paint();
		delay_s(1);
		draw_circle(12, 7, 7, 1);
		draw_paint();
		
		delay_s(1);*/
		draw_clear_screen();
		serial_print_str("Rectangle\n");
		draw_rect(1, 1, DRAW_PIXELS_WIDE - 1 -1 , DRAW_PIXELS_HIGH - 1 - 1, 2);
		/*draw_set_pixel(0,0,1);
		draw_set_pixel(DRAW_PIXELS_WIDE - 1, 0, 1);
		draw_set_pixel(DRAW_PIXELS_WIDE - 1, DRAW_PIXELS_HIGH - 1, 1);
		draw_set_pixel(0, DRAW_PIXELS_HIGH - 1, 1);
			*/	
		uns16 countp;
		for (countp = 0; countp < 200; countp++) {
			draw_paint();
		}	
		
		draw_clear_screen();
		serial_print_str("Rectangle\n");
		draw_rect(1, 1, DRAW_PIXELS_WIDE - 1 -1, DRAW_PIXELS_HIGH - 1 - 1, 1);
		draw_circle(31, 31, 30, 3);
		for (countp = 0; countp < 1000; countp++) {
			draw_paint();
		}	
		

		

	}
	
}	// main

