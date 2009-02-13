#include "pic_utils.h"
#include "draw\draw.h"
#include "config.h"
#include "draw\draw_screen_buffer.h"

// DRAW_PIXELS_HIGH

void draw_setup() {
	drv_setup();
}

void draw_init() {
uns8 count;
	drv_init();
	serial_print_debug("buffer_size=", DRAW_TOTAL_BUFFER_SIZE);
	for(count = 0 ; count < sizeof(draw_buffer0) ; count++) {
		draw_buffer0[count] = 0;
	}	
}

void draw_set_pixel(uns8 x, uns8 y, uns8 colour) {
uns8 *buffer;
uns16 buffer_loc;
uns8 loc_byte, loc_bit;
uns8 bit_count;
	
	buffer = &draw_buffer0;	// do differently if we have more than one buffer
	
	buffer_loc = y * DRAW_PIXELS_WIDE + x;

	loc_byte = buffer_loc / DRAW_PIXELS_PER_BYTE;
	loc_bit = buffer_loc & (DRAW_PIXELS_PER_BYTE -1);
	
	bit_count = DRAW_BITS_PER_PIXEL;
	
	while (bit_count > 0) {
		if (test_bit(colour, --bit_count)) {
			set_bit(buffer[loc_byte], loc_bit);
		} else {
			clear_bit(buffer[loc_byte], loc_bit);
		}
		loc_bit--;
	}
//	if (colour) {
//		set_bit(draw_buffer0[loc_byte], loc_bit);
//	} else {
//		clear_bit(draw_buffer0[loc_byte], loc_bit);
//	}
	

}
	
uns8 draw_get_pixel(uns8 x, uns8 y) {
	return 0;
}	


void draw_rect(uns8 x, uns8 y, uns8 width, uns8 height) {


}	

void draw_print_buffer() {

uns8	inv_y, x , y,
		 byte_loc, bit_loc;
uns16	 buffer_loc;

	for(y = 0 ; y < DRAW_PIXELS_HIGH ; y++) {
		inv_y = DRAW_PIXELS_HIGH - 1 - y; // need to print out from the top
		if (inv_y < 10) {
			serial_putc('0');
		}
		serial_print_int(inv_y);
		serial_putc(' ');
		serial_print_int_hex(inv_y * DRAW_PIXELS_WIDE / DRAW_PIXELS_PER_BYTE);
		serial_putc('|');
		for(x = 0 ; x < DRAW_PIXELS_WIDE  ; x++) {
			buffer_loc = inv_y * DRAW_PIXELS_WIDE + x;
			byte_loc = buffer_loc / DRAW_PIXELS_PER_BYTE;
			bit_loc = buffer_loc & (DRAW_PIXELS_PER_BYTE -1);

			if (bit_loc == 0) {
				serial_putc(' ');
				serial_print_int_hex(byte_loc);
				serial_putc(' ');
			}	
			
			if (test_bit(draw_buffer0[byte_loc], bit_loc)) {
				serial_putc('1');
			} else {
				serial_putc('0');
			}
		}
		serial_print_str("|\n");
	}
}			

