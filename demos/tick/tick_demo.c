//-----------------------------------------------------
// Pic Pack library
// 
// tick_demo.c
//
// Demonstration routines for the interrupt driven
// tick timing functions
//
// Ian Harris 2007
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if you use it send me an email" license. Oh, and
// if it breaks, you get to keep both pieces.
//-----------------------------------------------------

// Pic Pack includes
#include "pic_utils.h"
#include "pic_serial.h"
#include "pic_tick.h"
#include "pic_timer.h"

// System includes
#include "string.h"
#include "stdlib.h"
#include "config.h"

// Don't forget your config.h!


// Put received characters in here to see
// if they make a command
char serial_buffer[6]= '\0';

// See if we need to do a task
uns8 do_task = 0;

// Flag if we received something
bit receive = 0;

// Interrupt routines - - - - - - - - - -

void timer_0_callback() {
	handle_tick();
}	

void interrupt() {
	
	timer_handle_0_isr();

	serial_handle_tx_isr();
	serial_handle_rx_isr();
	
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
			case 'p':
				do_task = 1;
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
	

	#ifdef _PIC16
		timer_setup_0(TIMER_8BIT_MODE, TIMER_PRESCALER_1_TO_8, 0xff - 250 - 1);	// 1ms at 8Mhz
	#endif

	#ifdef _PIC18
		timer_setup_0(TIMER_16BIT_MODE, TIMER_PRESCALER_OFF, 0xffff - 3000 + 13);	// 1ms at 12Mhz
		//timer_setup_0(TIMER_16BIT_MODE, TIMER_PRESCALER_1_TO_2, 0xffff - 1500 + 7);	// 1ms at 12Mhz
		//timer_setup_0(TIMER_16BIT_MODE, TIMER_PRESCALER_1_TO_8, 0xffff - 375 + 1);	// 1ms at 12Mhz
	#endif	 

	serial_setup(BRG_HIGH_SPEED, SPBRG_9600);

	turn_peripheral_ints_on();
	turn_global_ints_on();

}


// Handle any command line tasks we need to do

void handle_tasks() {
	switch (do_task) {
		case 1:
			serial_print_str("Current tick = ");
			serial_print_int(tick_get_count());
			serial_print_str("\n");
			break;
	} // switch		

	do_task = 0;	// clear teask
}

void main() {
	
uns16 tick_marker = 0;
uns16 test_tick;

	configure_system();
	
	delay_ms(200);
	
	serial_print_str("\n\nPicPack tick demo\n");
	serial_print_str( "\n<");
	serial_print_str(__TIME__);
	serial_putc(' ');
	serial_print_str(__DATE__);
	serial_print_str(">\n");

	serial_print_str("Commands: p to print tick value\n");

	timer_start_0();	// kick that timer off...
	
	while(1) {
		
		if (serial_rx_avail()) {	// if we have a character received
			process_rx();	// do our processing
		}
		handle_tasks();	// handle any commands user has typed in
		 
		test_tick = tick_get_count();	// find out what we're up to
		if (tick_calc_diff(tick_marker, test_tick) >= 1000) {	// 1000 - it's a second
			serial_print_str(" <Ting! ");
			serial_print_int(test_tick);
			serial_print_str("> ");	// print something out
			tick_marker = test_tick;	// reset to find next 1000
		}

	}	// while (1)
	
}	// main

