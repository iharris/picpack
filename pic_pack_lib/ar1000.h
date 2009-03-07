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

// R0 ----------------------------------------------------------

#define AR1000_R0 0
#define R0_INT_OSC_EN 15
// ENABLE - remaining bits; unclear from datasheet

// R1 ----------------------------------------------------------

#define AR1000_R1 1

// Could this be >> 9 ?

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

// R2 ----------------------------------------------------------

// Confirmed

#define AR1000_R2 2
/** Tune channel enable */
#define R2_TUNE_ENABLE 15
// CHAN is 8-0
// Frequency (MHz) = 69 + 0.1 * CHAN. or 690 + CHAN

#define R2_CHAN_8 8
#define R2_CHAN_7 7
#define R2_CHAN_6 6
#define R2_CHAN_5 5
#define R2_CHAN_4 4
#define R2_CHAN_3 3
#define R2_CHAN_2 2
#define R2_CHAN_1 1
#define R2_CHAN_0 0

// R3 ----------------------------------------------------------

// Confirmed

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

/* Volume setting */
#define R3_VOL_3 10
#define R3_VOL_2 9
#define R3_VOL_1 8
#define R3_VOL_0 7

/* Seek threshold */
#define R3_SEEKTH_6 6
#define R3_SEEKTH_5 5
#define R3_SEEKTH_4 4
#define R3_SEEKTH_3 3
#define R3_SEEKTH_2 2
#define R3_SEEKTH_1 1
#define R3_SEEKTH_0 0

// R4-R9 undocumented -------------------------------------------

// R10 ----------------------------------------------------------

#define AR1000_R10 10

/** Seek wrap enable */
#define R10_SEEK_WRAP_ENABLE 15

// R11 ----------------------------------------------------------

#define AR1000_R11 11

/** AFC injection control */
#define R11_AFC_INJECTION_CONTROL 15

/** High side control bits */
#define R11_AFC_HIGH_SIDE_b1 2
#define R11_AFC_HIGH_SIDE_b2 0

// R12 undcoumented ---------------------------------------------

// R13 ----------------------------------------------------------

/** GPIO controls in this register */

#define AR1000_R13 13

/** GPIO3- 00-Disable 01-Stereo indication 10-Output logic 0 11-Output logic 1 */
#define R13_GPIO3_1	15
#define R13_GPIO3_0	14

/** GPIO2- 00-Disable 01-STC or RDS interrupt 10-Output logic 0 11-Output logic 1 */
#define R13_GPIO2_1	13
#define R13_GPIO2_0	12

/** GPIO1- 00-Disable 01-Reserved 10-Output logic 0 11-Output logic 1 */
#define R13_GPIO2_1	13
#define R13_GPIO2_0	12


// R14 ----------------------------------------------------------

#define AR1000_R14 14

/** Second volume settings (bit 3) */
#define R14_VOL2_3 15
/** Second volume settings (bit 2) */
#define R14_VOL2_2 14
/** Second volume settings (bit 1) */
#define R14_VOL2_1 13
/** Second volume settings (bit 0) */
#define R14_VOL2_0 12

// R15 ----------------------------------------------------------

#define AR1000_R15 15

/** RDS statistic data enable signal */
#define R15_RDS_STA_EN	15

/** RDS Error correction 0x=disable erorr correction 10=2 bit error correction 11=5 bit error correction */
#define R15_RDS_MECC_1	14
#define R15_RDS_MECC_0	13

/** RDS Control mode 0=block mode (RDSR asserted after 4 blocks received, data in blocks may not be correct.
                                    RBS1-4 will present the status of each block)
                     1=group mode (RDSR is asserted after 4 blocks received without any error) */
#define R15_RDS_CTRL	12


// R16 undocumented ---------------------------------------------

// R17 undocumented ---------------------------------------------

// R18 ----------------------------------------------------------

#define AR1000_RSSI 16

/* RSSI is bits 15 - 9 */

/* IF FREQ COunter is bits 8-0 */


// R19 ----------------------------------------------------------

#define AR1000_STATUS 19

/** Current tuned channel bit 8 */
#define STATUS_CHAN_8 15
/** Current tuned channel bit 7 */
#define STATUS_CHAN_7 14
/** Current tuned channel bit 6 */
#define STATUS_CHAN_6 13
/** Current tuned channel bit 5 */
#define STATUS_CHAN_5 12
/** Current tuned channel bit 4 */
#define STATUS_CHAN_4 11
/** Current tuned channel bit 3 */
#define STATUS_CHAN_3 10
/** Current tuned channel bit 2 */
#define STATUS_CHAN_2 9
/** Current tuned channel bit 1 */
#define STATUS_CHAN_1 8
/** Current tuned channel bit 0 */
#define STATUS_CHAN_0 7

/** RDS data recevied */
#define STATUS_RDS_DATA_READY 6

/** Seek or Tune has completed */
#define STATUS_SEEK_TUNE_COMPLETE 5

/** Seek has failed  */
#define STATUS_SEEK_FAIL 4

/** Stereo flag 1=stereo 0=mono */
#define STATUS_STEREO 3

/** Used but unknown function */
#define STATUS_BIT_2 2

// R20 ----------------------------------------------------------

#define AR1000_RBS 20

// R21 ----------------------------------------------------------

#define AR1000_RDS_1 21

// R22 ----------------------------------------------------------

#define AR1000_RDS_2 22

// R23 ----------------------------------------------------------

#define AR1000_RDS_3 23

// R24 ----------------------------------------------------------

#define AR1000_RDS_4 24

// R25 ----------------------------------------------------------

#define AR1000_RDS_5 25

// R26 ----------------------------------------------------------

#define AR1000_RDS_6 26

// R27 ----------------------------------------------------------

#define AR1000_DEV_ID 27

/** Version of FM radio bit 3 */
#define DEV_ID_VERSION_3 15
/** Version of FM radio bit 2 */
#define DEV_ID_VERSION_2 14
/** Version of FM radio bit 1 */
#define DEV_ID_VERSION_1 13
/** Version of FM radio bit 0 */
#define DEV_ID_VERSION_0 12

/** MFID (12 bits 5B1) bit 11 */
#define DEV_ID_MFID_11 11
/** MFID (12 bits 5B1) bit 10 */
#define DEV_ID_MFID_10 10
/** MFID (12 bits 5B1) bit 9 */
#define DEV_ID_MFID_9   9
/** MFID (12 bits 5B1) bit 8 */
#define DEV_ID_MFID_8   8
/** MFID (12 bits 5B1) bit 7 */
#define DEV_ID_MFID_7   7
/** MFID (12 bits 5B1) bit 6 */
#define DEV_ID_MFID_6   6
/** MFID (12 bits 5B1) bit 5 */
#define DEV_ID_MFID_5   5
/** MFID (12 bits 5B1) bit 4 */
#define DEV_ID_MFID_4   4
/** MFID (12 bits 5B1) bit 3 */
#define DEV_ID_MFID_3   3
/** MFID (12 bits 5B1) bit 2 */
#define DEV_ID_MFID_2   2
/** MFID (12 bits 5B1) bit 1 */
#define DEV_ID_MFID_1   1
/** MFID (12 bits 5B1) bit 0 */
#define DEV_ID_MFID_0   0

// R28 ----------------------------------------------------------

/** Chip id - 0x1000 for RDS version, 0x1010 for non-rds version */
#define AR1000_CHIP_ID 28


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