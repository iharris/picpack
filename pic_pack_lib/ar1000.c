#include "ar1000.h"

uns16 regs[28] = {
	0xffff,0x5b15,0xF4B9,0x8012,0x0400,0x28aa,0x4400,0x1ee7,
	0x7141,0x007d,0x82ce,0x4f55,0x970c,0xb845,0xfc2d,0x8097,
	0x04a1,0xdf6a,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000};

void ar1000_setup() {
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

void ar1000_write_register(uns8 reg, uns16 data)
{

	i2c_write_eeprom_16bit(AR1000_DEV_ADDR, reg, data);

}

void ar1000_seek() {

uns16 r1, r2, r3;

r1 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 1);
r2 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 3);
r3 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 3);

// Set hmute bit
set_bit(r1, R1_HARD_MUTE_ENABLE);

i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 1, r1);

// clear tune bit
clear_bit(r2, R2_TUNE_ENABLE);

// set chan bits r2 to 87.5Mhz
// Means setting the bits to 185
r2 = r2 & 0b1111111000000000;	// Mask out tune bits
r2 = r2 + 185;	// 875 - 690 = 185 for 87.5Mhz

i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 2, r2);

// clear seek bit
clear_bit(r3, R3_SEEK_ENABLE);

// Set SEEK UP/DOWN / SPACE / BAND / SEEKTH



	/* seek
Set hmute Bit
Clear TUNE Bit
Set CHAN Bits
Clear SEEK Bit
Set SEEKUP/SPACE/BAND/SEEKTH Bits
Enable SEEK Bit
Wait STC flag (Seek/Tune Complete, in “Status” register)
Clear hmute Bit
	register_values[02] = 0xB480;  //set tune to 900kHz
	register_values[03] = 0xA001;  //turn off seek, seek up, set threshold to 1
		
	ar1000calibration(register_values);
	
	register_values[03] = 0xE001; //turns on seek
	
	ar1000calibration(register_values);

*/
}
