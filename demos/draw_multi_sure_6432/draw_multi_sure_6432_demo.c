//-----------------------------------------------------
// Pic Pack library
// 
// draw_multi_sure_6432.c
//
// Demonstration routines showing the draw library
// used with the Sure Electronics 6432 board
//
// Ian Harris 2009
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if it breaks, you get to keep both pieces" license.
//-----------------------------------------------------

// Pic Pack includes
#include "config.h"
#include "pic_utils.h"
#include "pic_serial.h"
#include "pic_timer.h"
#include "draw\draw.h"
// System includes
#include "string.h"
#include "stdlib.h"
#include "sure_6432.h"

#include "draw\draw_screen_buffer.h"

void flash() {

	lata.0 = 1;
	delay_ms(200);
	lata.0 = 0;
	delay_ms(200);
}

void timer_0_callback() {
	drv_refresh();
}
	
// Interrupt routine - - - - - - - - - -
void interrupt() {
	timer_handle_0_isr(); 
	serial_handle_tx_isr();
	serial_handle_rx_isr();
}


// configure_system
//
// Get the pic ready for everything we want to do

void configure_system() {
	    
	kill_interrupts();	// turn off interrupts just in case

	turn_analog_inputs_off();	// kill those pesky analogue inputs

	#ifdef _PIC16
		timer_setup_0(TIMER_8BIT_MODE, TIMER_PRESCALER_1_TO_4, 0xff - 250 - 1);
	#endif

	#ifdef _PIC18
		timer_setup_0(TIMER_16BIT_MODE, TIMER_PRESCALER_OFF, 0xffff - 65000 -1);
	#endif	 
	
	
	serial_setup(BRGH_HIGH_SPEED, SPBRG_19200);

	turn_peripheral_ints_on();
	turn_global_ints_on();

	draw_setup_io();
	draw_init();

}

void draw_number_ticks() {
		draw_set_pixel(61, 31,1);
		draw_set_pixel(57,	46,1);
		draw_set_pixel(46,	57,1);
		draw_set_pixel(32,	61,1);
		draw_set_pixel(17,	57,1);
		draw_set_pixel(6,	46,1);
		draw_set_pixel(3,	31,1);
		draw_set_pixel(6,	17,1);
		draw_set_pixel(17,	6,1);
		draw_set_pixel(32,	2,1);
		draw_set_pixel(46,	6,1);
		draw_set_pixel(57,	17,1);

		draw_set_pixel(31,31, 2);
		draw_set_pixel(32,31, 2);
		draw_set_pixel(31,32, 2);
		draw_set_pixel(32,32, 2);
}


rom char *minute_hand_y = {
4,
5,
5,
6,
7,
8,
10,
11,
13,
16,
18,
21,
23,
26,
29,
31,
34,
37,
40,
42,
45,
47,
50,
52,
53,
55,
56,
57,
58,
58,
58,
58,
58,
57,
56,
55,
53,
52,
50,
47,
45,
42,
40,
37,
34,
31,
29,
26,
23,
21,
18,
16,
13,
11,
10,
8,
7,
6,
5,
5
};

rom char *minute_hand_x = {
32,
34,
37,
40,
42,
45,
47,
50,
52,
53,
55,
56,
57,
58,
58,
59,
58,
58,
57,
56,
55,
53,
52,
50,
47,
45,
42,
40,
37,
34,
32,
29,
26,
23,
21,
18,
16,
13,
11,
10,
8,
7,
6,
5,
5,
5,
5,
5,
6,
7,
8,
10,
11,
13,
16,
18,
21,
23,
26,
29,
};

rom char *hour_hand_y = {
14	,
15	,
15	,
15	,
16	,
17	,
18	,
19	,
20	,
22	,
23	,
25	,
27	,
29	,
31	,
32	,
34	,
36	,
38	,
40	,
41	,
43	,
45	,
46	,
47	,
48	,
49	,
50	,
50	,
50	,
50	,
50	,
50	,
50	,
49	,
48	,
47	,
46	,
45	,
43	,
41	,
40	,
38	,
36	,
34	,
32	,
31	,
29	,
27	,
25	,
23	,
22	,
20	,
19	,
18	,
17	,
16	,
15	,
15	,
15	,
};

rom char *hour_hand_x = {
32	,
33	,
35	,
37	,
39	,
41	,
42	,
44	,
45	,
46	,
47	,
48	,
49	,
49	,
49	,
50	,
49	,
49	,
49	,
48	,
47	,
46	,
45	,
44	,
42	,
41	,
39	,
37	,
35	,
33	,
32	,
30	,
28	,
26	,
24	,
23	,
21	,
19	,
18	,
17	,
16	,
15	,
14	,
14	,
14	,
14	,
14	,
14	,
14	,
15	,
16	,
17	,
18	,
19	,
21	,
23	,
24	,
26	,
28	,
30	,
};




	
		
void draw_minute_hand(uns8 minutes) {
	uns8 startx, starty;
	
	if (minutes < 16) {
		startx = 32;
		starty = 31;
	} else if (minutes < 31) {
		startx = 32;
		starty = 32;
	} else if (minutes < 46) {
		startx = 31;
		starty = 32;
	} else {
		startx = 31;
		starty = 31;
	}
	
	draw_line(startx, starty, minute_hand_x[minutes], minute_hand_y[minutes], 3);
		
}
	
void draw_hour_hand(uns8 hours, uns8 minutes) {
	uns8 startx, starty;
	uns8 index;
	
	index = hours * 5 + minutes / 12;
	if (index < 16) {
		startx = 32;
		starty = 31;
	} else if (index < 31) {
		startx = 32;
		starty = 32;
	} else if (index < 46) {
		startx = 31;
		starty = 32;
	} else {
		startx = 31;
		starty = 31;
	}
	/*serial_print_str("m:");
	serial_print_int(minutes);
	serial_putc(' ');
	serial_print_int(hour_hand_x[minutes]);
	serial_putc(' ');
	serial_print_int(hour_hand_y[minutes]);*/
	draw_line(startx, starty, hour_hand_x[index], hour_hand_y[index], 2);
		
}




