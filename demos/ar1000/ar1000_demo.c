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


// System includes
#include "string.h"
#include "stdlib.h"

// Don't forget your config.h!

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

	// neat trick to allow one button downloads
	if (rec == MAGIC_BOOSTBLOADER_REQUEST) {
		boostbloader();
	}	
	
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



// Init by sending 0x01 to 0x11 then 0x00
// d15-d8
// then d7 - d0

/* Tune
Set hmute Bit
Clear TUNE Bit
Clear SEEK Bit
Set BAND/SPACE/CHAN Bits
Enable TUNE Bit
Wait STC flag (Seek/Tune Complete, in “Status” register)
Clear hmute Bit
Update Functions (optional)

*/

/* seek
Set hmute Bit
Clear TUNE Bit
Set CHAN Bits
Clear SEEK Bit
Set SEEKUP/SPACE/BAND/SEEKTH Bits
Enable SEEK Bit
Wait STC flag (Seek/Tune Complete, in “Status” register)
Clear hmute Bit
Update Functions (optional, but remember to update CHAN with the seek in
READCHAN before next seek)
*/
// Handle any command line tasks we need to do
// Very handy to print out variables, do commands,
// print out some debug, etc...

void handle_tasks() {
	switch (do_task) {
		case 1:
			serial_print_str("Task 1: send_to=");
			serial_print_int(send_to);
			serial_print_str("\n");
			break;
		case 2:
			serial_print_str("Get Reg=0x\n");
			uns16 res = ar1000_read_register(send_to);
			serial_print_int_hex_16bit(res);
			serial_print_nl();
			break;
		case 3:
			serial_print_str("Reg all\n");
			for (uns8 count = 0; count < 0x20; count++) {
				uns16 res = ar1000_read_register(count);
				serial_print_int(count);
				serial_print_str("=");
				serial_print_int_hex_16bit(res);
				serial_print_nl();
			}	
			break;
		case 4:
			serial_print_str("Seek\n");
			ar1000_seek(1); // seek up
			break;
	} // switch		

	do_task = 0;
}



// configure_system
//
// Get the pic ready for everything we want to do

void configure_system() {
	    
	kill_interrupts();	// turn off interrupts just in case

	turn_analog_inputs_off();	// kill those pesky analogue inputs
	
	
	serial_setup(BRGH_HIGH_SPEED, SPBRG_19200);

	ar1000_setup();
	
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

	while(1){

		if (serial_rx_avail()) {
			process_rx();
		}
		handle_tasks();	  

	}	// while (1)
}	// main

