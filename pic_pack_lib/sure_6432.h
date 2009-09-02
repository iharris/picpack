#ifndef __sure_6432_h
#define __sure_6432_h

#include "config.h"

/**
	\file sure_6432
	\brief Sure 2416 led matrix display routines
	
	Routines to communicate with Sure 6432 led matrix display
*/

/*

Notes

 en is active low for 3mm board and active high for 5mm board (go figure)
 clocking data in, samples at clock high
 l (referred to in documenation as ST appears to latch on high pulse

Board display frequency is 70 frames per second
Scanning time of one row should be no less than 0.78125ms
The latch port will carry out a low to high level conversion (1 to 0) to 
output data for display after sending 64 bits
*/

#include "pic_utils.h"


/* Include this in your config.h

// - - - - - - - - - - - - - - - - - - - - 
// sure 6432 defines
// - - - - - - - - - - - - - - - - - - - - 


#define sure_6432_en_port PORTA
#define sure_6432_en_pin  0

#define sure_6432_r1_port PORTA
#define sure_6432_r1_pin  5

#define sure_6432_r2_port PORTE
#define sure_6432_r2_pin  2

#define sure_6432_g1_port PORTA
#define sure_6432_g1_pin  5

#define sure_6432_g2_port PORTE
#define sure_6432_g2_pin  2


#define sure_6432_a_port PORTE
#define sure_6432_a_pin  1

#define sure_6432_b_port PORTE
#define sure_6432_b_pin  1

#define sure_6432_c_port PORTE
#define sure_6432_c_pin  1

#define sure_6432_d_port PORTE
#define sure_6432_d_pin  1

#define sure_6432_s_port PORTE
#define sure_6432_s_pin  1

#define sure_6432_l_port PORTE
#define sure_6432_l_pin  1

*/


#define sure_6432_setup() sure_6432_setup_io()
void sure_6432_setup_io();
void sure_6432_init();

// - - - - - - - - - - - - - - - - - - - - 
#ifdef fix_this_later

#ifndef i2c_scl_port
	#error "You haven't defined i2c_scl_port in your config.h!"
#endif

#ifndef i2c_sda_port
	#error "You haven't defined i2c_sda_port in your config.h!"
#endif

#ifndef i2c_scl_pin
	#error "You haven't defined i2c_scl_pin in your config.h!"
#endif

#ifndef i2c_sda_pin
	#error "You haven't defined i2c_sda_pin in your config.h!"
#endif
#endif


#endif