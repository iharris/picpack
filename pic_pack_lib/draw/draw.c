#include "pic_utils.h"
#include "draw\draw.h"
#include "config.h"
#include "draw\draw_screen_buffer.h"

/* Draw buffer addressing

4 | U V W X Y
3 | P Q R S T
2 | K L M N O
1 | F G H I J
0 | A B C D E
	---------
	0 1 2 3 4
DRAW_HW_Y_ORIGIN == BOTTOM_LEFT
DRAW_HW_BUFFER_ORIENTATION == HORIZONTAL

0 | A B C D E
1 | F G H I J
2 | K L M N O
3 | P Q R S T
4 | U V W X Y
	---------
	0 1 2 3 4
DRAW_HW_Y_ORIGIN == TOP_LEFT
DRAW_HW_BUFFER_ORIENTATION == HORIZONTAL

0 | E J O T Y
1 | D I N S X
2 | C H M R W
3 | B G L Q V
4 | A F K P U
	---------
	0 1 2 3 4
DRAW_HW_Y_ORIGIN == BOTTOM_LEFT
DRAW_HW_BUFFER_ORIENTATION == VERTICAL


0 | A F K P U
1 | B G L Q V
2 | C H M R W
3 | D I N S X
4 | E J O T Y
	---------
	0 1 2 3 4
DRAW_HW_Y_ORIGIN == TOP_LEFT
DRAW_HW_BUFFER_ORIENTATION == VERTICAL

*/

void draw_clear_screen() {

uns8 count;
	
	#if DRAW_TOTAL_BUFFER_SIZE < 256
		count = 0;
		do {
			draw_buffer0[count] = 0;
			count++;
		} while (count < DRAW_TOTAL_BUFFER_SIZE);
	#else
		count = 0;
		do {
			draw_buffer0[count] = 0;
			count++;
		} while (count != 0);
		#if DRAW_TOTAL_BUFFER_SIZE < 512
			do {
				draw_buffer1[count] = 0;
				count++;
			} while (count < DRAW_TOTAL_BUFFER_SIZE - 256);
		#else
			do {
				draw_buffer1[count] = 0;
				count++;
			} while (count != 0);
			#if DRAW_TOTAL_BUFFER_SIZE > 512
				#if DRAW_TOTAL_BUFFER_SIZE < 768
					do {
						draw_buffer2[count] = 0;
						count++;
					} while (count < DRAW_TOTAL_BUFFER_SIZE - 512);
				#else
					// > 768
					do {
						draw_buffer2[count] = 0;
						count++;
					} while (count != 0);
					
					#if DRAW_TOTAL_BUFFER_SIZE < 1024
						do {
							draw_buffer3[count] = 0;
							count++;
						} while (count < DRAW_TOTAL_BUFFER_SIZE - 768);
					#else
						do {
							draw_buffer3[count] = 0;
							count++;
						} while (count != 0);
					#endif
				#endif	
			#endif
			
		#endif		
		
	#endif
	

}

void draw_setup_io() {
	drv_setup_io();
}

void draw_init() {
	drv_init();
	draw_clear_screen();
}	

 

