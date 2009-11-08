#ifndef __draw_screen_buffer_h
#define __draw_screen_buffer_h

#include "pic_utils.h"
#include "config.h"
#include "draw\draw.h"


uns8 get_draw_buffer(uns16 address);
void set_draw_buffer(uns16 address, uns8 data);

// We have DRAW_PIXELS_WIDE by DRAW_PIXELS_HIGH / DRAW_PIXELS_PER_BYTE

#define DRAW_TOTAL_BUFFER_SIZE (DRAW_PIXELS_WIDE * DRAW_PIXELS_HIGH / DRAW_PIXELS_PER_BYTE)

//#define DRAW_TOTAL_BUFFER_SIZE 512

#if DRAW_TOTAL_BUFFER_SIZE <= 256
	extern uns8 draw_buffer0[DRAW_TOTAL_BUFFER_SIZE];
	#define DRAW_BUFFERS 1
	#warning "1 draw buffer"
#else
	extern uns8 draw_buffer0[256];

	#if DRAW_TOTAL_BUFFER_SIZE <= 512
		extern uns8 draw_buffer1[DRAW_TOTAL_BUFFER_SIZE - 256];
		#define DRAW_BUFFERS 2
		#warning "2 draw buffers"
	#else
		extern uns8 draw_buffer1[256];

		#if DRAW_TOTAL_BUFFER_SIZE <= 768
			#warning "3 draw buffers"
			extern uns8 draw_buffer2[DRAW_TOTAL_BUFFER_SIZE - 512];
			#define DRAW_BUFFERS 3
		#else

			extern uns8 draw_buffer2[256]; 
			#if DRAW_TOTAL_BUFFER_SIZE <= 1024
				#warning "4 draw buffers"
				extern uns8 draw_buffer3[DRAW_TOTAL_BUFFER_SIZE - 768];
				#define DRAW_BUFFERS 4
			#else
				#error "Display too big for Draw library.  Must be =< 1024 bytes buffer at present"
			#endif
		#endif
	#endif
#endif			

#endif