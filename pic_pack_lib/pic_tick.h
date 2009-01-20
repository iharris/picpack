/*
 
 \file pic_tick.h
 
 Timer helper routines

 Ian Harris 2008
 imharris [at] gmail.com

 Released under the "do whatever you like with this
 but it breaks, you get to keep both pieces" license.

*/
#ifndef __pic_tick_h
#define __pic_tick_h

#include "pic_utils.h"

/** Global tick counter */
static uns16 tick = 0;

/**
	\brief Return current tick count
	
	Returns the current tick count. Thread and interrupt safe.
*/
uns16 tick_get_count();
/**
	\brief Calculate the tick time difference between two values
	
	Calculates how many ticks have elapsed between two tick values. Covers
	cases where the tick count wraps beyond its 16 bit value.
*/
uns16 tick_calc_diff(uns16 a, uns16 b);

/**
	\brief Call this routine to increment tick count
	
	Typically called during the interrupt routine of a timer to increment
	the tick count.  Note this routine assumes that interupts are off - which
	is always the case in an interrupt sub routine.
*/
void handle_tick();

/**
	\brief Call this routine to increment tick count - inline version
	
	Typically called during the interrupt routine of a timer to increment
	the tick count.  Note this routine assumes that interupts are off - which
	is always the case in an interrupt sub routine. Inline version so you
	don't use up one stack level
*/
inline void handle_tick_inline() {
   tick++;	// we assume that interrupts are off at this point
}   		

#endif   