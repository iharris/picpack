#ifndef __config_h
#define __config_h

#include "platform.h"

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 20
#define SERIAL_RX_BUFFER_SIZE 4

// Use this define if you want fine-grained control of what happens in the serial port
//#define SERIAL_DEBUG_ON

// Use this define if you are debugging in the IDE simulator and don't want it to hang
// waiting for serial interrupts that will never come...
//#define SERIAL_IDE_DEBUG

// Use thie define if you want to drop a character if the TX buffer is full,
// rather than the default behaviour, which is to wait until the TX buffer has
// a spare spot.
//#define SERIAL_DISCARD_ON_TX_FULL_DURING_INT

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