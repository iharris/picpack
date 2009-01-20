/*
 
 Routines to talk to Sure electronics seven segment displays

 Ian Harris 2008
 imharris [at] gmail.com

 Released under the "do whatever you like with this
 but if you use it send me an email" license. Oh, and
 if it breaks, you get to keep both pieces.

*/

#include "sure_7seg.h"

void sure_7seg_setup() {
	spi_setup();
}

	
uns8 sure_7seg_convert(uns8 digit) {
	switch (digit) {
	case ' ': return 0;
	case '0': return 0xfc;
	case '1': return 0x60;
	case '2': return 0xda;
	case '3': return 0xf2;
	case '4': return 0x66;
	case '5': return 0xb6;
	case '6': return 0xbe;
	case '7': return 0xe0;
	case '8': return 0xfe;
	case '9': return 0xf6;
	}	
}

void sure_7seg_write_str(char *data) {

uns8 count, digit;
char converted[5];

	count = 0;
	do {
		digit = data[count];
		converted[count] = sure_7seg_convert(digit);
		count++;
	   } while (count < 4);
	spi_write_sure(converted[3]);
	spi_write_sure(converted[2]);
	spi_write_sure(converted[1]);
	spi_write_sure(converted[0]);

	clear_pin(spi_clk_port, spi_clk_pin);	// set to low
	set_pin(spi_clk_port, spi_clk_pin);	// set to high
	   
}

