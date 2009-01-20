#ifndef __config_h
#define __config_h

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 16
#define SERIAL_RX_BUFFER_SIZE 4
//#define SERIAL_DEBUG_ON
//#define SERIAL_IDE_DEBUG

// - - - - - - - - - - - - - - - - - - - - 
// pic i2c defines
// - - - - - - - - - - - - - - - - - - - - 

#define i2c_scl_port PORTA
#define i2c_sda_port PORTA
#define i2c_scl_pin  2
#define i2c_sda_pin  1

// - - - - - - - - - - - - - - -
// General platform definitions
// - - - - - - - - - - - - - - -
#define PLATFORM_TYPE OLIMEX_BOARD
#define PLATFORM_CLOCK 20000000


#endif