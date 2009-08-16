#ifndef __config_h
#define __config_h

#include "platform.h"

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 16
#define SERIAL_RX_BUFFER_SIZE 4

//#define SERIAL_DEBUG_ON

// - - - - - - - - - - - - - - - - - - - - 
// sure 2416 defines
// - - - - - - - - - - - - - - - - - - - - 

#define ht1632_displays 4

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


// - - - - - - - - - - - - - - - - - - - - 
// Draw defines
// - - - - - - - - - - - - - - - - - - - - 

// y
#define DRAW_PIXELS_HIGH 16
// x (4 x 24) = 96
#define DRAW_PIXELS_WIDE 96

#define DRAW_BITS_PER_PIXEL 1

#define DRAW_HW_Y_ORIGIN TOP_LEFT
// or BOTTOM_LEFT

#define DRAW_HW_BUFFER_ORIENTATION VERTICAL
// or HORIZONTAL


#define DRAW_DEBUG


// - - - - - - - - - - - - - - - - - - - - 
// pic lcd defines
// - - - - - - - - - - - - - - - - - - - - 

#define lcd_e_port   PORTA
#define lcd_e_pin   1

#define lcd_rs_port  PORTA
#define lcd_rs_pin  3

#define lcd_rw_port  PORTA
#define lcd_rw_pin  2

#define lcd_db7_port PORTD
#define lcd_db7_pin 7
#define lcd_db6_port PORTD
#define lcd_db6_pin 6
#define lcd_db5_port PORTD
#define lcd_db5_pin 5
#define lcd_db4_port PORTD
#define lcd_db4_pin 4


// - - - - - - - - - - - - - - -
// General platform definitions
// - - - - - - - - - - - - - - -
#define PLATFORM_TYPE SURE_PICDEM_2
#define PLATFORM_CLOCK 12000000


#endif