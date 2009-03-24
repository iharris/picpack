#include "ar1000.h"
#include "pic_serial.h"

uns8 ar1000_seek_threshold;
//1111010 010111001

uns16 regs[18] = {
	0xffff,0x5b15,0xF4B9,0x8012,0x0400,0x28aa,0x4400,0x1ee7,
	0x7141,0x007d,0x82ce,0x4f55,0x970c,0xb845,0xfc2d,0x8097,
	0x04a1,0xdf6a};

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
r2 = r2 & 0b1000000000000000;	// Mask out tune bits
//r2 = r2 + (frequency - 690);	// eg 875 - 690 = 185 for 87.5Mhz
r2 = r2 & ((frequency - 690) << 7);	// eg 875 - 690 = 185 for 87.5Mhz

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
set_bit(regs[3], R3_SEEK_CHANNEL_SPACING);
regs[2] = 0b0000000100111111;
ar1000_init();
regs[2] = 0b0000001100111111;

ar1000_init();  //a7e0
return;
r1 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 1);
r2 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 2);
serial_print_str("r2o=");
serial_print_int_hex(r2);
r3 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 3);
//1 100111111 000000
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
//serial_print_str("f=");
//serial_print_int(frequency);
//serial_print_nl();
r2 = frequency - 690;
//serial_print_str(" f-690=");
//serial_print_int(r2);
r2 = r2 << 6;
//serial_print_str("tuning to=");
//serial_print_int_hex_16bit(r2);
i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 2, r2);
// 100111100 0000000
// Enable TUNE Bit
set_bit(r2, R2_TUNE_ENABLE);
i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 2, r2);
//1 001111000 00000
//serial_print_str(" r2en=");
//serial_print_int_hex_16bit(r2);

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
//    i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 2, 0xb480);
 //   i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 3, 0xa001);
  //  i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 3, 0xe001);
  //0xF4B9
    regs[2] = 0xcd80; //0xB480;//  0xf4b9; //0b1111111000000000;
    //ar1000_init();
    regs[3] = 0xa001;
    ar1000_init();
    regs[3] = 0xe001;
    ar1000_init();
	//	register_values[02] = 0xB480;  //set tune to 900kHz
//	register_values[03] = 0xA001;  //turn off seek, seek up, set threshold to 1
		
//	ar1000calibration(register_values);
	
//	register_values[03] = 0xE001; //turns on seek
	
//	ar1000calibration(register_values);
}

void ar1000_test() {
/*	serial_print_str("Read 01 = ");
	uns16 r1 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 1);
	serial_print_int_hex_16bit(r1);
	serial_print_nl();
	serial_print_str("Write  ");
	i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 1, 0xabcd);
	serial_print_str("Read 01 = ");
	r1 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 1);
	serial_print_int_hex_16bit(r1);
	serial_print_nl();
*/
	uns16 r1 = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, 1);
	toggle_bit(r1, R1_HARD_MUTE_ENABLE);
	i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 1, r1);
	
}

void ar1000_seek_more() {
//    i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 2, 0xb480);
 //   i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 3, 0xa001);
  //  i2c_write_eeprom_16bit(AR1000_DEV_ADDR, 3, 0xe001);
  //0xF4B9
//    regs[2] = 0; //0xB480;//  0xf4b9; //0b1111111000000000;
    set_bit(regs[2], R2_TUNE_ENABLE);
    uns16 channel = i2c_read_eeprom_16bit(AR1000_DEV_ADDR, AR1000_STATUS);
    channel = channel >> 7;
    serial_print_str(" fr=");
    serial_print_int(channel+690);
    serial_print_nl();
	//channel = channel << 6;
	regs[2] = channel;
    set_bit(regs[2], R2_TUNE_ENABLE);
    //ar1000_init();
    regs[3] = 0xa005; 
    ar1000_init();
    regs[3] = 0xe005;
    ar1000_init();
}

rom uns8 vol_lookup[] = {
0x0F,
0xCF,
0xDF,
0xEF,
0xFF,
0xEE,
0xFE,
0xED,
0xFD,
0xFB,
0xFA,
0xF9,
0xF7,
0xE6,
0xF6,
0xE5,
0xF5,
0xE3,
0xF3,
0xF2,
0xF1,
0xF0};



void ar1000_set_volume(uns8 volume) {

uns16 reg, temp;
uns8  vol;
uns16 vol1, vol2;

	if (volume > 21) { return; }
	vol = vol_lookup[volume];
	vol2 = vol >> 4;
	vol1 = vol & 0x0f;

	
	regs[3] = (regs[3] & ~0x0780) | (vol1 << 7);
	//write(3, register_values[3]);

	regs[14] = (regs[14] & ~0xF000) | (vol2 << 12);
	//write(14, register_values[14]);
		
	//serial_print_str(" AR3=");
	//serial_print_int_hex_16bit(reg);
	//regs[3] = reg;
	ar1000_init();
}

// Volume Control
// there are two different fields about volume control in AR1000E
//  Volume   :  D7  ~D10 in register R3
//  Volume2 :  D12~D15 in register R14
//  22 combinations of ( volume2 + volume)  are  recommended.
//
//
/*
void set_vol (uint8_t vol)
{
	register_values[1]  |= 0x0200;
	write(1, register_values[1]);

	register_values[3] = (register_values[3] & ~0x0780) | (v1[vol] << 7);
	write(3, register_values[3]);

	register_values[14] = (register_values[14] & ~0xF000)| (v2[vol] << 12);
	write(14, register_values[14]);

	 // clear hmute bit
    register_values[1] &= ~0x0200;
    write(1,register_values[1]);


*/
//	l=0F 80 12 0012000F0780 
//  l=DF FFFF 787F 000F 0780    