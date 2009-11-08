/**
	\file ds1307.h
	\brief Routines for communicating with the ds1307 real time clock
*/


#ifndef __ds1307_h
#define __ds1307_h defined

#include "i2c.h"
#include "pic_utils.h"

/** The ds1307 device address */
#define ds1307_device  0xD0

/** ds1307 seconds register */
#define ds1307_seconds_register 0x00
/** ds1307 minutes register */
#define ds1307_minutes_register 0x01
/** ds1307 hours register */
#define ds1307_hours_register   0x02
/** ds1307 day of week register */
#define ds1307_day_register     0x03
/** ds1307 date in month register */
#define ds1307_date_register    0x04
/** ds1307 month register */
#define ds1307_month_register   0x05
/** ds1307 year register */
#define ds1307_year_register    0x06
/** ds1307 control register */
#define ds1307_control_register 0x07

/**
	\brief Get the decoded minutes register from the ds1307
	
	Returns the number of minutes past the hour from the ds1307.  The result is
	coverted to decimal from BCD and is ready to use.
	Range - 0 through 59
*/	
uns8 rtc_get_minutes();


/**
	\brief Get the decoded hours register from the ds1307
	
	Returns hour from the ds1307.  The result is
	coverted to decimal from BCD and is ready to use.  These routines
	assume the ds1307 is running in 24 hour mode.
	Range - 0 through 23


*/
uns8 rtc_get_hours();


/**
	\brief Get the decoded seconds register from the ds1307
	
	Returns seconds from the ds1307.  The result is
	coverted to decimal from BCD and is ready to use.
	Range - 0 through 59
	
*/	
uns8 rtc_get_seconds();

/**
	\brief Get the day register from the ds1307
	
	Returns the day of the week from the ds1307.  The result is
	coverted to decimal from BCD and is ready to use.
	Range - 1 through 7
*/
uns8 rtc_get_day();


/**
	\brief Get the date register from the ds1307
	
	Returns the date in month from the ds1307.  The result is
	coverted to decimal from BCD and is ready to use.
	Range 1 through 28/29/30/31 depending on month
*/
uns8 rtc_get_date();


/**
	\brief Get the month register from the ds1307
	
	Returns the month of the year from the ds1307.  The result is
	coverted to decimal from BCD and is ready to use.
	Range 1 through 12
*/	
uns8 rtc_get_month();


/**
	\brief Get the year register from the ds1307
	
	Returns the year from the ds1307.  The result is
	coverted to decimal from BCD and is ready to use.
	Range 0 through 99
*/	
uns8 rtc_get_year();

/**
	\brief Get the config register from the ds1307
	
	Returns the config register from the ds1307.
	Bit 7 - Out - Value on SQWE pin if not outputting square wave
	Bit 6 - 0
	Bit 5 - 0
	Bit 4 - SQWE - Enable square wave output
	Bit 3 - 0
	Bit 2 - 0
	Bit 1 - RS1
	Bit 0 - RS0
	
	RS1/0 determin the speed of the square wave output. Set to 0/0 for 
	1 Hz.
*/	
uns8 rtc_get_config();


/**
	\brief Set the year register from the ds1307
	
	Changes the year in the ds1307.
*/	
void rtc_set_year(uns16 year);

/**
	\brief Set the minutes register from the ds1307
	
	Changes the minutes in the ds1307.
	
	\param seconds Value to set minutes to

*/	
void rtc_set_minutes(uns16 minutes);


/**
	\brief Set the day of the week register from the ds1307
	
	Changes the day of the week in the ds1307.
	
	\param seconds Value to set day to

*/	
void rtc_set_day(uns8 day);
/**
	\brief Set the date register from the ds1307
	
	Changes the date in the ds1307.

	\param seconds Value to set date to

*/	
void rtc_set_date(uns8 date);

/**
	\brief Set the seconds register in the ds1307
	
	Changes the seconds in the ds1307.
	
	\param seconds Value to set seconds to
*/	
void rtc_set_seconds(uns8 seconds);

/**
	\brief Set the hours register in the ds1307
	
	Changes the hours in the ds1307.  Forces the ds1307 into
	24 hour mode.
*/	
void rtc_set_hours(uns8 hours);

/**
	\brief Set the month register in the ds1307
	
	Changes the month in the ds1307.
*/	
void rtc_set_month(uns8 month);

/**
	\brief Set the config register in the ds1307
	
	Sets the config register in the ds1307.

	Bit 7 - Out - Value on SQWE pin if not outputting square wave
	Bit 6 - 0
	Bit 5 - 0
	Bit 4 - SQWE - Enable square wave output
	Bit 3 - 0
	Bit 2 - 0
	Bit 1 - RS1
	Bit 0 - RS0
	
	RS1/0 determin the speed of the square wave output. Set to 0/0 for 
	1 Hz.
	
	\param config Value to set the config register to
*/	
uns8 rtc_set_config(uns8 config);

/**
	\brief Stop the clock in the ds1307
	
	Pauses time in the ds1307
*/	
void rtc_stop_clock();


/**
	\brief Starts the clock in the ds1307
	
	Resume time in the ds1307
*/	
void rtc_start_clock();


/**
	\brief Setup ports and pins for use in the ds1307
	
	Calls i2c_setup() to configure ports and pins ready for use 
*/	
void rtc_setup_io();
#define rtc_setup() rtc_setup_io()

#endif
