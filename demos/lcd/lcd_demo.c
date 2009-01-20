//-----------------------------------------------------
// Pic Pack library
// 
// serial_demo.c
//
// Demonstration routines for the interrupt driven
// way funky serial library
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
#include "config.h"
#include "lcd.h"

// System includes
#include "string.h"
#include "stdlib.h"

// Don't forget your config.h!


// Put received characters in here to see
// if they make a command
char serial_buffer[17] = '\0';

// See if we need to do a task
uns8 do_task = 0;

// Flag if we received something
bit receive = 0;


// Interrupt routine - - - - - - - - - -

void interrupt() {
	serial_handle_tx_isr();
	serial_handle_rx_isr();
}



void process_rx() {
	uns8 len, rec;
	uns8 temp;
	rec = serial_getc();
	serial_putc(rec);
	if (rec == '\r') {
		temp = serial_buffer[0];
		switch (temp) {
			case 'c':
				do_task = 1;
				break;
		    case 'h':
				do_task = 2;
				break;
		    case '1':
				do_task = 3;
				break;
			case '2':
				do_task = 4;
				break;
			default:
				if (temp == 't') {
					serial_print_str("Printing to LCD: ");
					serial_print_str(&serial_buffer[1]);
					serial_putc('\n');
					lcd_write_data_str(&serial_buffer[1]);
				};
		}		
		serial_buffer[0] = '\0';
        serial_putc('\n');
        serial_putc('>');			
	} else {
		len = strlen(serial_buffer);
		serial_buffer[len] = rec;
		serial_buffer[len+1] = '\0';
	}	

}

void configure_system() {
	    
	kill_interrupts();

	turn_analog_inputs_off();
	
	serial_setup(BRGH_HIGH_SPEED, SPBRG_9600);

	lcd_setup();	// Setup the pins (output / input)
	lcd_init ();	// Configure chip ready for display
	
	turn_peripheral_ints_on();
	turn_global_ints_on();

}





// Handle any command line tasks we need to do
// Very handy to print out variables, do commands,
// print out some debug, etc...



void handle_tasks() {
	
	
	switch (do_task) {
		case 1:
			serial_print_str("Clearing LCD\n");
			lcd_write_command(LCD_CLEAR_DISP);
			break;
		case 2:
			serial_print_str("Cursor home\n");
			lcd_write_command(LCD_RETURN_HOME);
			break;
		case 3:
			serial_print_str("Cursor to line 1\n");
			lcd_write_command(LCD_LINE1);
			break;
		case 4:
			serial_print_str("Cursor to line 2\n");
			lcd_write_command(LCD_LINE2);
			break;
	} // switch		

	do_task = 0;
}

void main() {
	
	configure_system();
	delay_ms(100);
	serial_print_str("\n\nPIC LCD test\n");
	serial_print_str( "\n<");
	serial_print_str(__TIME__);
	serial_putc(' ');
	serial_print_str(__DATE__);
	serial_print_str(">\n");

	serial_print_str("Commands:\nc - clear display\nh - cursor home\n");
	serial_print_str("1 - Cursor to line 1\n2 - Cursor to line 2\n");
	serial_print_str("tstring - print string to LCD\n");

	lcd_write_command(LCD_CLEAR_DISP);	// clear the display
	lcd_write_command(LCD_RETURN_HOME);	// cursor back to the beginning
	lcd_write_command(LCD_LINE1);	// goto line 1
	lcd_write_data_str("PicPack LCD demo");	// print welcome message


	while (1) {

		if (serial_rx_avail()) {
			process_rx();
		}
		handle_tasks();	

	}	// while (1)

	
}	// main

