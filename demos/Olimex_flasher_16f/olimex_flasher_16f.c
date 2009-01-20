/* Flasher! 

A good little test program that flashes leds..

*/

#include "pic_utils.h"

// Don't include CONFIG pragma when bootloading!

//#pragma DATA _CONFIG, _CP_OFF & _CCP1_RB0 & _DEBUG_OFF & _WRT_PROTECT_OFF & _CPD_OFF & _LVP_OFF & _PWRTE_OFF & _WDT_OFF & _PWRTE_ON & _BODEN_OFF &_MCLR_ON & _INTRC_IO  

// Set your clock speed in config.h


#define flash_port PORTA
#define flash_pin  0

// Need this interrupt when bootloading, even though it's empty

void interrupt () {
}

void flash() {
	toggle_pin(flash_port, flash_pin);
}		

void main() {

	make_output(flash_port, flash_pin);
	
	while (1) {
		// Let's flash!
		flash();

		// Tip! delay_ms etc only take 8 bit values
		// so delay_ms(500) will not do what we want.
		// So to delay for half a second, we do this twice:
		delay_ms(250);
		delay_ms(250);
	
	}		

}