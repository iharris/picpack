#ifndef __config_h
#define __config_h
#include "platform.h"
#include "system.h"

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 64
#define SERIAL_RX_BUFFER_SIZE 4
//#define SERIAL_DEBUG_ON

// - - - - - - - - - - - - - - -
// General platform definitions
// - - - - - - - - - - - - - - -
#define PLATFORM_TYPE TECH_TOYS_PIC18F4550
#define PLATFORM_CLOCK 16000000

// Config for 20Mhz crystal, FS USB (half 96Mhz=48Mhz) CPU clock at 16Mhz (96Mhz/6) 
// Only clock CPU at 16Mhz since this is about the maximum at 3v. :-(

#pragma DATA	_CONFIG1L, _PLLDIV_5_1L & _CPUDIV_OSC4_PLL6_1L & _USBDIV_2_1L
#pragma DATA    _CONFIG1H, _FOSC_HSPLL_HS_1H & _FCMEN_OFF_1H & _IESO_OFF_1H

#pragma DATA    _CONFIG2L, _PWRT_ON_2L & _BOR_OFF_2L  & _VREGEN_OFF_2L
#pragma DATA    _CONFIG2H, _WDT_OFF_2H & _WDTPS_128_2H
#pragma DATA    _CONFIG3H, _CCP2MX_OFF_3H & _LPT1OSC_OFF_3H & _PBADEN_OFF_3H & _MCLRE_ON_3H
#pragma DATA    _CONFIG4L, _STVREN_ON_4L & _LVP_OFF_4L & _DEBUG_OFF_4L & _XINST_OFF_4L
#pragma DATA    _CONFIG5L, _CP0_OFF_5L & _CP1_OFF_5L & _CP2_OFF_5L & _CP3_OFF_5L
#pragma DATA    _CONFIG5H, _CPB_OFF_5H & _CPD_OFF_5H
#pragma DATA    _CONFIG6L, _WRT0_OFF_6L & _WRT1_OFF_6L & _WRT2_OFF_6L & _WRT3_OFF_6L
#pragma DATA    _CONFIG6H, _WRTC_OFF_6H & _WRTB_OFF_6H & _WRTD_OFF_6H
#pragma DATA    _CONFIG7L, _EBTR0_OFF_7L & _EBTR1_OFF_7L & _EBTR2_OFF_7L & _EBTR3_OFF_7L
#pragma DATA    _CONFIG7H, _EBTRB_OFF_7H



#endif