//-----------------------------------------------------
// Pic Pack library
// 
// usb.c
//
// Demonstration routines for the interrupt driven
// usb library
//
// Ian Harris 2008
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if it breaks, you get to keep both pieces" license.
//-----------------------------------------------------


// Pic Pack includes
#include "config.h"
#include "pic_utils.h"
#include "pic_serial.h"
#include "pic_usb.h"

// System includes
#include "string.h"
#include "stdlib.h"
#include "config.h"

// Put received characters in here to see
// if they make a command
char serial_buffer[6]= '\0';

// See if we need to do a task
uns8 do_task = 0;

// Flag if we received something
bit receive = 0;

// Dummy variable to show we can set it 
uns8 send_to = 0;

// Interrupt routine - - - - - - - - - -

void interrupt() {
	
		usb_handle_isr();
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
			case 'a':
				do_task = 1;
				break;
		    case 'x':
				do_task = 2;
				break;
			case 'c':
				do_task = 3;
				break;
			case 'n':
				do_task = 4;
				break;
			default:
				// More complicated example
				// Let's set a variable based on input
				// You can type:
				// s05
				// And press enter, it will set "send_to" to this (in hex)
				if ((first_char == 's') && (strlen(serial_buffer)==3)) {
					send_to = atoui_hex(&serial_buffer[1]);
					serial_print_str(" Set send_to to ");
					serial_print_int(send_to);	// Of course, this could be anything
					serial_putc('\n');
				};
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
	
	serial_setup(BRGH_HIGH_SPEED, SPBRG_19200);

	usb_init();
	turn_peripheral_ints_on();
	turn_global_ints_on();
}


// Handle any command line tasks we need to do
// Very handy to print out variables, do commands,
// print out some debug, etc...

void handle_tasks() {
	switch (do_task) {
		case 1:
			serial_print_str("USB on");
			serial_print_str("\n");
			delay_ms(100);
			usb_enable_module();
			break;
		case 2:
			serial_print_str("You did task 2\n");
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

	serial_print_str("Commands: a,x,c,n,s00\n");

// a,x,c,n just press the key and enter
// s00 sets the variable "send_to" to the hex value you enter


	while(1){

		if (serial_rx_avail()) {
			process_rx();
		}
		handle_tasks();	  

	}	// while (1)
	//ADCON1 = 0x0F;								//PORTA all digital operation
/*	make_output(PORTC, 2); //TRISCbits.TRISC2 = 0;						//set RC2 an output
	latd = 0x00;								//use LED1 - LED4 as a simple counter
	trisd = 0x00; 								//configure PORTD for output

	//For PIC18LF4550-Eval-Rev4A hardware with 74HC573 latch
	make_output(PORTC, 0); //TRISCbits.TRISC0 = 0;
	latc.0 = 1; //LATCbits.LATC0  = 1;			//un-latch 74HC573 to make OUTD follows LATD
	make_output(PORTC, 1); //TRISCbits.TRISC1 = 0;						//enable the latch
	latc.1 = 0; //LATCbits.LATC1 = 0; /~OE

	while(1)
	{
			for(uns8 counter = 0 ; counter <16; counter++){
			latd = counter;
			delay_s(1);					//software delay 100*200 machine cycle=>20,000*(0.8us)=16ms
												//dependency: delays.h under ..\mcc18\h
			}
					
	}
	*/
}	// main

