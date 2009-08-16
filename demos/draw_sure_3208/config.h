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
// ht1632 defines
// - - - - - - - - - - - - - - - - - - - - 


#define ht1632_cs1_port PORTA
#define ht1632_cs1_pin  0

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
#define DRAW_PIXELS_HIGH 8
// x
#define DRAW_PIXELS_WIDE 32

#define DRAW_BITS_PER_PIXEL 1

#define DRAW_HW_Y_ORIGIN TOP_LEFT
// or BOTTOM_LEFT

#define DRAW_HW_BUFFER_ORIENTATION VERTICAL
// or HORIZONTAL


#define DRAW_DEBUG



// - - - - - - - - - - - - - - -
// General platform definitions
// - - - - - - - - - - - - - - -
#define PLATFORM_TYPE SURE_PICDEM_2
#define PLATFORM_CLOCK 12000000


#endif