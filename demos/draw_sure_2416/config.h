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


#define sure_2416_cs1_port PORTA
#define sure_2416_cs1_pin  0

#define sure_2416_data_port PORTA
#define sure_2416_data_pin  5

#define sure_2416_wr_port PORTE
#define sure_2416_wr_pin  2

#define sure_2416_rd_port PORTE
#define sure_2416_rd_pin  1


// - - - - - - - - - - - - - - - - - - - - 
// Draw defines
// - - - - - - - - - - - - - - - - - - - - 

// y
#define DRAW_PIXELS_HIGH 16
// x
#define DRAW_PIXELS_WIDE 24

#define DRAW_BITS_PER_PIXEL 1

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