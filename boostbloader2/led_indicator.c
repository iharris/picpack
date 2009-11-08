
#include "config.h"


void led_indicator_on() {

#if PLATFORM_TYPE == SURE_PICDEM_2
	trisb = 0b00000000;
	latb =  0b00001110;	// Turn on a led on the dev board
#endif
#if PLATFORM_TYPE == OLIMEX_PIC_MT
	clear_bit(trisb, 4);
	#ifdef _PIC16
		clear_bit(portb, 4);
	#endif
	#ifdef _PIC18	
		clear_bit(latb, 4);	// Turn on a led on the dev board
	#endif
#endif
#if PLATFORM_TYPE == OLIMEX_BOARD
	trisa = 0b00000000;
	#ifdef _PIC16
		porta.0 = 1;	// Turn on a led on the dev board
	#endif
	#ifdef _PIC18
		lata.0 = 1;
	#endif	
#endif
#if PLATFORM_TYPE == SFE_TDN_V1
	trisb = 0b00000000;
	portb.1 = 1;
#endif

#if PLATFORM_TYPE == PIC_MT_BOARD
	trisb = 0b00000000;
	latb.4 = 0;
#endif


}

void led_indicator_off() {
	#if PLATFORM_TYPE == SURE_PICDEM_2
	latb =  0b00001111;	// Turn off the led
#endif
#if PLATFORM_TYPE == OLIMEX_PIC_MT
	#ifdef _PIC16
		set_bit(portb, 4);
	#endif
	#ifdef _PIC18	
		set_bit(latb, 4);
	#endif
#endif
#if PLATFORM_TYPE == OLIMEX_BOARD
	#ifdef _PIC16
		porta.0 = 0;	// Turn off the dev board led
	#endif
	#ifdef _PIC18
		lata.0 = 0;		// Turn off the dev board led
	#endif	
#endif
#if PLATFORM_TYPE == SFE_TDN_V1
	portb.1 = 0;
#endif
}