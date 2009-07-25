#ifndef __config_h
#define __config_h
#include "platform.h"


// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 126
#define SERIAL_RX_BUFFER_SIZE 4
// enable this define if you want fine-grained control...
//#define SERIAL_DEBUG_ON
//#define SERIAL_IDE_DEBUG
// - - - - - - - - - - - - - - - - - - - - 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// lcd definitions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#define lcd_e_port   PORTA
#define lcd_e_pin   5

#define lcd_rs_port  PORTA
#define lcd_rs_pin  2

#define lcd_rw_port  PORTA
#define lcd_rw_pin  3

#define lcd_db7_port PORTC
#define lcd_db7_pin 3
#define lcd_db6_port PORTC
#define lcd_db6_pin 2
#define lcd_db5_port PORTC
#define lcd_db5_pin 1
#define lcd_db4_port PORTC
#define lcd_db4_pin 0



// Platform definitions
//
//

#define PLATFORM_CLOCK 20000000
#define PLATFORM_TYPE  OLIMEX_BOARD




#endif