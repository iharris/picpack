//--------------------------------------------------------
// Pic Pack library
// 
// pic_utils.h
//
// Common general utilities for using pics
//
// Ian Harris 2008
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if it breaks, you get to keep both pieces" license.
//--------------------------------------------------------

#ifndef __pic_utils_h
#define __pic_utils_h

#include <system.h>
#include "config.h"

// Automatically declare number of ports depending on the chip

#ifdef PORTB
	#define NUMBER_PORTS 2
#endif
#ifdef PORTC
	#undef NUMBER_PORTS
	#define NUMBER_PORTS 3
#endif
#ifdef PORTD
	#undef NUMBER_PORTS
	#define NUMBER_PORTS 4
#endif
#ifdef PORTE
	#undef NUMBER_PORTS
	#define NUMBER_PORTS 5
#endif



// Commonly used data types, to make everything more portable
	
#define int8  char
#define uns8  unsigned char
#define uns16 unsigned int
#define int16 int
#define uns32 unsigned long
#define int32 long


// Shadow storage so we don't have to worry about read then write problems
// on PIC 16 architecture chips

extern   uns8 port_shadow[NUMBER_PORTS];
volatile uns8 port_array[NUMBER_PORTS] @PORTA;
volatile uns8 tris_array[NUMBER_PORTS] @TRISA;



// A function to turn off those pesky analog inputs
// since there's no common way amoung chips
// Add yours here - and send it in for inclusion!
#ifdef _PIC16F88
	#define turn_analog_inputs_off() ansel = 0
#endif	 
#ifdef _PIC16F876A
	#define turn_analog_inputs_off() adcon1 = 6
#endif
// Note - 16F877a unverified
#ifdef _PIC16F877A
	#define turn_analog_inputs_off() adcon1 = 6
#endif
#ifdef _PIC18F4520
	#define turn_analog_inputs_off() adcon1 = 15
#endif
#ifdef _PIC18F452
	#define turn_analog_inputs_off() adcon1 = 6
#endif
#ifdef _PIC18F2620
	#define turn_analog_inputs_off() adcon1 = 15
#endif
#ifdef _PIC18F4550
	#define turn_analog_inputs_off() adcon1 = 15
#endif

#ifndef turn_analog_inputs_off()
	#define turn_analog_inputs_off() \
		#warning "!!! - See pic_utils.h for turning off analog inputs on your pic"
#endif

// Powerful routines to ensure no read before write problems!
// Use the capital letter version of the port, eg, PORTA
// with these routines, and the pin number
// eg

// set_pin(PORTA, 2);
// will set porta's pin 2 (high)

// clear_pin(PORTA, 2);
// will set porta's pin 2 (low)

// toggle_pin(PORTA, 2);
// will invert porta's pin 2 (low->high or high->low)

// test_pin(PORTA, 2)
// will return true or false based on the pin input

// change_pin(PORTA, 2, 0)
// will change porta's pin 2 to low
 
// change_pin(PORTA, 2, 1)
// will change porta's pin 2 to high

// uns8 my_var = 1
// change_pin(PORTA, 2, my_var)
// will change porta's pin 2 to, in this case, high

// NOTE: These routines work really well, as in, they
// condense into very little code if used with constants.
// If you want to use variables in the place of port and pin numbers
// then use the set_pin_var variants.

#define set_pin(port, pin) \
 set_bit(port_shadow[port - PORTA], pin); \
 port_array[port - PORTA] = port_shadow[port - PORTA]; 

#define clear_pin(port, pin) \
 clear_bit(port_shadow[port - PORTA], pin); \
 port_array[port - PORTA] = port_shadow[port - PORTA]; 

#define toggle_pin(port, pin) \
    port_shadow[port - PORTA] ^= (1 << (pin)); \
    port_array[port - PORTA] = port_shadow[port - PORTA]; 
     	

#define test_pin(port, pin) \
	 ((port_array[port - PORTA] & (1 << pin)) != 0)

#define change_pin(port, pin, value) \
	if (value) { \
		set_pin(port, pin); \
	} else { \
		clear_pin(port, pin); \
	}	

void set_pin_var(uns8 port, uns8 pin);
void clear_pin_var(uns8 port, uns8 pin);
void toggle_pin_var(uns8 port, uns8 pin);
#define test_pin_var(port, pin) test_pin(port, pin)
void change_pin_var(uns8 port, uns8 pin, bit value);

// Routines to use the same port and pin assignments as above
// to make pins inputs or outputs
// eg
// make_output(PORTA, 2);
// will make porta's pin 2 into an output

#define make_output(port, pin) clear_bit(tris_array[port - PORTA], pin)
#define make_input(port, pin)  set_bit(tris_array[port - PORTA], pin)
 
#define turn_peripheral_ints_on() set_bit(intcon, PEIE)
#define turn_peripheral_ints_off() clear_bit(intcon, PEIE)

#define turn_global_ints_on() set_bit(intcon, GIE)
#define turn_global_ints_off() clear_bit(intcon, GIE)

// If you want to make sure the interrupts are dead, this is the best way

#define kill_interrupts()    do { \
   	 bit store_gie; \
   	 intcon.GIE = 0; \
   } while (intcon.GIE != 0)

// Start of critical section - turn off all interrupts
#define start_crit_sec() \
	bit store_gie = intcon.GIE; \
    kill_interrupts()

// End of critical section, restore interrupts if they were on before    
#define end_crit_sec() \
	intcon.GIE = store_gie

#if PLATFORM_CLOCK == 4000000
	#pragma CLOCK_FREQ 4000000
	#define GOT_CLOCK
#endif
#if PLATFORM_CLOCK == 16000000
	#pragma CLOCK_FREQ 16000000
	#define GOT_CLOCK
#endif
#if PLATFORM_CLOCK == 12000000
	#pragma CLOCK_FREQ 12000000
	#define GOT_CLOCK
#endif
#if PLATFORM_CLOCK == 20000000
	#pragma CLOCK_FREQ 20000000
	#define GOT_CLOCK
#endif
#if PLATFORM_CLOCK == 8000000
	#pragma CLOCK_FREQ 8000000
	#define GOT_CLOCK
#endif
#if PLATFORM_CLOCK == 4000000
	#pragma CLOCK_FREQ 4000000
	#define GOT_CLOCK
#endif

#if PLATFORM_CLOCK == 48000000
	#pragma CLOCK_FREQ 48000000
	#define GOT_CLOCK
#endif

#ifndef GOT_CLOCK
	#warning "Don't know about your clock. See pic_utils.h"
#endif	

#ifdef _PIC16
#define boostbloader() \
 		kill_interrupts(); \
		delay_ms(250); \
		delay_ms(250); \
		delay_ms(250); \
		delay_ms(250); \
		asm bcf _pclath,3 \
		asm bcf _pclath,4 \
		asm data 0x2000
#endif


#ifdef _PIC18
#define boostbloader() \
 		kill_interrupts(); \
		delay_ms(250); \
		delay_ms(250); \
		delay_ms(250); \
		delay_ms(250); \
		asm reset
#endif
		
#define MAGIC_BOOSTBLOADER_REQUEST 4		


#endif