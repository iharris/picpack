/*! \file pic_timer.h
    \brief Pic timer routines

	It's the way cool interrupt driven timer library

	Ian Harris 2007
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if you use it send me an email" license. Oh, and
	if it breaks, you get to keep both pieces.
    
   
*/

#include "pic_timer.h"

#ifdef _PIC18
	uns16 timer_0_start_value = 0;
#endif
#ifdef _PIC16
	uns8 timer_0_start_value = 0;
#endif;	

#ifdef _PIC18
void timer_setup_0(bit mode_8_bit, uns8 prescaler_setting, uns16 timer_start_value) {

	clear_bit(t0con, TMR0ON);	// turn off timer if it was on so we can get it set up
	if (mode_8_bit) {
		set_bit(t0con, T08BIT); 	// enable 8 bit mode 
	} else {
		clear_bit(t0con, T08BIT);
	}	
	clear_bit(t0con, T0CS);	// Internal instruction cycle clock
	// T0SE -- don't care
	if (prescaler_setting == TIMER_PRESCALER_OFF) {
		set_bit(t0con, PSA);
	} else {
		clear_bit(t0con, PSA);
		t0con &= 0b11111000;
		t0con |= prescaler_setting;
	}
	timer_0_start_value = timer_start_value;
	set_bit(intcon, TMR0IE); 	// Turn on timer 0 interrupts
}		
#endif

#ifdef _PIC16

void timer_setup_0(bit mode_8_bit, uns8 prescaler_setting, uns16 timer_start_value) {

	clear_bit( option_reg, T0CS ); // configure timer0 as a timer
	// T0SE -- don't care
	if (prescaler_setting == TIMER_PRESCALER_OFF) {
		set_bit(option_reg, PSA);	// Asign it to watchdog timer (ie, not us)
	} else {
		clear_bit(option_reg, PSA);	// prescaler assigned to us
		option_reg &= 0b11111000;
		option_reg |= prescaler_setting;
	}
	timer_0_start_value = timer_start_value;
}		
#endif

#ifdef _PIC18
void timer_start_0() {
	tmr0h = timer_0_start_value >> 8;
	tmr0l = timer_0_start_value & 0xff;
	set_bit(t0con, TMR0ON);
}
#endif

#ifdef _PIC16
void timer_start_0() {
	tmr0 = timer_0_start_value;
	// There's no "on" for timer 0
	set_bit(intcon, TMR0IE); 	// Turn on timer 0 interrupts
}
#endif

#ifdef _PIC18
void timer_stop_0() {
	clear_bit(t0con, TMR0ON);
}
#endif
#ifdef _PIC16
void timer_stop_0() {
	clear_bit(intcon, TMR0IE); 	// turn off timer 0 interrupts
}
#endif

