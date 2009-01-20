/*
 
 pic_tick.c
 ----------
 
 Timer helper routines

 Ian Harris 2008
 imharris [at] gmail.com

 Released under the "do whatever you like with this
 but if you use it send me an email" license. Oh, and
 if it breaks, you get to keep both pieces.

*/
#include "pic_tick.h"

uns16 tick_get_count() {

bit my_store_gie;
uns16 result;

	my_store_gie = intcon.GIE;	// save global interrupt       
	kill_interrupts();	// turn them off

	result = tick;	// Grab a copy
			
	intcon.GIE = my_store_gie;	// interrupts back to normal
	return result;	// return the result
}	
	
uns16 tick_calc_diff(uns16 a, uns16 b) {
	if (a <= b) {	// simple case
		return b-a;
	} else {
		return 65535 - a + b + 1;	// more complex case
	}
}

void handle_tick() {
   tick++;	// we assume that interrupts are off at this point
}   		
		
		