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
	
	
	lcd_setup();	// Setup the pins (output / input)
	lcd_init ();	// Configure chip ready for display

	sure_2416_setup();

	serial_setup(BRGH_HIGH_SPEED, SPBRG_19200);

	turn_peripheral_ints_on();
	turn_global_ints_on();
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
	sure_2416_send_command(0b00000000);	// sys dis 
	sure_2416_send_command(0b00101100); // format pmos 16 outs
	
	sure_2416_send_command(0b00010100); //master
	sure_2416_send_command(0b00000001); // sys en
	sure_2416_send_command(0b00000011); //led on
	
	while (1) {
		delay_ms(250);
		serial_print_str("writing...\n");
		sure_2416_write(0,0b00001111);
	}
	
}	// main

