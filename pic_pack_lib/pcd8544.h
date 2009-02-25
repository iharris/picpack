#ifndef __nokia_3310_h
#define __nokia_3310_h

#include "config.h"

/**
	\file pcd8544.h
	\brief PCD8544 Interface routines (used in Nokia 3310 LCD)
	
	Routines to communicate with Nokia 3310 LCD display via the PCD8544 chip
*/

#include "pic_utils.h"


/* Include this in your config.h

// - - - - - - - - - - - - - - - - - - - - 
// PCD8544 (Nokia 3310) interface defines
// - - - - - - - - - - - - - - - - - - - - 

#define pcd8544_res_port  PORTD
#define pcd8544_res_pin   3

#define pcd8544_sclk_port  PORTC
#define pcd9544_sclk_pin   3

#define pcd8544_sdin_port PORTC
#define pcd8544_sdin_pin  5

#define pcd8544_dc_port   PORTF
#define pcd8544_dc_pin    6

#define pcd8544_sce_port  PORTF
#define pcd8544_sce_pin   7
*/


#define pcd8544_setup() pcd8544_setup_io()
void pcd8544_setup_io();
void pcd8544_init();
void pcd8544_write(uns8 mem_addr, uns8 data);
void pcd8544_send_command(uns8 command);
void pcd8544_set_pixel(uns8 x, uns8 y, uns8 colour);
void pcd8544_clear();

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