void main() {

	trisa = 0;
	flash();
	configure_system();
	flash();
	delay_ms(100);

	serial_print_str("\n\nPicPack Sure 6432 Led display demo\n");
	serial_print_str( "\n<");
	serial_print_str(__TIME__);
	serial_putc(' ');
	serial_print_str(__DATE__);
	serial_print_str(">\n");

	serial_print_str("total buffer = ");
	serial_print_int(DRAW_TOTAL_BUFFER_SIZE);
	serial_print_nl();
	serial_print_str("wide = ");
	serial_print_int(DRAW_PIXELS_WIDE);
	serial_print_nl();
	serial_print_str("high = ");
	serial_print_int(DRAW_PIXELS_HIGH);
	serial_print_nl();
	serial_print_str("ppb = ");
	serial_print_int(DRAW_PIXELS_PER_BYTE);
	serial_print_nl();
	draw_clear_screen();
			draw_clear_screen();
			draw_number_ticks();

			draw_circle2(31, 31, 31, 2);	
	timer_start_0();	// kick that timer off...	
	
	for(;;){
		/*delay_ms(250);
		
		draw_set_pixel(0,0,1);
		draw_paint();
		
		delay_s(1);
		draw_set_pixel(23,0,1);
		draw_paint();
		
		delay_s(1);
		draw_set_pixel(23,15,1);
		draw_paint();

		delay_s(1);
		draw_set_pixel(0,15,1);
		draw_paint();

		delay_s(1);
		draw_set_pixel(0,0,0);
		draw_paint();

		delay_s(1);
		draw_set_pixel(23,0,0);
		draw_paint();
		
		delay_s(1);
		draw_set_pixel(23,15,0);
		draw_paint();

		delay_s(1);
		draw_set_pixel(0,15,0);
		draw_paint();

		delay_s(1);
		draw_line(0, 0, 23, 15, 1);
		draw_paint();

		delay_s(1);
		draw_line(0, 15, 23, 0, 1);
		draw_paint();

		delay_s(1);
		draw_line(0, 0, 23, 15, 0);
		draw_paint();

		delay_s(1);
		draw_line(0, 15, 23, 0, 0);
		draw_paint();

		delay_s(1);
		draw_circle(12, 8, 1, 1);
		draw_paint();

		delay_s(1);
		draw_circle(12, 7, 2, 1);
		draw_paint();

		delay_s(1);
		draw_circle(12, 7, 3, 1);
		draw_paint();

		delay_s(1);
		draw_circle(12, 7, 4, 1);
		draw_paint();
		delay_s(1);
		draw_circle(12, 7, 5, 1);
		draw_paint();
		delay_s(1);
		draw_circle(12, 7, 6, 1);
		draw_paint();
		delay_s(1);
		draw_circle(12, 7, 7, 1);
		draw_paint();
		
		delay_s(1);*/
		
		uns16 countp;
		
		/*draw_clear_screen();
		serial_print_str("Pixels\n");
		draw_set_pixel(0,0,1);
		draw_set_pixel(DRAW_PIXELS_WIDE - 1, 0, 1);
		draw_set_pixel(DRAW_PIXELS_WIDE - 1, DRAW_PIXELS_HIGH - 1, 1);
		draw_set_pixel(0, DRAW_PIXELS_HIGH - 1, 1);
		

		for (countp = 0; countp < 200; countp++) {
			draw_paint();
		}	
		
		draw_clear_screen();
		serial_print_str("Rectangle\n");
		draw_rect(0, 0, DRAW_PIXELS_WIDE  , DRAW_PIXELS_HIGH  , 1);

		for (countp = 0; countp < 100; countp++) {
			draw_paint();
		}	
		
		draw_clear_screen();
		serial_print_str("Rectangle\n");
		draw_rect(2, 2, DRAW_PIXELS_WIDE -4 , DRAW_PIXELS_HIGH - 4, 2);
		draw_set_pixel(0,0,1);
		draw_set_pixel(DRAW_PIXELS_WIDE - 1, 0, 1);
		draw_set_pixel(DRAW_PIXELS_WIDE - 1, DRAW_PIXELS_HIGH - 1, 1);
		draw_set_pixel(0, DRAW_PIXELS_HIGH - 1, 1);
				
		for (countp = 0; countp < 100; countp++) {
			draw_paint();
		}	
		
		draw_clear_screen();
		serial_print_str("Rectangle\n");
		draw_rect(1, 1, DRAW_PIXELS_WIDE -2, DRAW_PIXELS_HIGH - 2 , 1);
		draw_circle(32, 32, 31,0);
		for (countp = 0; countp < 200; countp++) {
			draw_paint();
		}	
		*/
		/*draw_clear_screen();
		
		draw_circle(31, 31, 31, 2);
		for (countp = 0; countp < 500; countp++) {
			draw_paint();
		}
		*/
		draw_clear_screen();
		
		uns8 minutes =59;
		uns8 hours = 11;
		for(;;) {
			minutes++;
			if (minutes == 60) {
				minutes = 0;
				hours++;
				if (hours == 12) {
					hours = 0;
				}
					
			}
			draw_clear_screen();
			draw_number_ticks();
			draw_minute_hand(minutes);
			draw_hour_hand(hours, minutes);
			draw_circle2(31, 31, 31, 2);
			draw_paint();
			serial_putc('L');
			delay_ms(100);	
					
		}

	}
	
}	// main

