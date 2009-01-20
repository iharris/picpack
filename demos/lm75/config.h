#ifndef __config_h
#define __config_h

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 16
#define SERIAL_RX_BUFFER_SIZE 4
//#define SERIAL_DEBUG_ON

// - - - - - - - - - - - - - - - - - - - - 
// pic i2c defines
// - - - - - - - - - - - - - - - - - - - - 

#define i2c_scl_port PORTC
#define i2c_sda_port PORTC
#define i2c_scl_pin  3
#define i2c_sda_pin  4

// - - - - - - - - - - - - - - -
// General platform definitions
// - - - - - - - - - - - - - - -
#define PLATFORM_TYPE SURE_PICDEM_2
#define PLATFORM_CLOCK 12000000


#endif