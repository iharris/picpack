#include "ds1631.h"

void ds1631_setup() {
	i2c_setup();
}

void ds1631_set_config(uns8 addr, uns8 config)
{
	i2c_write_eeprom(0x90 + addr, ds1631_access_config, config);
}

uns8 ds1631_get_config(uns8 addr)
{
	return i2c_read_eeprom(0x90 + addr, ds1631_access_config);
}

void ds1631_convert_temp(uns8 addr) {
	i2c_start();
	i2c_send_byte(0x90 + addr); 
	i2c_send_byte(ds1631_start_convert);	
	i2c_stop();	
}

uns16 ds1631_get_temp(uns8 addr)
{

	return i2c_read_eeprom_16bit(0x90 + addr, ds1631_read_temp);

}
