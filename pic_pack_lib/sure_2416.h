#ifndef __sure_6432_h
#define __sure_6432

#include "config.h"

/**
	\file sure_2416
	\brief Sure 2416 led matrix display routines
	
	Routines to communicate with Sure 6432 led matrix display
*/

#include "pic_utils.h"


/* Include this in your config.h

// - - - - - - - - - - - - - - - - - - - - 
// sure 2416 defines
// - - - - - - - - - - - - - - - - - - - - 


#define sure_2416_cs1_port PORTA
#define sure_2416_cs1_pin  0

#define sure_2416_data_port PORTA
#define sure_2416_data_pin  5

#define sure_2416_wr_port PORTE
#define sure_2416_wr_pin  2

#define sure_2416_rd_port PORTE
#define sure_2416_rd_pin  1

*/

void sure_2416_setup();
void sure_2416_write(uns8 mem_addr, uns8 data);
void sure_2416_send_command(uns8 command);

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