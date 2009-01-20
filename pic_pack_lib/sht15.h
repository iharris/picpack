#ifndef __sht15_h
#define __sht15_h defined
#include "pic_utils.h"
#include "config.h"

// Include the following in your config.h:
/*
// - - - - - - - - - - - 
// sht15 defines
// - - - - - - - - - - -

#define sht15_sck_port PORTA
#define sht15_sck_pin 1
#define sht15_sda_port PORTA
#define sht15_sda_pin 0

// - - - - - - - - - - -
*/

void sht15_boot_up(void);

void sht15_start(void);
void sht15_read(void);
uns16 sht15_read_humidity(void);
uns16 sht15_read_temperature(void);
void sht15_send_byte(uns8 sht15_command);
uns16 sht15_read_byte16(void);
uns16 sht15_fix_humidity_h(uns16 sensor_out);
int16 sht15_fix_temperature_h(uns16 sensor_out);
uns16 sht15_fix_humidity_l(uns8 sensor_out);

#ifndef sht15_sck_port
	#error "sht15_sck_port not defined in config.h"
#endif	
#ifndef sht15_sda_port
	#error "sht15_sda_port not defined in config.h"
#endif	
#ifndef sht15_sck_pin
	#error "sht15_sck_pin not defined in config.h"
#endif	
#ifndef sht15_sda_pin
	#error "sht15_sda_pin not defined in config.h"
#endif	



#endif