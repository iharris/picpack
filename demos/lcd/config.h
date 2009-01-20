#ifndef __config_h
#define __config_h

// - - - - - - - - - - - - - - - - - - - - 
// Platform information
// - - - - - - - - - - - - - - - - - - - - 
#define PLATFORM_TYPE breadboard
#define PLATFORM_CLOCK 12000000

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 16
#define SERIAL_RX_BUFFER_SIZE 4
//#define SERIAL_DEBUG_ON

// - - - - - - - - - - - - - - -


// lcd

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



#endif