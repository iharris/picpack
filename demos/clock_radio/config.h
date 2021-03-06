#ifndef __config_h
#define __config_h

#include "platform.h"

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 64
#define SERIAL_RX_BUFFER_SIZE 4
//#define SERIAL_DEBUG_ON

// - - - - - - - - - - - - - - - - - - - - 
// pic i2c defines
// - - - - - - - - - - - - - - - - - - - - 

#define i2c_scl_port PORTA
#define i2c_scl_pin  0
#define i2c_sda_port PORTA
#define i2c_sda_pin  6

#define rf_ce_port PORTB
#define rf_ce_pin  1

#define rf_dr1_port PORTB
#define rf_dr1_pin  0

#define rf_cs_port PORTB
#define rf_cs_pin 3

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
#define PLATFORM_CLOCK 8000000


#endif