//----------------------------------------------------- 
// Pic Pack library 
//  
// draw_multi_sure_clock.c 
// 
// Grab a couple of Sure electronics 6432 boards and make yourself 
// one cool clock 
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
 
uns8 minutes =59; 
uns8 hours = 11; 
uns8 seconds = 59;

void check_time() {
	// we'll fake it for now
	seconds++;
	if (seconds == 60) {
		seconds = 0;
		minutes++;
		if (minutes == 60) {
			minutes = 0;
			hours++;
			if (hours == 12) {
				hours = 0;
			}
		}
	}
}				 
 
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
		draw_set_pixel(61, 32,2); 
		draw_set_pixel(57,	46,2); 
		draw_set_pixel(46,	57,2); 
		draw_set_pixel(31,	61,2); 
		draw_set_pixel(17,	57,2); 
		draw_set_pixel(6,	46,2); 
		draw_set_pixel(2,	31,2); 
		draw_set_pixel(6,	17,2); 
		draw_set_pixel(17,	6,2); 
		draw_set_pixel(32,	2,2); 
		draw_set_pixel(46,	6,2); 
		draw_set_pixel(57,	17,2); 
 
		draw_set_pixel(31,31, 1); 
		draw_set_pixel(32,31, 1); 
		draw_set_pixel(31,32, 1); 
		draw_set_pixel(32,32, 1); 
} 
 
 
rom char *minute_hand_y = { 
 4, 4, 4, 5, 6, 7, 9,11,13,16,18,21,23,26,29,
31,34,37,40,42,45,47,50,52,53,55,56,57,58,58,
58,58,58,57,56,55,53,52,50,47,45,42,40,37,34,
31,29,26,23,21,18,16,13,11,10, 8, 7, 6, 5, 5}; 
 
rom char *minute_hand_x = { 
32,34,37,40,42,45,47,50,52,54,56,57,58,59,59,
59,58,58,57,56,55,53,52,50,47,45,42,40,37,34,
32,29,26,23,21,18,16,13,11,10, 8, 7, 6, 5, 5,
 5, 5, 5, 6, 7, 8,10,11,13,16,18,21,23,26,29,}; 
 
rom char *hour_hand_y = { 
13	,13	,13	,14	,14	,15	,16	,18	,19	,20	,22	,25	,27	,29	,31	,
32	,34	,36	,38	,40	,41	,43	,45	,46	,47	,48	,49	,50	,50	,50	,
50	,50	,50	,50	,49	,48	,47	,46	,45	,43	,41	,40	,38	,36	,34	,
32	,31	,29	,27	,25	,23	,22	,20	,19	,18	,17	,16	,15	,15	,15	,}; 
 
rom char *hour_hand_x = { 
32	,33	,35	,37	,39	,41	,42	,44	,45	,46	,48	,49	,50	,50	,50	,
50	,49	,49	,49	,48	,47	,46	,45	,44	,42	,41	,39	,37	,35	,33	,
32	,30	,28	,26	,24	,23	,21	,19	,18	,17	,16	,15	,14	,14	,14	,
14	,14	,14	,14	,15	,16	,17	,18	,19	,21	,23	,24	,26	,28	,30	,}; 
 
rom char *second_hand_y = { 
2	,
2	,
3	,
3	,
5	,
6	,
8	,
9	,
12	,
14	,
17	,
19	,
22	,
25	,
29	,
32	,
35	,
38	,
41	,
44	,
46	,
49	,
51	,
54	,
55	,
57	,
58	,
60	,
60	,
61	,
61	,
61	,
60	,
60	,
58	,
57	,
55	,
54	,
51	,
49	,
46	,
44	,
41	,
38	,
35	,
32	,
29	,
26	,
23	,
20	,
17	,
15	,
13	,
10	,
9	,
7	,
6	,
4	,
4	,
3	,



};

rom char *second_hand_x = {
32	,
35	,
38	,
41	,
44	,
46	,
49	,
51	,
54	,
55	,
57	,
58	,
60	,
60	,
61	,
60	,
60	,
59	,
59	,
57	,
56	,
54	,
53	,
50	,
48	,
46	,
43	,
40	,
37	,
34	,
31	,
28	,
25	,
22	,
19	,
17	,
14	,
12	,
9	,
8	,
6	,
5	,
3	,
3	,
2	,
2	,
2	,
3	,
3	,
5	,
6	,
8	,
9	,
12	,
14	,
17	,
19	,
22	,
25	,
28	,


};
	
