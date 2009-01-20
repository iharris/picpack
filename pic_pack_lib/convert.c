
#include "convert.h"

/*
0 =  0000  00 0 00
1 =  0625  06 1 00 
2 =  1250  13 1 00
3 =  1875  19 2 25
4 =  2500  25 3 25
5 =  3125  31 3 25
6 =  3750  38 4 50
7 =  4375  44 4 50
8 =  5000  50 5 50
9 =  5625  56 6 50
10 = 6250  63 6 50
11 = 6875  69 7 75
12 = 7500  75 8 75
13 = 8125  81 8 75
14 = 8750  88 9 up
15 = 9375  94 9 up
*/

uns8 convert_to_dec2b(uns8 data) {
	
	switch (data) {
		case 0: return 00;
		case 1: return 00;
		case 2: return 00;
		case 3: return 25;
		case 4: return 25;
		case 5: return 25;
		case 6: return 50;
		case 7: return 50;
		case 8: return 50;
		case 9: return 50;
		case 10: return 50;
		case 11: return 75;
		case 12: return 75;
		case 13: return 75;
		case 14: return 99;
		case 15: return 99;
	}	
}

uns8 convert_to_dec1(uns8 data) {
	
	switch (data) {
		case 0: return 0;
		case 1: return 1;
		case 2: return 1;
		case 3: return 2;
		case 4: return 3;
		case 5: return 3;
		case 6: return 4;
		case 7: return 4;
		case 8: return 5;
		case 9: return 6;
		case 10: return 6;
		case 11: return 7;
		case 12: return 8;
		case 13: return 8;
		case 14: return 9;
		case 15: return 9;
	}	
}
uns8 convert_to_dec2(uns8 data) {
	
	switch (data) {
		case 0: return 00;
		case 1: return 06;
		case 2: return 13;
		case 3: return 19;
		case 4: return 25;
		case 5: return 31;
		case 6: return 38;
		case 7: return 44;
		case 8: return 50;
		case 9: return 56;
		case 10: return 63;
		case 11: return 69;
		case 12: return 75;
		case 13: return 81;
		case 14: return 88;
		case 15: return 94;
	}	
}


char* temp_to_str(uns8 int_part, uns8 fract_part, char* buffer) {

    buffer[5] = 0;
	if (int_part>9) {
		buffer[0] = '0' + int_part / 10;
		buffer[1] = '0' + int_part % 10;
	} else {
		buffer[0] = ' ';
		buffer[1] = '0' + int_part;
	}
	buffer[2] = '.';
	if (fract_part>9) {
		buffer[3] = '0' + fract_part / 10;
		buffer[4] = '0' + fract_part % 10;
	} else {
		buffer[3] = '0';
		buffer[4] = '0' + fract_part;
	}
	return buffer;
}
