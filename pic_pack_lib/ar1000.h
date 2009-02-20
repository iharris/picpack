/*! \file ar1000.h
    \brief AR1000 FM radio module routines

	A library to communicate with the AR1000 / AR1010 FM radio chip

	Ian Harris 2009
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if you use it send me an email" license. Oh, and
	if it breaks, you get to keep both pieces.
    
   
*/

#include "pic_utils.h"
#include "i2c.h"

/** Config define for low power mode */
#define LM75_SHUTDOWN	1
/** config define for normal mode */
#define LM75_NORMAL		0



/**
	\brief Setup lm75 ports and pins
	
*/
void lm75_setup(void);

/**
	\brief Set LM75 config register
	
	Sets the LM75 config register (memory location 0x01)
	
*/
void lm75_set_config(uns8 addr, uns8 config);

/**
	\brief Get LM75 config register
	
	Gets the LM75 config register (memory location 0x01)


*/
uns8 lm75_get_config(uns8 addr);


/**
	\brief Request temperature from LM75

	Returns 16bit raw temperature register from LM75

*/

uns16 lm75_get_temp(uns8 addr);
