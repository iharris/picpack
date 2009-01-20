/** \file spi.h

	Single Pin Interface routines
	
	Covers standard SPI-like interfaces (clock + data) and
	Sure Electronics displays which are a little different
*/	

#ifndef __spi_h
#define __spi_h

#include "pic_utils.h"
#include "config.h"

// Include the following in your config.h:
/*
// - - - - - - - - - - - 
// spi defines
// - - - - - - - - - - -

#define spi_clk_port PORTA
#define spi_data_port PORTA
#define spi_clk_pin  2
#define spi_data_pin  3

// - - - - - - - - - - -
*/



/**
	\brief Send a byte of data using software spi
	
	Sends a byte of data MSB first, data only changes
	on clock low
*/
void spi_write(uns8 data);

/**
	\brief Send a byte of data using software spi
	
	Sends a byte of data LSB first, data only changes
	on clock low
*/
void spi_write_lsb(uns8 data);

/**
	\brief Setup ports and pins for SPI output
	
	Setup ports and pins for SPI output
*/
void spi_setup();

/**
	\brief SPI write for Sure devices
	
	SPI write byte for Sure devices.  Sure devices
	do things a little differently. Data goes LSB first but data
	changes on clock high.
*/
void spi_write_sure(uns8 data);

/** 
	\brief SPI test routine
	
*/
void spi_pulse_0();
/** 
	\brief SPI test routine
	
*/
void spi_pulse_1();


#ifndef spi_clk_port
	#error "spi_clk_port not defined in config.h"
#endif	
#ifndef spi_data_port
	#error "spi_data_port not defined in config.h"
#endif	
#ifndef spi_clk_pin
	#error "spi_clk_pin not defined in config.h"
#endif	
#ifndef spi_data_pin
	#error "spi_data_pin not defined in config.h"
#endif	



#endif