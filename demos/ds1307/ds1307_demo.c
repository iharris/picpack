//-----------------------------------------------------
// Pic Pack library
// 
// ds1307_demo.c
//
// Demonstration routines showing the use of the DS1307 
// real time clock
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
#include "ds1307.h"

// System includes
#include "string.h"
#include "stdlib.h"

// Don't forget your config.h!

#define DF1307_ADDRESS 0


// Put received characters in here to see
// if they make a command
char serial_buffer[6]= '\0';

// See if we need to do a task
uns8 do_task = 0;

// Flag if we received something
bit receive = 0;

// Interrupt routine - - - - - - - - - -
void interrupt() {
	
	serial_handle_rx_isr();
	serial_handle_tx_isr();
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
	// Neat trick to allow one button downloads
	//if (rec == MAGIC_BOOSTBLOADER_REQUEST) {
	//	boostbloader();
	//}	
	
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
			default: 
				if ((first_char == 's') && (strlen(serial_buffer)>2)) {
					uns8 setting = atoui_dec(&serial_buffer[2]);
					serial_print_str(" Setting to ");
					serial_print_int(setting);
					serial_putc(' ');
					switch (serial_buffer[1]) {
					case 's' :
						rtc_set_seconds(setting);
						serial_print_str("sec");
						break;
					case 'm' :
						rtc_set_minutes(setting);
						serial_print_str("min");
						break;
					case 'h' :
						rtc_set_hours(setting);
						serial_print_str("hour");
						break;
					case 'a' :
						rtc_set_day(setting);
						serial_print_str("day");
						break;
					case 't' :
						rtc_set_date(setting);
						serial_print_str("date");
						break;
					case 'o' :
						rtc_set_month(setting);
						serial_print_str("month");
						break;
					case 'y' :
						rtc_set_year(setting);
						serial_print_str("year");
						break;
					default:
						serial_print_str(" huh?");		
					}
					serial_putc('\n');	
				}	
				
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
	
	rtc_setup();
	
	serial_setup(BRGH_HIGH_SPEED, SPBRG_9600);

	turn_peripheral_ints_on();
	turn_global_ints_on();
}

	

void request_time() {

uns8 minutes, hours, seconds, day, date, month, year;
char buffer[10];

	minutes = rtc_get_minutes();
	hours   = rtc_get_hours();
	seconds = rtc_get_seconds();
	day 	= rtc_get_day();
	date 	= rtc_get_date();
	month 	= rtc_get_month();
	year 	= rtc_get_year();
	
	if (hours > 9) {
		buffer[0] = '0' + hours / 10;
		buffer[1] = '0' + hours % 10;
	} else {
		buffer[0] = ' ';
		buffer[1] = '0' + hours;
	}
	buffer[2] = ':';
	
	buffer[3] = '0' + minutes / 10;
	buffer[4] = '0' + minutes % 10;
	
	buffer[5] = ':';

	buffer[6] = '0' + seconds / 10;
	buffer[7] = '0' + seconds % 10;
	buffer[8] = 0;

	serial_print_str("The time is ");
	serial_print_str(buffer);
	
	serial_print_nl();
	
	serial_print_str("The day is ");
	
	switch (day) {
		case 1 : serial_print_str("Mon"); break;
		case 2 : serial_print_str("Tues"); break;
		case 3 : serial_print_str("Wednes"); break;
		case 4 : serial_print_str("Thurs"); break;
		case 5 : serial_print_str("Fri"); break;
		case 6 : serial_print_str("Satur"); break;
		case 7 : serial_print_str("Sun"); break;
	}
	serial_print_str("day\n");
	
	serial_print_str("The date is ");
	
	serial_print_int(date);
	serial_print_str(" of ");
	
	switch (month) {
		case 1 : serial_print_str("Jan"); break;
		case 2 : serial_print_str("Feb"); break;
		case 3 : serial_print_str("Mar"); break;
		case 4 : serial_print_str("Apr"); break;
		case 5 : serial_print_str("May"); break;
		case 6 : serial_print_str("Jun"); break;
		case 7 : serial_print_str("Jul"); break;
		case 8 : serial_print_str("Aug"); break;
		case 9 : serial_print_str("Sep"); break;
		case 10: serial_print_str("Oct"); break;
		case 11: serial_print_str("Nov"); break;
		case 12: serial_print_str("Dec"); break;
	}
	
	serial_print_str(" 20");
	if (year < 10) {
		serial_putc('0');
	}	
	serial_print_int(year);	
		
}

void request_config() {
	serial_print_str("Config = ");
	serial_print_int(rtc_get_config());
	serial_print_nl();
}

void handle_tasks() {
	switch (do_task) {
		case 1:
			serial_print_str("Requesting time.\n");
			request_time();
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

	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);

	serial_print_str("\n\nPIC terminal\n");
	serial_print_str( "\n<");
	serial_print_str(__TIME__);
	serial_putc(' ');
	serial_print_str(__DATE__);
	serial_print_str(">\n");

	serial_print_str("Commands:\n");
	serial_print_str("t - Request time\n");
	serial_print_str("sX05 - Set X to 05\n");
	serial_print_str("X=\n");
	serial_print_str("s sec\nm min\nh hour\n");
	serial_print_str("a day\nt date\no month\ny year\n");
	

	while(1) {

		if (serial_rx_avail()) {
			process_rx();
		}
		handle_tasks();	  

	}	// while (1)

	
}	// main

