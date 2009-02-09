#ifndef __sure_2416_h
#define __sure_2416_h

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

#define SURE_2416_CMD_SYS_DISABLE		0b00000000
#define SURE_2416_CMD_SYS_ENABLE		0b00000001

#define SURE_2416_CMD_NMOS_8_COMMON		0b00100000
#define SURE_2416_CMD_NMOS_16_COMMON	0b00100100
#define SURE_2416_CMD_PMOS_8_COMMON		0b00101000
#define SURE_2416_CMD_PMOS_16_COMMON	0b00101100

#define SURE_2416_CMD_CLK_MASTER_MODE	0b00010100
#define SURE_2416_CMD_CLK_SLAVE_MODE    0b00010000

#define SURE_2416_CMD_CLK_SOURCE_INT_RC	0b00011000
#define SURE_2416_CMD_CLK_SOURCE_EXT	0b00011100

#define SURE_2416_CMD_LEDS_OFF			0b00000010
#define SURE_2416_CMD_LEDS_ON			0b00000011

#define SURE_2416_CMD_BLINK_OFF			0b00001000
#define SURE_2416_CMD_BLINK_ON			0b00001001			


void sure_2416_setup();
void sure_2416_init();
void sure_2416_write(uns8 mem_addr, uns8 data);
void sure_2416_send_command(uns8 command);

void sure_2416_set_brightness(uns8 brightness);

void sure_2416_set_pixel(uns8 x, uns8 y, uns8 colour);
uns8 sure_2416_get_pixel(uns8 x, uns8 y);
void sure_2416_horizontal_line(uns8 x, uns8 y, uns8 length, uns8 colour);	// to east direction
void sure_2416_vertical_line(uns8 x, uns8 y, uns8 length, uns8 colour);	// to the north direction
void sure_2416_clear_screen();

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