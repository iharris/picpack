#include "ds1307.h"


uns8 bcd_to_dec(uns8 bcd) {
	return (bcd & 0b00001111) + ((bcd >> 4) * 10);
}

uns8 dec_to_bcd(uns8 dec) {
	return ((dec / 10) << 4) + (dec % 10);
}

uns8 rtc_get_minutes()  {
    return bcd_to_dec(i2c_read_eeprom(ds1307_device, ds1307_minutes_register));
}
uns8 rtc_get_hours() {

// Always assume it's in 24 hour mode

	return bcd_to_dec(0b00111111 & i2c_read_eeprom(ds1307_device, ds1307_hours_register));
}    	
	
uns8 rtc_get_seconds() {
    return bcd_to_dec(0b01111111 & i2c_read_eeprom(ds1307_device, ds1307_seconds_register));
}

uns8 rtc_get_day() {
    return bcd_to_dec(i2c_read_eeprom(ds1307_device, ds1307_day_register));
}

uns8 rtc_get_date() {
    return bcd_to_dec(i2c_read_eeprom(ds1307_device, ds1307_date_register));
}
	
uns8 rtc_get_month() {
    return bcd_to_dec(i2c_read_eeprom(ds1307_device, ds1307_month_register));
}
uns8 rtc_get_year() {
    return bcd_to_dec(i2c_read_eeprom(ds1307_device, ds1307_year_register));
}

uns8 rtc_get_config()  {
    return i2c_read_eeprom(ds1307_device, ds1307_control_register);
}

uns8 rtc_set_config(uns8 config)  {
    i2c_write_eeprom(ds1307_device, ds1307_control_register, config);
}


void rtc_stop_clock() {
	i2c_write_eeprom(ds1307_device, ds1307_seconds_register, 0b10000000 | i2c_read_eeprom(ds1307_device, ds1307_seconds_register));
}

void rtc_start_clock() {
	i2c_write_eeprom(ds1307_device, ds1307_seconds_register, 0b01111111 & i2c_read_eeprom(ds1307_device, ds1307_seconds_register));
}

void rtc_set_year(uns16 year) {
	i2c_write_eeprom(ds1307_device, ds1307_year_register, dec_to_bcd(year));
}
void rtc_set_minutes(uns16 minutes) {
	i2c_write_eeprom(ds1307_device, ds1307_minutes_register, dec_to_bcd(minutes));
}
void rtc_set_day(uns8 day) {
	i2c_write_eeprom(ds1307_device, ds1307_day_register, dec_to_bcd(day));
}
void rtc_set_date(uns8 date) {
	i2c_write_eeprom(ds1307_device, ds1307_date_register, dec_to_bcd(date));
}

void rtc_set_seconds(uns8 seconds) {
    i2c_write_eeprom(ds1307_device, ds1307_seconds_register, (0b10000000 & i2c_read_eeprom(ds1307_device, ds1307_seconds_register)) + dec_to_bcd(seconds));
}

void rtc_set_hours(uns8 hours) {
	// by doing this we clear the 12/24 flag, making it 24 hour mode
		i2c_write_eeprom(ds1307_device, ds1307_hours_register, dec_to_bcd(hours));
}

void rtc_set_month(uns8 month) {
	i2c_write_eeprom(ds1307_device, ds1307_month_register, dec_to_bcd(month));
}

void rtc_setup_io() {
	i2c_setup_io();
}	