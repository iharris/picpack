#ifndef __config_h
#define __config_h

#include "platform.h"

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 32
#define SERIAL_RX_BUFFER_SIZE 8
//#define SERIAL_DEBUG_ON

// Nokia 3310
// Connections
// #RES -> RD3
// SCK -> SCK1 (RC3)
// SDIN -> MOSI1 (RC5 SD01)
// D/#C -> RF6
// #SCE -> #SS1 (RF7)

// - - - - - - - - - - - - - - - - - - - - 
// PCD8544 (Nokia 3310) interface defines
// - - - - - - - - - - - - - - - - - - - - 


#define pcd8544_res_port  PORTD
#define pcd8544_res_pin   3

#define pcd8544_sclk_port  PORTC
#define pcd8544_sclk_pin   3

#define pcd8544_sdin_port PORTC
#define pcd8544_sdin_pin  5

#define pcd8544_dc_port   PORTF
#define pcd8544_dc_pin    6

#define pcd8544_sce_port  PORTF
#define pcd8544_sce_pin   7



// - - - - - - - - - - - - - - - - - - - - 
// Draw defines
// - - - - - - - - - - - - - - - - - - - - 

// y
#define DRAW_PIXELS_HIGH 48
// x
#define DRAW_PIXELS_WIDE 84

#define DRAW_BITS_PER_PIXEL 1

#define DRAW_HW_Y_ORIGIN TOP_LEFT
// or BOTTOM_LEFT

#define DRAW_HW_BUFFER_ORIENTATION Y
// or Y


#define DRAW_DEBUG


// - - - - - - - - - - - - - - -
// General platform definitions
// - - - - - - - - - - - - - - -
#define PLATFORM_TYPE OLIMEX_PIC_LCD3310
#define PLATFORM_CLOCK 48000000


#endif