draw_test_hands() {
	uns8 count;
	
	for (count=0;count++;count<60) {
		draw_set_pixel(second_hand_x[count], second_hand_y[count],3);
		draw_set_pixel(minute_hand_x[count], minute_hand_y[count],2);
		draw_set_pixel(hour_hand_x[count],   hour_hand_y[count], 1);
	}
		
}	

void draw_second_hand(uns8 seconds) {

	draw_set_pixel(second_hand_x[seconds], second_hand_y[seconds], 3); 
	//draw_set_pixel(minute_hand_x[seconds], minute_hand_y[seconds], 2); 
	//draw_set_pixel(hour_hand_x[seconds], hour_hand_y[seconds], 1); 
}	
		 
void draw_minute_hand(uns8 minutes) { 
	uns8 startx, starty;
	
	if (minutes < 15) { 
		startx = 32; 
		starty = 31;
	} else if (minutes < 30) { 
		startx = 32; 
		starty = 32;
	} else if (minutes < 45) { 
		startx = 31; 
		starty = 32;
	} else { 
		startx = 31; 
		starty = 31; 
	} 
	 
	draw_line(startx, starty, minute_hand_x[minutes], minute_hand_y[minutes], 3); 
		 
} 
	 
void draw_hour_hand(uns8 hours, uns8 minutes) { 
	uns8 startx, starty, endx, endy; 
	uns8 index, lookup; 
	 
	index = hours * 5 + minutes / 12;
	 
	if (index < 15) { 
		startx = 32; 
		starty = 31;
	} else if (index < 30) { 
		startx = 32; 
		starty = 32;
	} else if (index < 45) { 
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
 
	serial_print_str("\n\nPicPack Sure 6432 clock\n"); 
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
	timer_start_0();	// kick that timer off...	 

	
/*
 General Run
 
	for(;;){ 
		check_time();

		draw_clear_screen();
		draw_print_str(0, 6, 16, 0, 3, "28");
		draw_print_str(54, 6, 11, 0, 3, "Mo");
		draw_print_str(0,  63, 11, 0, 3, "08");
		draw_print_str(51, 63,  16, 0, 3, "Fb"); //Ja Fb Mr Ap My Jn Jl Au Se Oc No De
		draw_circle2(31, 31, 31, 1); // outside
		//draw_circle2(31, 31, 29, 2); seconds
		//draw_circle2(31, 31, 27, 3);	// minutes
		//draw_circle2(31, 31, 18, 2);	//hour
		draw_minute_hand(minutes); 
		draw_hour_hand(hours, minutes); 
		draw_number_ticks();
		draw_second_hand(seconds);
		draw_paint(); 
		delay_ms(1);	 
	} 
*/
		draw_clear_screen();
		draw_print_str(0, 6, 16, 0, 3, "28");
		draw_print_str(54, 6, 11, 0, 3, "Mo");
		draw_print_str(0,  63, 11, 0, 3, "08");
		draw_print_str(51, 63,  16, 0, 3, "Fb"); //Ja Fb Mr Ap My Jn Jl Au Se Oc No De
		
		draw_circle2(31, 31, 31, 1); // outside
		//draw_circle2(31, 31, 29, 2); seconds
		//draw_circle2(31, 31, 27, 3);	// minutes
		//draw_circle2(31, 31, 18, 2);	//hour
		//draw_minute_hand(minutes); 
		//draw_hour_hand(hours, minutes); 
		draw_test_hands();
		draw_number_ticks();
		//draw_second_hand(seconds);
		draw_paint(); 
		//delay_ms(1);	 
		for(;;) {};
} 
	  // void draw_print_str(uns8 x, uns8 y, uns8 width, uns8 start_pixel, uns8 colour, char *str)
#define TEST_IT 2

#if TEST_IT == 1 | TEST_IT == 2
	#warning "test_it = X!"
#endif	