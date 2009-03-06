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

// address

#define AR1000_DEV_ADDR 0b00100000

// registers

#define AR1000_R0 0
#define R0_INT_OSC_EN 15
// ENABLE - remaining bits; unclear from datasheet

#define AR1000_R1 1

/** Enable RDS signal */
#define R1_RDS_ENABLE 15
/** Enable RDS interrupt */
#define R1_RDS_INT_ENABLE 14
/** Seek Tune Complete (STC) interrupt enable */
#define R1_STC_INT_ENABLE 13
/** De-emphasis 1=75us 0=50us*/
#define R1_DEEMP_SETTING 12
/** Force mono */
#define R1_FORCE_MONO 11
/** Soft mute enable */
#define R1_SOFT_MUTE_ENABLE 10
/** Hard mute enable */
#define R1_HARD_MUTE_ENABLE 9


//1100110110000000
// b480: 1(tune) 011010010(ch) 000000
// ch=310+ 690 = 900
 #define AR1000_R2 2
/** Tune channel enable */
#define R2_TUNE_ENABLE 15
// Channel is 14-6 (from 0 - 511 if I read the datasheet right)
// Frequency (MHz) = 69 + 0.1 * CHAN. or 690 + CHAN
//5 432109876 543210
#define AR1000_R3 3

/** Seek direction 1=up 0=down */
#define R3_SEEK_UP 15
/** Seek enable */
#define R3_SEEK_ENABLE 14
/** Seek channel spacing 1=100k 0=200k */
#define R3_SEEK_CHANNEL_SPACING 13
/** Band control 0=US/EUROPE 1=Japan */
#define R3_BAND_1 12
/** Band control 0=Japan narrow band 76Mhz - 90Mhz 1=Japan wide band 76Mhz - 108Mhz */
#define R3_BAND_0 11

//	register_values[03] = 0xA001;  //turn off seek, seek up, set threshold to 1
// 1(seekup) 0(seeken) 1(ch sp) 00(us channel) 0000(vol) 0000001(seekthresh)		
//	ar1000calibration(register_values);
// 1         0         1        00             0000      0000001	
//	register_values[03] = 0xE001; //turns on seek
// 1         1         1        00             000 0      000 0001

/** Volume setting */
#define R3_VOL_3 10
#define R3_VOL_2 9
#define R3_VOL_1 8
#define R3_VOL_0 7

/** Seek threshold */
#define R3_SEEKTH_6 6
#define R3_SEEKTH_5 5
#define R3_SEEKTH_4 4
#define R3_SEEKTH_3 3
#define R3_SEEKTH_2 2
#define R3_SEEKTH_1 1
#define R3_SEEKTH_0 0

#define AR1000_R10 10

/** Seek wrap enable */
#define R10_SEEK_WRAP_ENABLE 15

#define AR1000_R11 11

/** AFC injection control */
#define R11_AFC_INJECTION_CONTROL 15

/** High side control bits */
#define R11_AFC_HIGH_SIDE_b1 14
#define R11_AFC_HIGH_SIDE_b2 13

/* GPIO controls in this register */
#define AR1000_R13 13

/* Volume 2 in this register */
#define AR1000_R14 14

#define R14_VOL2_3 15
#define R14_VOL2_2 14
#define R14_VOL2_1 13
#define R14_VOL2_0 12

/* RDS stuff this register */
#define AR1000_R15 15

#define AR1000_RSSI 16
/* RSSI is bits 15 - 9 */
/* IF FREQ COunter is bits 8-0 */

#define AR1000_STATUS 19
/* Channel number is bits 15-6 (?) */ //543210987 6543 210
// try bits 13-4
// was 5
#define STATUS_RDS_DATA_READY 6
// was 4
#define STATUS_SEEK_TUNE_COMPLETE 5
// was 3
#define STATUS_SEEK_FAIL 4
/** Stereo flag 1=stereo 0=mono */
// was 2
#define STATUS_STEREO 3

#define AR1000_RBS 18


#define AR1000_RDS_1 19
#define AR1000_RDS_2 20
#define AR1000_RDS_3 21
#define AR1000_RDS_4 22
#define AR1000_RDS_5 23
#define AR1000_RDS_6 24

// devid is 27
#define AR1000_DEV_ID 27
// VERSION<3:0> FM radio version control (4'b0101)
// MFID<11:0> Manufacturer ID (12'h5B1)

// chip id is 28
#define AR1000_CHIP_ID 28
//CHIPNO<15:0> FM radio IC No. (16'h1000 is for RDS version, 16�h1010 is No-RDS version)







/**
	\brief Setup AR1000 ports and pins
	
	#define so as not to break existing code with new naming standard
	
*/
#define ar1000_setup() ar1000_setup_io()

/**
	\brief Setup AR1000 ports and pins
	
*/
void  ar1000_setup_io();
void  ar1000_init();
void  ar1000_set_register(uns8 reg, uns8 data);
uns8  ar1000_get_register(uns8 reg);
void  ar1000_write_registers();
void  ar1000_read_registers();
uns16 ar1000_read_register(uns8 reg);
void  ar1000_write_register(uns8 reg, uns16 data);
void  ar1000_tune(uns16 frequency);
void  ar1000_set_seek_threshold(uns8 new_seek_threshold);
void  ar1000_seek(uns16 frequency, bit seek_up);
void ar1000_seek2();
void ar1000_test(); 
void ar1000_seek_more();
void ar1000_set_volume(uns8 volume) ;