void draw_set_pixel(uns8 x, uns8 y, uns8 colour) {

uns8 *buffer;
uns16 buffer_loc, loc_byte;
uns8  loc_bit, loc_in_buffer, buffer_num;
uns8  bit_count;
	// inverse here
	// y = DRAW_PIXELS_HIGH - 1 - y;
	
    #if DRAW_HW_Y_ORIGIN == TOP_LEFT
		#if DRAW_HW_BUFFER_ORIENTATION == HORIZONTAL
			buffer_loc = y * DRAW_PIXELS_WIDE + x;
		#else
		// DRAW_HW_BUFFER_ORIENTATION == VERTICAL
			buffer_loc = x * DRAW_PIXELS_HIGH + y;
		#endif
    #else
    // DRAW_HW_Y_ORIENTATION == BOTTOM_LEFT
		#if DRAW_HW_BUFFER_ORIENTATION == HORIZONTAL
			buffer_loc = y * DRAW_PIXELS_WIDE + x;
		#else
		// DRAW_HW_BUFFER_ORIENTATION == VERTICAL
			buffer_loc = x * DRAW_PIXELS_HIGH + y;
		#endif
    #endif
    /*serial_print_int(x);
    serial_putc(' ');
    serial_print_int(y);
    serial_print_str("->");
    serial_print_int(buffer_loc);
    serial_print_nl();
	*/
	buffer_loc = buffer_loc * DRAW_BITS_PER_PIXEL;	         
//	loc_byte = buffer_loc / DRAW_PIXELS_PER_BYTE;
	loc_byte = buffer_loc / 8;
	loc_bit = (buffer_loc & (0x07));
	
/*byte	0	     1        2   
bit		0 2 4 6  0 2 4 6  0 2 4 6
pix	    0 1 2 3  4 5 6 7  8 9 a b 
bit pos 0 2 4 6  8 a c e
*/
	loc_in_buffer = loc_byte & 0xff;
	buffer_num = loc_byte >> 8;
	
	/*serial_print_str(" x=");
	serial_print_int(x);
	serial_print_str(" y=");
	serial_print_int(y);
	serial_print_str(" buffer_loc=");
	serial_print_int(buffer_loc);
	serial_print_str(" loc_byte=");
	serial_print_int(loc_byte);
	serial_print_str(" loc_bit=");
	serial_print_int(loc_bit);
	serial_print_str(" LIB=");
	serial_print_int(loc_in_buffer);
	serial_print_str(" bnum=");
	serial_print_int(buffer_num);
	serial_print_str("\n");
	*/
	if (buffer_num == 0) {
		buffer = &draw_buffer0;
	}
	#if DRAW_TOTAL_BUFFER_SIZE > 256
	
	else if (buffer_num == 1) {
		buffer = &draw_buffer1;

	}
		#if DRAW_TOTAL_BUFFER_SIZE > 512
		else if (buffer_num == 2) {
			buffer = &draw_buffer2;

		}
			#if DRAW_TOTAL_BUFFER_SIZE > 768
			else if (buffer_num == 3) {
			buffer = &draw_buffer3;

			}
			#endif
		#endif
	#endif
	#if DRAW_BITS_PER_PIXEL > 1
	
		bit_count = 0;
	
		while (bit_count < DRAW_BITS_PER_PIXEL) {
			if (test_bit(colour, bit_count)) {
				set_bit(buffer[loc_in_buffer], loc_bit);
			} else {
				clear_bit(buffer[loc_in_buffer], loc_bit);
			}
			bit_count++;
			loc_bit++;
		}
	#else
	
		if (colour) {
			set_bit(buffer[loc_in_buffer], loc_bit);
		} else {
			clear_bit(buffer[loc_in_buffer], loc_bit);
		}	
	#endif	

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

			//if (bit_loc == 0) {
			//	serial_putc(' ');
			//	serial_print_int_hex(byte_loc);
			//	serial_putc(' ');
			//}	
			
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
	
	draw_circle_points(x_centre, y_centre, x, y, 2);

	while (x < y) {
		x++;
		if (p < 0)
			p += 2 * x + 1;
		else {
			y--;
			p += 2 * (x - y + 1);
		}
		draw_circle_points (x_centre, y_centre, x, y, colour);
	}

}
void draw_circle_points2 (int ctr_x, int ctr_y, int pt_x, int pt_y, uns8 colour) {
	// the eight symmetric points
	draw_set_pixel (ctr_x + pt_x + 1, ctr_y + pt_y +1, colour);
	draw_set_pixel (ctr_x + pt_y + 1, ctr_y + pt_x +1, colour);

	draw_set_pixel (ctr_x + pt_x + 1, ctr_y - pt_y, colour);
	draw_set_pixel (ctr_x + pt_y + 1, ctr_y - pt_x, colour);

	draw_set_pixel (ctr_x - pt_x, ctr_y - pt_y, colour);
	draw_set_pixel (ctr_x - pt_y, ctr_y - pt_x, colour);

	draw_set_pixel (ctr_x - pt_x, ctr_y + pt_y +1, colour);
	draw_set_pixel (ctr_x - pt_y, ctr_y + pt_x +1, colour);
	
}
void draw_circle2(int x_centre, int y_centre, int r, uns8 colour) {
	int x, y;
	int p = 1 - r;         // Initial value of decision parameter.
	
	x = 0;
	y = r;
	
	draw_circle_points2(x_centre, y_centre, x, y, colour);
    

	while (x < y) {
		x++;
		if (p < 0)
			p += 2 * x + 1;
		else {
			y--;
			p += 2 * (x - y + 1);
		}
		draw_circle_points2 (x_centre, y_centre, x, y, colour);
	}

}


