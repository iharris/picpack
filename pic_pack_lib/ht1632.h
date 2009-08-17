#ifndef __ht1632_h
#define __ht1632_h

#include "config.h"

/**
	\file ht1632.h
	\brief Holtek LED matrix display routines, used in Sure 2416 display and others
	
	Routines to communicate with Holtek HT1632 led matrix display chip
*/

#include "pic_utils.h"


/* Include this in your config.h

// - - - - - - - - - - - - - - - - - - - - 
// HT1632 defines
// - - - - - - - - - - - - - - - - - - - - 

// If you have more then 1 display daisy chained, define
// the number of displays, and a matching number of ht1632_csX_port 
// and ht1632_csX_pin defines:

// #define ht1632_displays 4

#define ht1632_cs1_port PORTB
#define ht1632_cs1_pin  0

#define ht1632_cs2_port PORTB
#define ht1632_cs2_pin  1

#define ht1632_cs3_port PORTB
#define ht1632_cs3_pin  2

#define ht1632_cs4_port PORTB
#define ht1632_cs4_pin  2

#define ht1632_data_port PORTA
#define ht1632_data_pin  5

#define ht1632_wr_port PORTE
#define ht1632_wr_pin  2

#define ht1632_rd_port PORTE
#define ht1632_rd_pin  1

*/

#define HT1632_CMD_SYS_DISABLE		0b00000000
#define HT1632_CMD_SYS_ENABLE		0b00000001

#define HT1632_CMD_NMOS_8_COMMON	0b00100000
#define HT1632_CMD_NMOS_16_COMMON	0b00100100
#define HT1632_CMD_PMOS_8_COMMON	0b00101000
#define HT1632_CMD_PMOS_16_COMMON	0b00101100

#define HT1632_CMD_CLK_MASTER_MODE	0b00010100
#define HT1632_CMD_CLK_SLAVE_MODE   0b00010000

#define HT1632_CMD_CLK_SOURCE_INT_RC	0b00011000
#define HT1632_CMD_CLK_SOURCE_EXT		0b00011100

#define HT1632_CMD_LEDS_OFF			0b00000010
#define HT1632_CMD_LEDS_ON			0b00000011

#define HT1632_CMD_BLINK_OFF		0b00001000
#define HT1632_CMD_BLINK_ON			0b00001001			


void ht1632_setup_io();
void ht1632_init(uns8 hw_config);
void ht1632_write(uns8 mem_addr, uns8 data);
void ht1632_send_command(uns8 command);

void ht1632_set_brightness(uns8 brightness);

void ht1632_set_pixel(uns8 x, uns8 y, uns8 colour);
uns8 ht1632_get_pixel(uns8 x, uns8 y);
void ht1632_horizontal_line(uns8 x, uns8 y, uns8 length, uns8 colour);	// to east direction
void ht1632_vertical_line(uns8 x, uns8 y, uns8 length, uns8 colour);	// to the north direction
void ht1632_clear();
void ht1632_fill(uns8 colour);
void ht1632_fill2(uns8 colour);

// - - - - - - - - - - - - - - - - - - - - 


#ifndef ht1632_cs1_port
	#error "You haven't defined ht1632_cs1_port in your config.h!"
#endif

#ifndef ht1632_cs1_pin
	#error "You haven't defined i2c_sda_port in your config.h!"
#endif

#ifndef ht1632_data_port
	#error "You haven't defined i2c_scl_pin in your config.h!"
#endif

#ifndef ht1632_data_pin
	#error "You haven't defined i2c_sda_pin in your config.h!"
#endif

#ifndef ht1632_wr_port
	#error "You haven't defined i2c_scl_pin in your config.h!"
#endif

#ifndef ht1632_wr_pin
	#error "You haven't defined i2c_sda_pin in your config.h!"
#endif

#ifndef ht1632_rd_port
	#error "You haven't defined i2c_scl_pin in your config.h!"
#endif

#ifndef ht1632_rd_pin
	#error "You haven't defined i2c_sda_pin in your config.h!"
#endif


#endif