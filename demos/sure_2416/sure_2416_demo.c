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
#include "lm75.h"
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



uns8 convert_3bits_to_2dec(uns8 data) {
	
	switch (data) {
		case 0: return 00;
		case 1: return 13;
		case 2: return 25;
		case 3: return 38;
		case 4: return 50;
		case 5: return 63;
		case 6: return 75;
		case 7: return 88;
	}	
	return 99;
}

uns8 convert_3bits_to_1dec(uns8 data) {
	
	switch (data) {
		case 0: return 0;
		case 1: return 1;
		case 2: return 3;
		case 3: return 4;
		case 4: return 5;
		case 5: return 6;
		case 6: return 8;
		case 7: return 9;
	}	
	return 99;
}



// Print to the lcd the decoded temperature
void print_lcd_temp(uns16 raw_temp) {
uns8 int_part, dec_part;

	// Pull apart integer from decimal values
	int_part = raw_temp >> 8;
	dec_part = (raw_temp & 0xff) >> 5;
	
	// Print result to two decimal places
	lcd_write_data_int(int_part);
	lcd_write_data_str(".");
	lcd_write_data_int(convert_3bits_to_1dec(dec_part));
	lcd_write_data_str("\xDF ");
}

// Request temperature from sensor and print it out

void request_temp() {

uns16 raw_temp;


	raw_temp = lm75_get_temp(LM75_ADDRESS); 
	
	lcd_write_command(LCD_LINE1);	// goto line 1
	lcd_write_data_str("Current = ");	// print welcome message

	print_lcd_temp(raw_temp);
	
	if (raw_temp > max_temp) {
		max_temp = raw_temp;
	}
	if (raw_temp < min_temp) {
		min_temp = raw_temp;
	}
	lcd_write_command(LCD_LINE2);
	
	print_lcd_temp(min_temp);
	delay_ms(10);
	lcd_write_data_str(" > ");
	print_lcd_temp(max_temp);
	
		
}



// configure_system
//
// Get the pic ready for everything we want to do

void configure_system() {
	    
	kill_interrupts();	// turn off interrupts just in case

	turn_analog_inputs_off();	// kill those pesky analogue inputs
	
	lm75_setup();
	lm75_set_config(LM75_ADDRESS, LM75_NORMAL);
	
	lcd_setup();	// Setup the pins (output / input)
	lcd_init ();	// Configure chip ready for display

	serial_setup(BRGH_HIGH_SPEED, SPBRG_9600);

	turn_peripheral_ints_on();
	turn_global_ints_on();
}




void main() {
	
	configure_system();

	delay_ms(100);

	serial_print_str("\n\nPIC LM75 + LCD demo\n");
	serial_print_str( "\n<");
	serial_print_str(__TIME__);
	serial_putc(' ');
	serial_print_str(__DATE__);
	serial_print_str(">\n");

	while (1) {
		request_temp();
		delay_s(2);
	}
	
}	// main

