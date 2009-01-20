//-----------------------------------------------------
// Pic Pack library
// 
// lm75_demo.c
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


// System includes
#include "string.h"
#include "stdlib.h"

// Don't forget your config.h!

#define LM75_ADDRESS 0


// Put received characters in here to see
// if they make a command
char serial_buffer[6]= '\0';

// See if we need to do a task
uns8 do_task = 0;

// Flag if we received something
bit receive = 0;

// Interrupt routine - - - - - - - - - -
void interrupt() {
	
	serial_handle_tx_isr();
	serial_handle_rx_isr();
}


void request_config() {

uns8 config_reg;

	config_reg = lm75_get_config(LM75_ADDRESS); 
	
	// Print out raw temp data
	serial_print_str("Config = 0x");
	serial_print_int_hex(config_reg);
	serial_print_nl();
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




// Request temperature from sensor and print it out

void request_temp() {

uns16 raw_temp;
uns8 int_part, dec_part;


	raw_temp = lm75_get_temp(LM75_ADDRESS); 
	
	// Print out raw temp data
	serial_print_str("Raw temp = 0x");
	serial_print_int_hex(raw_temp >> 8);
	serial_print_int_hex(raw_temp & 0xff);
	serial_print_nl();

	// Pull apart integer from decimal values
	int_part = raw_temp >> 8;
	dec_part = (raw_temp & 0xff) >> 5;
	
	// Print result to two decimal places
	serial_print_str("Temp = ");
	serial_print_int(int_part);
	serial_putc('.');
	serial_print_int(convert_3bits_to_2dec(dec_part));
	serial_print_nl();

	// Print result to one decimal places
	serial_print_str("Temp = ");
	serial_print_int(int_part);
	serial_putc('.');
	serial_print_int(convert_3bits_to_1dec(dec_part));
	serial_print_nl();
}


// Process_rx
//
// Keep adding characters to our input buffer
// when we enter a command, then act on it

void process_rx() {
	
	uns8 len,	// length of string
		 rec;	// received character
	uns8 first_char;
	
	rec = serial_getc();	// get the character from the fifo
	
	serial_putc(rec);	// print it out so we can see what we typed

	if (rec == '\r') {	// did we press return?
		first_char = serial_buffer[0];
		switch (first_char) {
			case 't':
				do_task = 1;
				break;
		    case 'c':
				do_task = 2;
				break;
		}		
		serial_buffer[0] = '\0';	// clear the buffer
        serial_putc('\n');	// print new line
        serial_putc('>');	// print the prompt
	} else {
		len = strlen(serial_buffer);	// add the character to our line input
		serial_buffer[len] = rec;
		serial_buffer[len+1] = '\0';
	}

}

// configure_system
//
// Get the pic ready for everything we want to do

void configure_system() {
	    
	kill_interrupts();	// turn off interrupts just in case

	turn_analog_inputs_off();	// kill those pesky analogue inputs
	
	lm75_setup();
	lm75_set_config(LM75_ADDRESS, LM75_NORMAL);
	
	serial_setup(BRGH_HIGH_SPEED, SPBRG_9600);

	turn_peripheral_ints_on();
	turn_global_ints_on();
}



void handle_tasks() {
	switch (do_task) {
		case 1:
			serial_print_str("Requesting temp.\n");
			request_temp();
			serial_print_str("\n>");
			break;
		case 2:
			serial_print_str("Requesting config\n");
			request_config();
			serial_print_str("\n>");
			break;
		case 3:
			serial_print_str("You did task 3\n");
			break;
		case 4:
			serial_print_str("You did task 4. Wow.\n");
			break;
	} // switch		

	do_task = 0;
}

void main() {
	
	configure_system();

	delay_ms(100);

	serial_print_str("\n\nPIC terminal\n");
	serial_print_str( "\n<");
	serial_print_str(__TIME__);
	serial_putc(' ');
	serial_print_str(__DATE__);
	serial_print_str(">\n");

	serial_print_str("Commands:\n");
	serial_print_str("t - Request temp\n");
	serial_print_str("c - Request config\n");

	while(1){

		if (serial_rx_avail()) {
			process_rx();
		}
		handle_tasks();	  

	}	// while (1)

	
}	// main

