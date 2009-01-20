/*! \file pic_timer.h
    \brief Pic timer routines

	It's the way cool interrupt driven timer library

	Ian Harris 2007
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if you use it send me an email" license. Oh, and
	if it breaks, you get to keep both pieces.
    
   
*/

#include "pic_utils.h"

/** Timer mode for devices where this is applicable (16bit timer) */
#define TIMER_16BIT_MODE 0
/** Timer mode for devices where this is applicable (8bit timer) */
#define TIMER_8BIT_MODE 1

#define TIMER_PRESCALER_OFF			0xff
#define TIMER_PRESCALER_1_TO_2		0x00
#define TIMER_PRESCALER_1_TO_4		0x01
#define TIMER_PRESCALER_1_TO_8		0x02
#define TIMER_PRESCALER_1_TO_16		0x03
#define TIMER_PRESCALER_1_TO_32		0x04
#define TIMER_PRESCALER_1_TO_64		0x05
#define TIMER_PRESCALER_1_TO_128	0x06
#define TIMER_PRESCALER_1_TO_256	0x07


#ifdef _PIC18
extern uns16 timer_0_start_value;
#endif

#ifdef _PIC16
extern uns8 timer_0_start_value;
#endif

/**
	\brief Setup timer zero with starting values
	
	Turns off timer zero, congures 16/8bit mode (only for 18f devices), prescaler
	setting and start value (which will be loaded on each reset.
*/	
void timer_setup_0(bit mode_16_bit, uns8 prescaler_setting, uns16 timer_start_value);

/**
	\brief Start timer 0
	
	Kicks off timer 0.  In pic18 devices this will turn the timer on, on pic16 devices
	this will turn on timer0 interrupts.
*/	
void timer_start_0();

/**
	\brief Stop timer 0
	
	Stops timer 0.  In pic18 devices, this will switch the timer off. On pic16 devices
	this will merely turn off the interrupt and the timer will continue running.
*/	
void timer_stop_0();

/** 
	\brief Timer 0 callback function
	
	When a timer 0 interrupt occurs, after handling the interupt and timing issues, this
	callback function is executed. You will need to define this subroutine in your
	code, otherwise linking will fail.
*/

extern void timer_0_callback();

/**
	\brief handle timer 0 in interrupt service routine
	
	Call this routine in your interrupt subroutine to automatically service timer 0 
	interrupts if they have occursed.
	
*/	
#ifdef _PIC18
inline void timer_handle_0_isr() {
	uns16 start_value;	
	if (test_bit(intcon, TMR0IF)) {	// interrupt?
		start_value = + tmr0l + timer_0_start_value  + 1;	// adjust start value
		tmr0h = start_value >> 8;	// set high value
		tmr0l = start_value & 0xff;	// set low value (must be done in this order)
		clear_bit( intcon, TMR0IF ); //clear timer 0 interrupt bit
		timer_0_callback();	// call the callback
	}
}	
#endif

#ifdef _PIC16
inline void timer_handle_0_isr() {
	uns8 start_value;	
	if (test_bit(intcon, TMR0IF)) {	// interrupt?
		start_value = + tmr0 + timer_0_start_value  + 1;	// adjust start value
		tmr0 = start_value;	// reset timer value
		clear_bit( intcon, TMR0IF ); //clear timer 0 interrupt bit
		timer_0_callback();	// call the callback
	}
}	
#endif



