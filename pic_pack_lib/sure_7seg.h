/** \file sure_7seg.h
 
 Routines to talk to Sure electronics seven segment displays

 Ian Harris 2008
 imharris [at] gmail.com

 Released under the "do whatever you like with this
 but if you use it send me an email" license. Oh, and
 if it breaks, you get to keep both pieces.

*/
#ifndef __sure_7seg_h
#define __sure_7seg_h

#include "spi.h"
#include "pic_utils.h"

/**
	\brief Setup ports and pins to communicate to Sure display
	
	Set up ports and pins as appropriate to communicate
	via SPI to Sure 7 segment displays
*/
void sure_7seg_setup();

/**
	\brief Display ASCII string to 7 segment displays
	
	Converts ASCII to the appropriate magic characters
	to display on a Sure 7 segment display. Only numbers
	0-9 and space are implented for now.
*/
 /*
 
 To create your own characters, add together:
       0x40
     --------
     |      |
0x02 |      | 0x20
     | 0x01 |
     |------|
     |      |
0x04 |      | 0x10
     |      |
     --------
      0x08

*/





*/
void sure_7seg_write_str(char *data);

#endif
