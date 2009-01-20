#include "hc4led.h"
#include "pic_serial.h"

void hc4led_setup() {
	spi_setup();
}
	
uns8 hc4led_convert(uns8 digit) {
	switch (digit) {
	case ' ': return 0;
	case '0': return 126;
	case '1': return 24;
	case '2': return 109;
	case '3': return 61;
	case '4': return 27;
	case '5': return 55;
	case '6': return 115;
	case '7': return 28;
	case '8': return 127;
	case '9': return 31+32;
	case '\'': return 15;
	}	
}
	
void hc4led_write_str(char *data) {

uns8 count, digit;
char converted[5];

	count = 4;
	do {
		digit = data[count-1];
		converted[count-1] = hc4led_convert(digit);
		count--;
	   } while (count > 0);
	count = 4;
	do {
		digit = converted[count-1];
		spi_write(digit);
		count--;
	   } while (count > 0);
	   
}

