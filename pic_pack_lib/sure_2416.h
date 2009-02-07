#ifndef __sure_6432_h
#define __sure_6432

#include "config.h"

/**
	\file sure_6432
	\brief Sure 6432 led matrix display routines
	
	Routines to communicate with Sure 6432 led matrix display
*/

#include "pic_utils.h"


/* Include this in your config.h

// - - - - - - - - - - - - - - - - - - - - 
// sure 6432 defines
// - - - - - - - - - - - - - - - - - - - - 


#define sure_6432_g1_port PORTA
#define sure_6432_g1_pin  1
#define sure_6432_g2_port PORTA
#define sure_6432_g2_pin  1
#define sure_6432_r1_port PORTA
#define sure_6432_r1_pin  1
#define sure_6432_r2_port PORTA
#define sure_6432_r2_pin  1
#define sure_6432_l_port  PORTA
#define sure_6432_l_pin   1
#define sure_6432_s_port  PORTA
#define sure_6432_s_pin   1
#define sure_6432_a_port  PORTA
#define sure_6432_a_pin   1
#define sure_6432_b_port  PORTA
#define sure_6432_b_pin   1
#define sure_6432_c_port  PORTA
#define sure_6432_c_pin   1
#define sure_6432_d_port  PORTA
#define sure_6432_d_pin   1
#define sure_6432_en_port PORTA
#define sure_6432_en_pin  1
*/

void sure_6432_setup()

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