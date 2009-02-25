#ifndef __config_h
#define __config_h

#include "platform.h"

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 16
#define SERIAL_RX_BUFFER_SIZE 4
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
#define pcd9544_sclk_pin   3

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