//-----------------------------------------------------
// Pic Pack library
// 
// lm75_lcd_demo.c
//
// Demonstration routines showing the use of the LM75
// temperature sensor
//
// Ian Harris 2008
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if you use it send me an email" license. Oh, and
// if it breaks, you get to keep both pieces.
//-----------------------------------------------------

// Pic Pack includes
#include "config.h"
#include "pic_utils.h"
#include "pic_serial.h"
#include "lcd.h"
#include "sure_2416.h"

// System includes
#include "string.h"
#include "stdlib.h"


// max and min temps


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
	make_output(PORTB, 0);
	make_output(PORTB, 1);
	make_output(PORTB, 2);
	make_output(PORTB, 3);
	
	set_pin(PORTB, 0);
	set_pin(PORTB, 1);
	set_pin(PORTB, 2);
	set_pin(PORTB, 3);
	delay_s(1);
	clear_pin(PORTB, 0);
	clear_pin(PORTB, 1);
	clear_pin(PORTB, 2);
	clear_pin(PORTB, 3);
	delay_s(1);
	
	lcd_setup();	// Setup the pins (output / input)
	lcd_init ();	// Configure chip ready for display

	sure_2416_setup();
	
	serial_setup(BRGH_HIGH_SPEED, SPBRG_19200);

	turn_peripheral_ints_on();
	turn_global_ints_on();

	sure_2416_init();
}




void main() {
	
	configure_system();

	delay_ms(100);

	serial_print_str("\n\nPicPack Sure 2416 Led display demo\n");
	serial_print_str( "\n<");
	serial_print_str(__TIME__);
	serial_putc(' ');
	serial_print_str(__DATE__);
	serial_print_str(">\n");
			sure_2416_write(0,0b00001111);

	while (1) {
		//delay_ms(250);
		serial_print_str("writing...\n");
		sure_2416_write(0,0b00001111);
		/*delay_s(5);
		sure_2416_set_pixel(0,0,1);

		delay_s(1);
		sure_2416_set_pixel(23,0,1);
		
		delay_s(1);
		sure_2416_set_pixel(23,15,1);

		delay_s(1);
		sure_2416_set_pixel(0,15,1);

		delay_s(1);
		sure_2416_set_pixel(0,0,0);

		delay_s(1);
		sure_2416_set_pixel(23,0,0);
		
		delay_s(1);
		sure_2416_set_pixel(23,15,0);

		delay_s(1);
		sure_2416_set_pixel(0,15,0);*/
		uns8 x,y;
		for(x = 0 ; x < 24 ; x++) {
			for(y = 0 ; y < 16 ; y++) {
				sure_2416_set_pixel(x, y, 1);
				//sure_2416_set_pixel(x, y, 0);
			}
		}		
		delay_ms(100);
		for(x = 0 ; x < 24 ; x++) {
			for(y = 0 ; y < 16 ; y++) {
				//sure_2416_set_pixel(x, y, 1);
				sure_2416_set_pixel(x, y, 0);
			}
		}		
		delay_ms(100);

	}
	
}	// main

