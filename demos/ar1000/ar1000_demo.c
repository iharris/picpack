//-----------------------------------------------------
// Pic Pack library
// 
// ar1000_demo.c
//
// Demonstration routines showing the use of the AR1000
// FM radio module
// 
// Using the Sure Electronics PicDem 2 board 
//
// Ian Harris 2009
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
#include "ar1000.h"
#include "lcd.h"

// System includes
#include "string.h"
#include "stdlib.h"

// Don't forget your config.h!

#define LM75_ADDRESS 0

// max and min temps

uns16 max_temp = 0;
uns16 min_temp = 0xffff;

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
	
	ar1000_setup_io();
	
	lcd_setup();	// Setup the pins (output / input)
	lcd_init ();	// Configure chip ready for display

	serial_setup(BRGH_HIGH_SPEED, SPBRG_19200);

	turn_peripheral_ints_on();
	turn_global_ints_on();
}




void main() {
	
	configure_system();

	delay_ms(100);

	serial_print_str("\n\nPIC AR1000 + LCD demo\n");
	serial_print_str( "\n<");
	serial_print_str(__TIME__);
	serial_putc(' ');
	serial_print_str(__DATE__);
	serial_print_str(">\n");

	while (1) {
	}
	
}	// main

