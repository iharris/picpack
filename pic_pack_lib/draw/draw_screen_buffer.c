#include "pic_utils.h"
#include "draw\draw_screen_buffer.h"

 
 
#if DRAW_TOTAL_BUFFER_SIZE <= 256
	uns8 draw_buffer0[DRAW_TOTAL_BUFFER_SIZE];
#else
	uns8 draw_buffer0[256];
	#if DRAW_TOTAL_BUFFER_SIZE <= 512
		uns8 draw_buffer1[DRAW_TOTAL_BUFFER_SIZE - 256];
	#else
		uns8 draw_buffer1[256];
		#if DRAW_TOTAL_BUFFER_SIZE <= 768
			uns8 draw_buffer2[DRAW_TOTAL_BUFFER_SIZE - 512];
		#else
			uns8 draw_buffer2[256]; 
			#if DRAW_TOTAL_BUFFER_SIZE <= 1024
				uns8 draw_buffer3[DRAW_TOTAL_BUFFER_SIZE - 768];
			#else
				#error "Display too big for Draw library.  Must be < 1024 bytes buffer at present"
			#endif
		#endif
	#endif
#endif			



uns8 get_draw_buffer(uns16 address) {

	if (address < 256) {
		return draw_buffer0[address];
	}
	#if DRAW_TOTAL_BUFFER_SIZE > 256
		else if (address < 512) {
			return draw_buffer1[address - 256];
		}
		#if DRAW_TOTAL_BUFFER_SIZE > 512
			else if (address < 768) {
				return draw_buffer2[address - 512];
			}
			#if DRAW_TOTAL_BUFFER_SIZE > 768 
				else if (address < 1024) {
					return draw_buffer3[address - 768];
				}
			#endif	
		#endif		
	#endif		
}

void set_draw_buffer(uns16 address, uns8 data) {

	if (address < 256) {
		draw_buffer0[address] = data;
	}
	#if DRAW_TOTAL_BUFFER_SIZE > 256
		else if (address < 512) {
			draw_buffer1[address - 256] = data;
		}
		#if DRAW_TOTAL_BUFFER_SIZE > 512
			else if (address < 768) {
			draw_buffer2[address - 512] = data;
			}
			#if DRAW_TOTAL_BUFFER_SIZE > 768
				else if (address < 1024) {
					draw_buffer3[address - 768] = data;
				}
			#endif	
		#endif		
	#endif			
}	
