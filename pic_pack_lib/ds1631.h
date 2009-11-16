/*! \file ds1631.h
    \brief DS1631 temperature sensor routines

	A library to communicate with the DS1631 sensor

	Ian Harris 2008
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if you use it send me an email" license. Oh, and
	if it breaks, you get to keep both pieces.
    
   
*/

#include "pic_utils.h"
#include "i2c.h"

#define ds1631_start_convert 0x51
#define ds1631_stop_convert  0x22
#define ds1631_read_temp 0xAA
#define ds1631_access_th 0xA1
#define ds1631_access_tl 0xA2
#define ds1631_access_config 0xAC
#define ds1631_software_por 0x54


/**
	\brief Setup ds1631 ports and pins
	
*/
#define ds1631_setup() ds1631_setup_io()
void ds1631_setup_io(void);

/**
	\brief Set ds1631 config register
	
	Sets the ds1631 config register
	
*/
void ds1631_set_config(uns8 addr, uns8 config);

/**
	\brief Get ds1631 config register
	
	Gets the ds1631 config register (memory location 0x01)


*/
uns8 ds1631_get_config(uns8 addr);


/**
	\brief Read temperature from ds1631

	Returns 16bit raw temperature register from ds1631. Note that if you are
	in one-shot mode (the default) you must have already issued a start_convert
	and waited until it is complete (to check for completion you can either
	wait long enough, or query the config register to check).

*/
uns16 ds1631_get_temp(uns8 addr);

/**
	\brief Start temperature conversion on ds1631
	
	This routine starts the temperature conversion in the ds1631. Issue
	this command before actually reading the temperature.
*/
void ds1631_convert_temp(uns8 addr);
