#ifndef __config_h
#define __config_h

#include "platform.h"

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 16
#define SERIAL_RX_BUFFER_SIZE 4

//#define SERIAL_DEBUG_ON
//#define SERIAL_IDE_DEBUG
#warning "CONFIG.H BEING READ"

// - - - - - - - - - - - - - - - - - - - - 
// sure 6432 defines
// - - - - - - - - - - - - - - - - - - - - 

/*

Here we define two displays that are aligned vertically,
ie, one below the other.
-------------
|    Disp   | 
|     1     |
-------------
-------------
|    Disp   | 
|     2     |
-------------

This makes a 64x64 pixel display.

*/

#define sure_6432_displays 2
#define sure_6432_display_orientation VERTICAL



#define sure_6432_en_port PORTC
#define sure_6432_en_pin  5

#define sure_6432_r1_port PORTC
#define sure_6432_r1_pin  4

#define sure_6432_r2_port PORTC
#define sure_6432_r2_pin  3	

#define sure_6432_g1_port PORTB
#define sure_6432_g1_pin  4

#define sure_6432_g2_port PORTB
#define sure_6432_g2_pin  5


#define sure_6432_a_port PORTA
#define sure_6432_a_pin  0

#define sure_6432_b_port PORTA
#define sure_6432_b_pin  1

#define sure_6432_c_port PORTA
#define sure_6432_c_pin  2

#define sure_6432_d_port PORTA
#define sure_6432_d_pin  3

#define sure_6432_s_port PORTB
#define sure_6432_s_pin  7

#define sure_6432_l_port PORTB
#define sure_6432_l_pin  6


// - - - - - - - - - - - - - - - - - - - - 
// Draw defines
// - - - - - - - - - - - - - - - - - - - - 

// y
#define DRAW_PIXELS_HIGH 64
// (32 x 2)

// x
#define DRAW_PIXELS_WIDE 64

#define DRAW_BITS_PER_PIXEL 2

#define DRAW_HW_Y_ORIGIN TOP_LEFT

#define DRAW_HW_BUFFER_ORIENTATION HORIZONTAL

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
#define PLATFORM_TYPE OLIMEX_BOARD
#define PLATFORM_CLOCK 20000000


#endif