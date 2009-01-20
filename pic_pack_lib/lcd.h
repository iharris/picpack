/** \file lcd.h

	\brief LCD routines
	
	This module contains routines to communicate with an LCD via the 4 bit parallel mode.
	All pins are protected from read-before-write problems with picpack's port latch
	simulation.  Reads ready flag to check LCD isn't busy before sending more data.
*/


#ifndef __lcd_h
#define __lcd_h include

#include "config.h"
#include "pic_utils.h"


// lcd routines
//


// Put the following in your config.h:
/*
#define lcd_e_port   PORTB
#define lcd_rs_port  PORTC
#define lcd_rw_port  PORTC
#define lcd_db7_port PORTC
#define lcd_db6_port PORTC
#define lcd_db5_port PORTC
#define lcd_db4_port PORTC

#define lcd_e_pin   7
#define lcd_rs_pin  4
#define lcd_rw_pin  5
#define lcd_db7_pin 3
#define lcd_db6_pin 2
#define lcd_db5_pin 1
#define lcd_db4_pin 0
*/

/** Clear LCD display */
#define LCD_CLEAR_DISP    0b00000001
/** Move cursor to top left position */
#define LCD_RETURN_HOME   0b00000010
/** Move to DRAM address (cursor position) */
#define LCD_SET_DRAM_ADDR 0b10000000 // + addr
/** Move cursor to line 1 */
#define LCD_LINE1 		  0b10000000
/** Move cursor to line 2 */
#define LCD_LINE2 		  0b11000000
/** Move cursor to line 3 */
#define LCD_LINE3 		  0b10010100
/** Move cursor to line 4 */
#define LCD_LINE4 		  0b11010100

/**
	\brief Setup port and pins to talk to LCD
	
	Call this routine first, to set up tris bits correctly to talk
	to the LCD
*/
void lcd_setup();

/**
	\brief Initialise LCD ready for display
	
	Configures LCD for 4 bit operation and gets ready for displaying text
*/
void lcd_init();

/** 
	\brief Send one byte of data to the LCD
*/
void lcd_write_data(uns8 data);

/** 
	\brief Print a string to the LCD
	
	Display the string on the LCD from the current cursor position
*/
void lcd_write_data_str(char* str);

/** 
	\brief Print a 16 bit integer the the LCD
	
	Displays an unsigned 16 bit integer on the LCD
*/
void lcd_write_data_int(uns16 i);

/** 
	\brief Sends a command to the LCD
	
	Use this to send commands to the LCD, eg, changing cursor position
*/
void lcd_write_command(uns8 data);

/** 
	\brief Wait while LCD is busy
	
	Internal routine to wait while the LCD is busy and unable to accept more data
*/
void lcd_wait_busy();

/** \brief Clear LCD display */
#define lcd_clear_display() lcd_write_command(LCD_CLEAR_DISP);	
/** \brief Return cursor home */
#define lcd_return_home()   lcd_write_command(LCD_RETURN_HOME);	

// Errors in case defines aren't found

#ifndef lcd_e_port
	#error "You haven't defined lcd_e_port in your config.h!"
#endif
#ifndef lcd_rs_port
	#error "You haven't defined lcd_rs_port in your config.h!"
#endif
#ifndef lcd_rw_port
	#error "You haven't defined lcd_rw_port in your config.h!"
#endif
#ifndef lcd_db7_port
	#error "You haven't defined lcd_db7_port in your config.h!"
#endif
#ifndef lcd_db6_port
	#error "You haven't defined lcd_db6_port in your config.h!"
#endif
#ifndef lcd_db5_port
	#error "You haven't defined lcd_db5_port in your config.h!"
#endif
#ifndef lcd_db4_port
	#error "You haven't defined lcd_db4_port in your config.h!"
#endif
#ifndef lcd_e_pin
	#error "You haven't defined lcd_e_pin in your config.h!"
#endif
#ifndef lcd_rs_pin
	#error "You haven't defined lcd_rs_pin in your config.h!"
#endif
#ifndef lcd_rw_pin
	#error "You haven't defined lcd_rw_pin in your config.h!"
#endif
#ifndef lcd_db7_pin
	#error "You haven't defined lcd_db7_pin in your config.h!"
#endif
#ifndef lcd_db6_pin
	#error "You haven't defined lcd_db6_pin in your config.h!"
#endif
#ifndef lcd_db5_pin
	#error "You haven't defined lcd_db5_pin in your config.h!"
#endif
#ifndef lcd_db4_pin
	#error "You haven't defined lcd_db4_pin in your config.h!"
#endif


#endif