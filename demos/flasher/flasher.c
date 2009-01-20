/* Flasher! 

A good little test program that flashes leds..

*/

#include "pic_utils.h"

#pragma DATA _CONFIG, _CP_OFF & _CCP1_RB0 & _DEBUG_OFF & _WRT_PROTECT_OFF & _CPD_OFF & _LVP_OFF & _PWRTE_OFF & _WDT_OFF & _PWRTE_ON & _BODEN_OFF &_MCLR_ON & _INTRC_IO  

#pragma CLOCK_FREQ 8000000

#define flash_port PORTB
#define flash_pin  0

void interrupt () {
				  }

void flash() {
	toggle_pin(flash_port, flash_pin);
}		

void main() {

	trisb.flash_pin = 0;
	
	while (1) {
		flash();

		delay_ms(250);
		delay_ms(250);
	
	}		

}