/*
    private final void circlePoints(int cx, int cy, int x, int y, int pix)
    {
        int act = Color.red.getRGB();
        
        if (x == 0) {
            raster.setPixel(act, cx, cy + y);
            raster.setPixel(pix, cx, cy - y);
            raster.setPixel(pix, cx + y, cy);
            raster.setPixel(pix, cx - y, cy);
        } else 
        if (x == y) {
            raster.setPixel(act, cx + x, cy + y);
            raster.setPixel(pix, cx - x, cy + y);
            raster.setPixel(pix, cx + x, cy - y);
            raster.setPixel(pix, cx - x, cy - y);
        } else 
        if (x < y) {
            raster.setPixel(act, cx + x, cy + y);
            raster.setPixel(pix, cx - x, cy + y);
            raster.setPixel(pix, cx + x, cy - y);
            raster.setPixel(pix, cx - x, cy - y);
            raster.setPixel(pix, cx + y, cy + x);
            raster.setPixel(pix, cx - y, cy + x);
            raster.setPixel(pix, cx + y, cy - x);
            raster.setPixel(pix, cx - y, cy - x);
        }
    }

    public void circleMidpoint(int xCenter, int yCenter, int radius, Color c)
    {
        int pix = c.getRGB();
        int x = 0;
        int y = radius;
        int p = (5 - radius*4)/4;

        circlePoints(xCenter, yCenter, x, y, pix);
        while (x < y) {
            x++;
            if (p < 0) {
                p += 2*x+1;
            } else {
                y--;
                p += 2*(x-y)+1;
            }
            circlePoints(xCenter, yCenter, x, y, pix);
        }
    }
*/

#define FONT_FIRST_CHAR 32
#define FONT_LAST_CHAR 127
#define FONT_HEIGHT 7
extern char PicPack5x7_bitmap_0[1];
extern char PicPack5x7_bitmap_1[1];
extern uns16 PicPack5x7_index[1];

void draw_print_str(uns8 x, uns8 y, uns8 width, uns8 start_pixel, uns8 colour, char *str) {

uns8 my_char;
uns16 index_pos;
uns16 index_pos_next;
uns16 count, s_count;
uns8 sliver, x_origin, y_origin, pixel;
	y_origin = y;
	x_origin = x;
	pixel = 0;
	while (*str != 0) {
		// first look up character in index
		my_char = *str;
		my_char = my_char - 32;


		index_pos = PicPack5x7_index[my_char];
		index_pos_next = PicPack5x7_index[my_char + 1];
			for(count = index_pos ; count < index_pos_next ; count++) {
				if (count < 256) {
					sliver = PicPack5x7_bitmap_0[count];
				} else {
					sliver = PicPack5x7_bitmap_1[count-256];
				}
				// Now iterate over sliver
				if (pixel >= start_pixel) {
					s_count = 0;
					while (s_count < 7) {
						if (test_bit(sliver, 6)) {
							#if DRAW_HW_Y_ORIGIN == BOTTOM_LEFT
								draw_set_pixel(x, y + s_count, colour);
							#else
							// DRAW_HW_Y_ORIGIN == TOP_LEFT
								draw_set_pixel(x, y - s_count, colour);
							#endif
						}	
						sliver <<= 1;
						s_count++;
					}
					x++;
				}
				if (x - x_origin == width) {
					return;
				}	
				pixel++;	
				// Need to add a bit here to only
				// increment to new line when we have got to height chars
				
			}      	
		str++;
		if (pixel >= start_pixel) {
			x++;
		}
		pixel++;
			
		if (x - x_origin == width) {
			return;
		}	
	}
}    
	
