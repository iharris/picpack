/*! \file usb_cdc_class.h
    \brief USB Communications Device Class (Serial Port) routines 

	It's the way cool interrupt driven USB Serial port library

	Ian Harris 2009
	imharris [at] gmail.com

	<a href="http://embeddedadventures.blogspot.com"> Website Link </a>

	Released under the "do whatever you like with this
	but if it breaks, you get to keep both pieces" license.
    
*/

#ifndef __usb_cdc_class_h
#define __usb_cdc_class_h

/**
	\brief Sends a single character to the USB serial port
	
	Deliver a single character out the virtual serial port.  This routine will add
	the character to the transmit buffer.  The actual buffer will be physically sent on
	the Start Of Frame (SOF) interrupt (each 1ms) or on the end point interrupt - ie, when the last character or chunk
	of characters was sent.

	\param c The 8 bit byte to be transmitted.
*/
void usb_cdc_putc(uns8 c);

/**
	\brief Retrieve a received character from the USB serial port
	
	Receive a character from the USB serial port.  If a character has
	not been received, this routine will wait indefinately.

	\return Byte from the receive buffer.

*/
uns8 usb_cdc_getc();

/**
	\brief Check to see if a character is available in the receive buffer
	
	If one or more bytes are available in the USB serial port receive buffer, this 
	routine will return true.  If there are no bytes available, it will return false.
	
	\return True if buffer is not empty, False if buffer is empty.
*/
uns8 usb_cdc_rx_avail();

/**
	\brief Check to see if the transmit buffer is empty
	
	Sometimes it is useful to see if the transmit buffer is empty,
	since then you can be sure your data is well on its way.
	In the case of USB, this means that the data has been at least placed
	into the outbound USB buffer; it's not possible to tell until after the fact
	if the data has actually been squirted out the USB port.
	
	\return True if transmit buffer is empty, False if buffer still has data in it.
*/
uns8 usb_cdc_tx_empty();
/**
	\brief Print a string out to the USB virtual serial port
	
	Send a null terminated string out the virtual serial port

	\param str the string to be sent
*/
void usb_cdc_print_str(char *str);

/**
	\brief Print a 16 bit number to the USB virtual serial port
 
	Print a 16 bit unsigned number in decimal to the USB virtual serial port

	\param i the 16 bit number to be printed
*/
void usb_cdc_print_int(uns16 i);

/**
	\brief Transmit any buffered characters over ther USB virtual serial port
 
	Generally only used internally, this routine will attempt to place any
	characters in the transmit queue into the USB buffers

	\param i the 16 bit number to be printed
*/
void usb_cdc_handle_tx();

void usb_cdc_setup();

#endif