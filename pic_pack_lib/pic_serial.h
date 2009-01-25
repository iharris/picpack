/*! \file pic_serial.h
    \brief Pic serial routines

	It's the way cool interrupt driven serial library

	Ian Harris 2007
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if it breaks, you get to keep both pieces" license.
    

Put the following into your config.h

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 20
#define SERIAL_RX_BUFFER_SIZE 4

// Use this define if you want fine-grained control of what happens in the serial port
//#define SERIAL_DEBUG_ON

// Use this define if you are debugging in the IDE simulator and don't want it to hang
// waiting for serial interrupts that will never come...
//#define SERIAL_IDE_DEBUG

// Use thie define if you want to drop a character if the TX buffer is full,
// rather than the default behaviour, which is to wait until the TX buffer has
// a spare spot.
//#define SERIAL_DISCARD_ON_TX_FULL_DURING_INT

// - - - - - - - - - - - - - - - - - - - - 

Put the following in your ISR

serial_handle_tx_isr();
serial_handle_rx_isr();

Put the following in your system setup routine

serial_setup(bit req_brgh, uns8 req_spbrg);


*/




#ifndef __pic_serial_h
#define __pic_serial_h

#include "pic_utils.h"
#include "config.h"

// Include these in your interrupt() routine

/** include in your ISR */
#define serial_handle_tx_isr() \
	if (pir1.TXIF) { serial_tx_isr(); }
/** include in your ISR */
#define serial_handle_rx_isr() \
	if (pir1.RCIF) { serial_rx_isr(); clear_bit( pir1, RCIF ); }


/** serial_load_tx
 
    \brief Serial transmit interrupt service routine
 
    This routine needs to be called from your interrupt() routine when the
	transmit hardware interrupt occurs in order to send bytes that are waiting
	in the fifo buffer.
 
*/
void serial_tx_isr();

/**
   \brief Serial receive interrupt service routine
   
   This routine needs to be called from your interrupt() routine when the
   receive hardware interrupt occurs in order to put received bytes into the
   fifo buffer.
*/
void serial_rx_isr(); 

/**
    \brief Configure the pic for serial communicaitons
 
	Configures the pic and gets ready for interrupt-driven serial
	communications.  Includes setting the tris bits appropriately,
	and getting the baud rate generator set up.  After calling this
	you can immediately start sending and receiving bytes.
 
	\param brgh refer to your pic documentation
	\param spbrg refer to your pic documentation

*/

void serial_setup(bit req_brgh, uns8 req_spbrg);

/**

 \brief Transmit a single character
 
 Sends a single character out the serial connection.  It is sent straight
 out if possible, otherwise put into the fifo. Note that if you fill the
 fifo while interrupts are off (eg, in an interrupt routine or a critical section)
 then this routine will hang the pic, since it's waiting for an interrupt to clear
 the fifo, which never comes... The moral is to keep your fifo big enough or
 don't send too much while interrupts are off (eg, in an interrupt
 response routine).  Of course, you *can* send things in an ISR - just don't
 fill the fifo up.
 
 \param c the character to transmit
 
*/
void serial_putc(uns8 c);

/**

	\brief Retrieve a character from the serial port
	
	Retrieve character from the serial port.  Note that if there is nothing
	in the fifo, this function will wait until a character is received - and
	this will never happen if interrupts are turned off when this is called!
	So, be careful not to call getc during a critical section or during an ISR
	*unless* you're sure there's something in the fifo. You can do this by calling
	the serial_rx_avail() routine.  In any other situation, you can call getc()
	and happily wait for a character to arrive.
	
*/ 
uns8 serial_getc(void);

/**
	\brief Print a string out to the serial port
	
	Send a null terminated string out the serial port

	\param str the string to be sent
*/
void serial_print_str(char *str);

/**
	\brief Print a rom string out to the serial port
	
	Send a null terminated rom string out the serial port

	\param str the rom string to be sent
*/

void serial_print_str(rom char *str);

/**
	\brief Print a 16 bit number to the serial port
 
	Print a 16 bit unsigned number in decimal to the serial port

	\param i the 16 bit number to be printed
*/
void serial_print_int(uns16 i);

