/*! \file usb_cdc_class.h
    \brief USB Communications Device Class (Serial Port) routines 

	It's the way cool interrupt driven USB Serial port library

	Ian Harris 2009
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if it breaks, you get to keep both pieces" license.
    
*/

#ifndef __usb_cdc_class_h
#define __usb_cdc_class_h

/**
	\brief Sends a single character to the USB serial port
	
	Deliver a single character out the virtual serial port.
*/
void usb_cdc_putc(uns8 c);
uns8 usb_cdc_getc();
void usb_cdc_handle_tx();
uns8 usb_cdc_rx_avail();
uns8 usb_cdc_tx_empty();
void usb_cdc_print_str(char *str);
void usb_cdc_print_int(uns16 i);
void usb_cdc_setup();

#endif