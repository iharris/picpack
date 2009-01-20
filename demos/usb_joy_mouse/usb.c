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
#include "pic_tick.h"
#include "pic_timer.h"

// System includes
#include "string.h"
#include "stdlib.h"
#include "config.h"

// Config for 20Mhz crystal, FS USB (half 96Mhz=48Mhz) CPU clock at 16Mhz (96Mhz/6) 
// Only clock CPU at 16Mhz since this is about the maximum at 3v. :-(

#pragma DATA	_CONFIG1L, _PLLDIV_5_1L & _CPUDIV_OSC4_PLL6_1L & _USBDIV_2_1L
#pragma DATA    _CONFIG1H, _FOSC_HSPLL_HS_1H & _FCMEN_OFF_1H & _IESO_OFF_1H

#pragma DATA    _CONFIG2L, _PWRT_ON_2L & _BOR_OFF_2L  & _VREGEN_OFF_2L
#pragma DATA    _CONFIG2H, _WDT_OFF_2H & _WDTPS_128_2H
#pragma DATA    _CONFIG3H, _CCP2MX_OFF_3H & _LPT1OSC_OFF_3H & _PBADEN_OFF_3H & _MCLRE_ON_3H
#pragma DATA    _CONFIG4L, _STVREN_ON_4L & _LVP_OFF_4L & _DEBUG_OFF_4L & _XINST_OFF_4L
#pragma DATA    _CONFIG5L, _CP0_OFF_5L & _CP1_OFF_5L & _CP2_OFF_5L & _CP3_OFF_5L
#pragma DATA    _CONFIG5H, _CPB_OFF_5H & _CPD_OFF_5H
#pragma DATA    _CONFIG6L, _WRT0_OFF_6L & _WRT1_OFF_6L & _WRT2_OFF_6L & _WRT3_OFF_6L
#pragma DATA    _CONFIG6H, _WRTC_OFF_6H & _WRTB_OFF_6H & _WRTD_OFF_6H
#pragma DATA    _CONFIG7L, _EBTR0_OFF_7L & _EBTR1_OFF_7L & _EBTR2_OFF_7L & _EBTR3_OFF_7L
#pragma DATA    _CONFIG7H, _EBTRB_OFF_7H


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
void timer_0_callback() {
	handle_tick();
}	

	
void interrupt() {
	
		serial_handle_tx_isr();
		serial_handle_rx_isr();
		usb_handle_isr();
		timer_handle_0_isr();
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
			case 'u':
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
	
	serial_setup(BRGH_HIGH_SPEED, SPBRG_57600);

	timer_setup_0(TIMER_16BIT_MODE, TIMER_PRESCALER_OFF, 0xffff - 4000); // close enough to 1ms at 16Mhz
	
	clear_bit(intcon2, RBPU); // turn on weak pull-ups

	make_input(JOY_PORT, UP_PIN); 
	make_input(JOY_PORT, DOWN_PIN); 
	make_input(JOY_PORT, LEFT_PIN); 
	make_input(JOY_PORT, RIGHT_PIN); 
	make_input(JOY_PORT, CENTER_PIN); 
	
	usb_setup();

	turn_peripheral_ints_on();
	turn_usb_ints_on();
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
			serial_print_str("Up!\n");
			delay_ms(100);
			signed char buffer[3];
			buffer[0] = 0;
			buffer[1] = -1;
			buffer[2] = -1;
			usb_send_data(1, (uns8 *)&buffer, 3, /*first*/ 0); // ep 1
			break;
		case 4:
			serial_print_str("You did task 4. Wow.\n");
			break;
	} // switch		

	do_task = 0;
}




void main() {

uns16 tick_marker = 0;
uns16 test_tick;
uns8	up_count,
		down_count,
		right_count,
		left_count,
		center_count,
		check_count;
signed char buffer[3];
			
	configure_system();
	delay_ms(100);
	serial_print_str("\n\nPIC USB JoyMouse demo\n");
	serial_print_str( "\n<");
	serial_print_str(__TIME__);
	serial_putc(' ');
	serial_print_str(__DATE__);
	serial_print_str(">\n");

	serial_print_str("Commands: a attach to USB bus\n");
	
	timer_start_0();	// kick that timer off...
	
	left_count = right_count = up_count = down_count = center_count = check_count = 0;
	
	while(1) {

		if (serial_rx_avail()) {
			process_rx();
		}
		handle_tasks();	
		
		test_tick = tick_get_count();	// find out what we're up to
		if (tick_calc_diff(tick_marker, test_tick) > 3) {	// every 3 ms
			tick_marker = test_tick;
			if (!test_pin(JOY_PORT, UP_PIN)) {
				up_count++;
			}	
			if (!test_pin(JOY_PORT, DOWN_PIN)) {
				down_count++;
			}	
			if (!test_pin(JOY_PORT, LEFT_PIN)) {
				left_count++;
			}	
			if (!test_pin(JOY_PORT, RIGHT_PIN)) {
				right_count++;
			}	
			if (!test_pin(JOY_PORT, CENTER_PIN)) {
				center_count++;
			}
			check_count++;
			if (check_count == 5) {
				check_count = 0;
				if (usb_get_state() == st_CONFIGURED) {
					
					if (center_count) {
						buffer[0] = 1;
					} else {	
						buffer[0] = 0;
					}	
					buffer[1] = 0 - left_count + right_count;	
					buffer[2] = 0 - up_count + down_count;
					if (buffer[0] | buffer[1] | buffer[2])
						usb_send_data(1, (uns8 *)&buffer, 3, /*first*/ 0); // ep 1

				}
				left_count = right_count = up_count = down_count = center_count = 0;
			}	
		}	
  

	}	// while (1)
	// main
}