/**
	\brief Print an 8 bit number in hex to the serial port
	
	Print a 8 bit unsigned number in hex to the serial port 

	\param i 8 bit number to be printed
*/
void serial_print_int_hex(uns8 i);

/**
	\brief Print a 16 bit number in hex to the serial port
	
	Print a 16 bit unsigned number in hex to the serial port 

	\param i 16 bit number to be printed

*/
void serial_print_int_hex_16bit(uns16 i);

/**
	\brief Print a newline
	
	Print a new line out the serial port - if you do this often, this routine
	can be used to save a couple of instructions. Always helps!
*/
void serial_print_nl();

/**
	\brief Print a space
	
	Print a space out the serial port - if you do this often, this routine
	can be used to save a couple of instructions. Always helps!
*/

void serial_print_spc();

/**
	\brief Tests if the serial tx fifo is full
	
	Tests to see if the serial transmit fifo is full.
	
	\return true (non zero) if tx fifo is full, false (zero) otherwise
*/
void serial_tx_full();

/**
	\brief Tests if the serial tx fifo is empty
	
	Tests to see if the serial transmit fifo is empty.
	
	\return true (non zero) if tx fifo is empty, false (zero) otherwise
*/
uns8 serial_tx_empty();

/**
	\brief Tests if the serial rx fifo has a character available
	
	Tests to see if the serial receive fifo has a character available.  Useful
	to call before getc() if interrupts are not enabled in that section of code.
	
	\return true (non zero) if there are one or more characters waiting
	in the fifo queue, false (zero) otherwise
*/
uns8 serial_rx_avail();

#if PLATFORM_CLOCK == 40000000
	#define SPBRG_9600 255
	#define SPBRG_19200 129
	#define SPBRG_38400 64
	#define SPBRG_57600 42
	#define SPBRG_115200 21
#endif

#if PLATFORM_CLOCK == 20000000
	#define SPBRG_9600 129
	#define SPBRG_19200 64
	#define SPBRG_38400 32
	#define SPBRG_57600 21
	#define SPBRG_115200 10
#endif

#if PLATFORM_CLOCK == 16000000
	#define SPBRG_9600 103
	#define SPBRG_19200 51
	#define SPBRG_38400 25
	#define SPBRG_57600 16
	#define SPBRG_115200 8
#endif	

#if PLATFORM_CLOCK == 12000000
	#define SPBRG_9600 77
	#define SPBRG_19200 38
	#define SPBRG_38400 19
	#define SPBRG_57600 12
	#define SPBRG_115200 6
#endif	

#if PLATFORM_CLOCK == 8000000
	#define SPBRG_9600 51
	#define SPBRG_19200 25
	#define SPBRG_38400 12
	#define SPBRG_57600 8
	#define SPBRG_115200 3
#endif

#if PLATFORM_CLOCK == 4000000
	#define SPBRG_9600 25
	#define SPBRG_19200 12
	#define SPBRG_38400 6
	#define SPBRG_57600 3
	#define SPBRG_115200 1
#endif




#ifndef SPBRG_9600
	#warning "Don't know the SPBRG value for your clock speed."
	#warning "Did you set your PLATFORM_CLOCK etc in your config.h?"
#endif

#define BRGH_HIGH_SPEED 1
#define BRGH_LOW_SPEED  0

#ifndef SERIAL_TX_BUFFER_SIZE
	#error "pic_serial: SERIAL_TX_BUFFER_SIZE not defined in config.h"
#endif	
#ifndef SERIAL_RX_BUFFER_SIZE
	#error "pic_serial: SERIAL_RX_BUFFER_SIZE not defined in config.h"
#endif	

#if SERIAL_TX_BUFFER_SIZE > 254
	#error "pic_serial: SERIAL_TX_BUFFER_SIZE can't be bigger than 254 bytes (or one ram bank)"
#endif

#if SERIAL_RX_BUFFER_SIZE > 254
	#error "pic_serial: SERIAL_RX_BUFFER_SIZE can't be bigger than 254 bytes (or one ram bank)"
#endif


#endif