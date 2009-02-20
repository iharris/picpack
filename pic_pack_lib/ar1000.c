#include "ar1000.h"

void lm75_setup() {
	i2c_setup();
}

void lm75_set_config(uns8 addr, uns8 config)
{
	i2c_write_eeprom(0x90 + addr, 0x01, config);
}

uns8 lm75_get_config(uns8 addr)
{
	return i2c_read_eeprom(0x90 + addr, 0x01);
}


uns16 lm75_get_temp(uns8 addr)
{

	return i2c_read_eeprom_16bit(0x90 + addr, 0x00);

}
