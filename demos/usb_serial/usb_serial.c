//-----------------------------------------------------
// Pic Pack library
// 
// usb_serial.c
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
#include "pic_usb.h"
#include "pic_tick.h"
#include "pic_timer.h"

#include "usb_cdc_class.h"

// System includes
#include "string.h"
#include "stdlib.h"
#include "config.h"

// If you'd like USB debug info out the serial connection, uncomment this line:
//#define USB_SERIAL_DEBUG
// You also might like to uncomment the USB_DEBUG and CDC_DEBUG lines in the config.h

#ifdef USB_SERIAL_DEBUG
	#include "pic_serial.h"
#endif

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


// a check to see if the usb connection has been configured yet
uns8 usb_configured = 0;

// Interrupt routine - - - - - - - - - -
void timer_0_callback() {
	handle_tick();
}	

	
void interrupt() {
	
	#ifdef USB_SERIAL_DEBUG
		serial_handle_tx_isr();
		serial_handle_rx_isr();
	#endif
	
	usb_handle_isr();
	timer_handle_0_isr();
}


void usb_device_configured_callback() {
	usb_configured = 1;
}	

// Process_rx
//
// Keep adding characters to our input buffer
// when we enter a command, then act on it

void process_rx() {
	
	uns8 len,	// length of string
		 rec;	// received character
	uns8 first_char;
	
	rec = usb_cdc_getc();	// get the character from the fifo
	
	usb_cdc_putc(rec);	// print it out so we can see what we typed

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
					usb_cdc_print_str(" Set send_to to ");
					usb_cdc_print_int(send_to);	// Of course, this could be anything
					usb_cdc_putc('\n');
				};
		}		
		serial_buffer[0] = '\0';	// clear the buffer
        usb_cdc_putc('\n');	// print new line
        usb_cdc_putc('>');	// print the prompt
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
	
	#ifdef USB_SERIAL_DEBUG 
		serial_setup(BRGH_HIGH_SPEED, SPBRG_57600);
	#endif
	
	timer_setup_0(TIMER_16BIT_MODE, TIMER_PRESCALER_OFF, 0xffff - 4000); // close enough to 1ms at 16Mhz
	
	usb_cdc_setup();

	usb_setup();
	
	turn_usb_ints_on();
	turn_peripheral_ints_on();
	turn_global_ints_on();
}

// Handle any command line tasks we need to do
// Very handy to print out variables, do commands,
// print out some debug, etc...

void handle_tasks() {
	switch (do_task) {
		case 1:
			usb_cdc_print_str("Task 1!\n");
			break;
		case 2:
			usb_cdc_print_str("You did task 2\n");
			break;
		case 3:
			usb_cdc_print_str("Task three\n");
			break;
		case 4:
			usb_cdc_print_str("Sending a string\n");
			usb_cdc_print_str("Hello and welcome to the thunderdome");
			break;
	} // switch		

	do_task = 0;
}




void main() {

uns16 tick_marker = 0;
uns16 test_tick;

		
	configure_system();
	delay_ms(100);
    #ifdef USB_SERIAL_DEBUG
		serial_print_str("USB here we go...");
    #endif
    delay_ms(100);
	usb_enable_module();
	while (usb_configured == 0) {
		#ifdef USB_SERIAL_DEBUG
			serial_print_str("<wait>");
		#endif
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
	}
	#ifdef USB_SERIAL_DEBUG
		serial_print_str("\nGreat! Now trying to print\n");
	#endif
	usb_cdc_print_str("\n\nPIC USB Serial demo\n");
	usb_cdc_print_str( "\n<");
	usb_cdc_print_str(__TIME__);
	usb_cdc_putc(' ');
	usb_cdc_print_str(__DATE__);
	usb_cdc_print_str(">\n");

	usb_cdc_print_str("Commands: a attach to USB bus\n");
	
	timer_start_0();	// kick that timer off...
	
	while(1) {

		if (usb_cdc_rx_avail()) {
			process_rx();
		}
		handle_tasks();	
		
		test_tick = tick_get_count();	// find out what we're up to
		if (tick_calc_diff(tick_marker, test_tick) > 3) {	// every 3 ms
			tick_marker = test_tick;
		}	
  

	}	// while (1)
	// main
}