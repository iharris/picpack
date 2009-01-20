/* 

Mega flasher - flash everything on port a and port b!

*/

#include "pic_utils.h"

//#pragma DATA _CONFIG, _CP_OFF & _CCP1_RB0 & _DEBUG_OFF & _WRT_PROTECT_OFF & _CPD_OFF & _LVP_OFF & _PWRTE_OFF & _WDT_OFF & _PWRTE_ON & _BODEN_OFF &_MCLR_ON & _INTRC_IO  

#pragma CLOCK_FREQ 20000000


void interrupt () {
				  }

void flash() {
	porta = ~porta;
	portb = ~portb;
}		

void main() {

	trisa = 0;
	trisb = 0;
	
	while (1) {
		flash();

		delay_ms(250);
		delay_ms(250);
	
	}		

}