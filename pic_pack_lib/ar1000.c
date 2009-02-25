#include "ar1000.h"
#include "pic_serial.h"

uns8 ar1000_seek_threshold;

uns16 regs[28] = {
	0xffff,0x5b15,0xF4B9,0x8012,0x0400,0x28aa,0x4400,0x1ee7,
	0x7141,0x007d,0x82ce,0x4f55,0x970c,0xb845,0xfc2d,0x8097,
	0x04a1,0xdf6a,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000};

void ar1000_setup_io() {
	i2c_setup();
}

void ar1000_set_register(uns8 reg, uns8 data)
{
	regs[reg] = data;
}

uns8 ar1000_get_register(uns8 reg)
{
	return regs[reg];
}

void ar1000_write_registers() {
uns8 count;

}


void ar1000_read_registers() {
}	


uns16 ar1000_read_register(uns8 reg)
{

	return i2c_read_eeprom_16bit(AR1000_DEV_ADDR, reg);

}

void ar1000_write_register(uns8 reg, uns16 data) {

	i2c_write_eeprom_16bit(AR1000_DEV_ADDR, reg, data);

}

void ar1000_seek(uns16 frequency, bit seek_up) {

uns16 r1, r2, r3;

r1 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 1);
r2 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 2);
r3 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 3);

// Set hmute bit
set_bit(r1, R1_HARD_MUTE_ENABLE);

i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 1, r1);

// clear tune bit
clear_bit(r2, R2_TUNE_ENABLE);

// set chan bits r2 to 87.5Mhz
// Means setting the bits to 185
r2 = r2 & 0b1111111000000000;	// Mask out tune bits
r2 = r2 + (690 - frequency);	// eg 875 - 690 = 185 for 87.5Mhz

i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 2, r2);

// clear seek bit
clear_bit(r3, R3_SEEK_ENABLE);
i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 3, r3);

// Set SEEK UP/DOWN / SPACE / BAND / SEEKTH
if (seek_up) {
	set_bit(r3, R3_SEEK_UP);
} else {
	clear_bit(r3, R3_SEEK_UP);
}
set_bit(r3, R3_SEEK_CHANNEL_SPACING);	// 100k spacing
clear_bit(r3, R3_BAND_1);	// US / EUROPE

r3 = r3 & 0b1111111110000000 + ar1000_seek_threshold;
i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 3, r3);

// Enable SEEK Bit
set_bit(r3, R3_SEEK_ENABLE);
i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 3, r3);

// Wait STC flag (Seek/Tune Complete, in “Status” register)
// Not done yet! not sure which register it is...

// Clear hmute Bit
clear_bit(r1, R1_HARD_MUTE_ENABLE);
i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 1, r1);
//	register_values[02] = 0xB480;  //set tune to 900kHz
//	register_values[03] = 0xA001;  //turn off seek, seek up, set threshold to 1
		
//	ar1000calibration(register_values);
	
//	register_values[03] = 0xE001; //turns on seek
	
//	ar1000calibration(register_values);


}

void  ar1000_tune(uns16 frequency) {

uns16 r1, r2, r3;

r1 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 1);
r2 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 2);
r3 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 3);

// Set hmute bit
set_bit(r1, R1_HARD_MUTE_ENABLE);

i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 1, r1);


// clear tune bit
clear_bit(r2, R2_TUNE_ENABLE);


i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 2, r2);

// clear seek bit
clear_bit(r3, R3_SEEK_ENABLE);
i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 3, r3);

// Set  SPACE / BAND / CHAN
set_bit(r3, R3_SEEK_CHANNEL_SPACING);	// 100k spacing
clear_bit(r3, R3_BAND_1);	// US / EUROPE
i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 3, r3);

// set chan bits r2 to 87.5Mhz
// Means setting the bits to 185
r2 = r2 & 0b1111111000000000;	// Mask out tune bits
r2 = r2 + (690 - frequency);	// eg 875 - 690 = 185 for 87.5Mhz
i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 2, r2);

// Enable TUNE Bit
set_bit(r2, R2_TUNE_ENABLE);
i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 2, r2);

// Wait STC flag (Seek/Tune Complete, in “Status” register)
// Not done yet! not sure which register it is...

// Clear hmute Bit
clear_bit(r1, R1_HARD_MUTE_ENABLE);
i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 1, r1);
//	register_values[02] = 0xB480;  //set tune to 900kHz
//	register_values[03] = 0xA001;  //turn off seek, seek up, set threshold to 1
		
//	ar1000calibration(register_values);
	
//	register_values[03] = 0xE001; //turns on seek
	
//	ar1000calibration(register_values);



	
}	


void ar1000_set_seek_threshold(uns8 new_seek_threshold)
{
	ar1000_seek_threshold = new_seek_threshold;
}	

void ar1000_init() {
	ar1000_seek_threshold = 0;
	for (uns8 count = 1; count < 18; count ++) {
		i2c_write_eeprom_16bit(AR1000_DEV_ADDR, count, regs[count]);
	}
	i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 0, regs[0]);
				
}


void ar1000_seek2() {
    i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 2, 0xb480);
    i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 3, 0xa001);
    i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 3, 0xe001);
    
	//	register_values[02] = 0xB480;  //set tune to 900kHz
//	register_values[03] = 0xA001;  //turn off seek, seek up, set threshold to 1
		
//	ar1000calibration(register_values);
	
//	register_values[03] = 0xE001; //turns on seek
	
//	ar1000calibration(register_values);
}

void ar1000_test() {
	serial_print_str("Read 01 = ");
	uns16 r1 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 1);
	serial_print_int_hex_16bit(r1);
	serial_print_nl();
	serial_print_str("Write  ");
	i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 1, 0xabcd);
	serial_print_str("Read 01 = ");
	r1 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 1);
	serial_print_int_hex_16bit(r1);
	serial_print_nl();

}