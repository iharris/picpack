#include "pic_utils.h"
#include "draw\draw.h"
#include "config.h"
#include "draw\draw_screen_buffer.h"



void draw_clear_screen() {
uns8 count;
	for(count = 0 ; count < sizeof(draw_buffer0) ; count++) {
		draw_buffer0[count] = 0;
	}	
}

void draw_setup() {
	drv_setup();
}

void draw_init() {
	drv_init();
	draw_clear_screen();
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


void draw_rect(uns8 x, uns8 y, uns8 width, uns8 height, uns8 colour) {
uns8 dx, dy;

	for(dy = y ; dy < y + height ; dy++) {
		for(dx = x ; dx < x + width ; dx++) {
			draw_set_pixel(dx, dy, colour);
		}
	}	
}	

void draw_print_buffer() {
#ifdef DRAW_DEBUG
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
#endif	
}			

void draw_line(uns8 x0, uns8 y0, uns8 x1, uns8 y1, uns8 colour) {

    int dy = y1 - y0;
    int dx = x1 - x0;
    int stepx, stepy;

    if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
    if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
    dy <<= 1;                                                  // dy is now 2*dy
    dx <<= 1;                                                  // dx is now 2*dx

    draw_set_pixel(x0, y0, colour);
    if (dx > dy) {
        int fraction = dy - (dx >> 1);                         // same as 2*dy - dx
        while (x0 != x1) {
            if (fraction >= 0) {
                y0 += stepy;
                fraction -= dx;                                // same as fraction -= 2*dx
            }
            x0 += stepx;
            fraction += dy;                                    // same as fraction -= 2*dy
			draw_set_pixel(x0, y0, colour);
        }
    } else {
        int fraction = dx - (dy >> 1);
        while (y0 != y1) {
            if (fraction >= 0) {
                x0 += stepx;
                fraction -= dy;
            }
            y0 += stepy;
            fraction += dx;
			draw_set_pixel(x0, y0, colour);
        }
    }
}


void draw_circle_lines (int ctr_x, int ctr_y, int pt_x, int pt_y, uns8 colour) {
	draw_line(ctr_x - pt_x, ctr_y + pt_y, ctr_x + pt_x, ctr_y + pt_y, colour);
	draw_line(ctr_x - pt_x, ctr_y - pt_y, ctr_x + pt_x, ctr_y - pt_y, colour);
	draw_line(ctr_x + pt_y, ctr_y + pt_x, ctr_x - pt_y, ctr_y + pt_x, colour);
	draw_line(ctr_x + pt_y, ctr_y - pt_x, ctr_x - pt_y, ctr_y - pt_x, colour);
}

void draw_filled_circle(int x_centre, int y_centre, int r, uns8 colour) {
	int x,y;
	int p = 1 - r;         // Initial value of decision parameter.

	x = 0;
	y = r;
	
	draw_circle_lines(x_centre, y_centre, x, y, colour);
    
	while (x < y) {
		x++;
		if (p < 0)
			p += 2 * x + 1;
		else {
			y--;
			p += 2 * (x - y) + 1;
		}
		draw_circle_lines (x_centre, y_centre, x, y, colour);
	}

}


void draw_circle_points (int ctr_x, int ctr_y, int pt_x, int pt_y, uns8 colour) {
	// the eight symmetric points
	draw_set_pixel (ctr_x + pt_x, ctr_y + pt_y, colour);
	draw_set_pixel (ctr_x - pt_x, ctr_y + pt_y, colour);

	draw_set_pixel (ctr_x + pt_x, ctr_y - pt_y, colour);
	draw_set_pixel (ctr_x - pt_x, ctr_y - pt_y, colour);
	
	draw_set_pixel (ctr_x + pt_y, ctr_y + pt_x, colour);
	draw_set_pixel (ctr_x - pt_y, ctr_y + pt_x, colour);
	
	draw_set_pixel (ctr_x + pt_y, ctr_y - pt_x, colour);
	draw_set_pixel (ctr_x - pt_y, ctr_y - pt_x, colour);
}

void draw_circle(int x_centre, int y_centre, int r, uns8 colour) {
	int x,y;
	int p = 1 - r;         // Initial value of decision parameter.

	x = 0;
	y = r;
	
	draw_circle_points(x_centre, y_centre, x, y, colour);
    
	while (x < y) {
		x++;
		if (p < 0)
			p += 2 * x + 1;
		else {
			y--;
			p += 2 * (x - y) + 1;
		}
		draw_circle_points (x_centre, y_centre, x, y, colour);
	}

}

#define FONT_FIRST_CHAR 32
#define FONT_LAST_CHAR 127
extern char PicPack5x7_bitmap_0[1];
extern char PicPack5x7_bitmap_1[1];
extern uns16 PicPack5x7_index[1];

void draw_print_str(uns8 x, uns8 y, uns8 colour, char *str) {

uns8 my_char;
uns16 index_pos;
uns16 index_pos_next;
uns16 count, s_count, y_origin;
uns8 sliver;
	y_origin = y;
	while (*str != 0) {
		// first look up character in index
		my_char = *str;
		serial_print_str("Char=");
		serial_putc(my_char);
		serial_print_nl();
		serial_print_debug("Char=", my_char);
		my_char = my_char - 32;

		index_pos = PicPack5x7_index[my_char];
		serial_print_debug("index_pos=", index_pos);
		index_pos_next = PicPack5x7_index[my_char + 1];
		serial_print_debug("index_pos_next=", index_pos_next);
			for(count = index_pos ; count < index_pos_next ; count++) {
				serial_print_debug("count=", count);
				if (count < 256) {
					sliver = PicPack5x7_bitmap_0[count];
				} else {
					sliver = PicPack5x7_bitmap_1[count-256];
				}
				serial_print_debug("sliver=", sliver);
				// Now iterate over sliver
				s_count = 0;
				while (s_count < 7) {
					serial_print_debug("s_count=", s_count);
					if (test_bit(sliver, 6)) {
						draw_set_pixel(x, y + s_count, colour);
					}	
					sliver <<= 1;
					s_count++;
				}
				x++;
				// Need to add a bit here to only
				// increment to new line when we have got to height chars
				
			}      	
			x++;
		str++;
	}
}    
	
