#include "pic_utils.h"
#include "draw\draw_screen_buffer.h"

 
#if DRAW_TOTAL_BUFFER_SIZE < 256
	uns8 draw_buffer0[DRAW_TOTAL_BUFFER_SIZE];
#else
	uns8 draw_buffer0[256];
	#if DRAW_TOTAL_BUFFER_SIZE < 512
		uns8 draw_buffer1[DRAW_TOTAL_BUFFER_SIZE - 256];
	#else
		uns8 draw_buffer1[256];
		#if DRAW_TOTAL_BUFFER_SIZE < 768
			uns8 draw_buffer2[DRAW_TOTAL_BUFFER_SIZE - 512];
		#else
			uns8 draw_buffer2[256]; 
			#if DRAW_TOTAL_BUFFER_SIZE < 1024
				uns8 draw_buffer3[DRAW_TOTAL_BUFFER_SIZE - 768];
			#else
				#error "Display too big for Draw library.  Must be < 1024 bytes buffer at present"
			#endif
		#endif
	#endif
#endif			
