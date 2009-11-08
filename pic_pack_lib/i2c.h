#ifndef __i2c_h
#define __i2c_h defined

#include "config.h"

/**
	\file i2c.h
	\brief I2C software routines
	
	I2C communication routines.  Although all standard functions are provided,
	you should only need to use i2c_setup, i2c_read_eeprom and i2c_write_eeprom
*/

#include "pic_utils.h"


/* Include this in your config.h

// - - - - - - - - - - - - - - - - - - - - 
// pic i2c defines
// - - - - - - - - - - - - - - - - - - - - 

#define i2c_scl_port PORTA
#define i2c_sda_port PORTA
#define i2c_scl_pin  0
#define i2c_sda_pin  1

// - - - - - - - - - - - - - - - - - - - - 

*/
/** Change SDA line to write mode */
#define  i2c_write_sda() clear_bit(tris_array[i2c_sda_port - PORTA], i2c_sda_pin);   // output
/** Change SDA line to read mode */
#define  i2c_read_sda() set_bit(tris_array[i2c_sda_port - PORTA], i2c_sda_pin);   // input
/**
	\brief Send start signal to I2C buss
	
	Signals a start condition on the I2C buss.
*/
void i2c_start(void);

/**
	\brief Send stop signal to I2C buss
	
	Signals a stop condition on the I2C buss.
*/
void i2c_stop(void);

/**
	\brief Receive byte from I2C buss
	
	Clock in a byte over I2C lines. Note that an acknowledge is
	not sent/received
*/
uns8 i2c_receive_byte();

/**
	\brief Send a byte to I2C buss
	
	Clock out a byte from the I2C buss. An acknowledge is
	"received" (although ignored).
*/
void i2c_send_byte(uns8 data);

/**
	\brief Read an 8 bit byte over I2C buss
	
	Read an 8 bit byte from the specified device at the memory address
*/	
uns8 i2c_read_eeprom(uns8 device_address, uns8 mem_address);

/** \brief Write an 8 bit byte ove I2C buss

	Write a byte to a given device address at the memory address
*/
void i2c_write_eeprom(uns8 device_address, uns8 mem_address, uns8 data);

/** \brief Write a 16 bit value over I2C buss

	Write a byte to a given device address at the memory address
*/
void i2c_write_eeprom_16bit(uns8 device_address, uns8 mem_address, uns16 data);

/** 
	\brief Read 16 bits of data over I2C buss
	
	Read a 16 bit chunk of data from the given device and memory address
*/
uns16 i2c_read_eeprom_16bit(uns8 device_address, uns8 mem_address);

/**
	\brief Setup ports and pins for I2C communication
	
	Set port and pins correctly for I2C communication 
*/
void i2c_setup_io();
#define i2c_setup() i2c_setup_io()

#ifndef i2c_scl_port
	#error "You haven't defined i2c_scl_port in your config.h!"
#endif

#ifndef i2c_sda_port
	#error "You haven't defined i2c_sda_port in your config.h!"
#endif

#ifndef i2c_scl_pin
	#error "You haven't defined i2c_scl_pin in your config.h!"
#endif

#ifndef i2c_sda_pin
	#error "You haven't defined i2c_sda_pin in your config.h!"
#endif



#endif