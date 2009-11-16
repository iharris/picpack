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
 
#define HAVE_DS1307 
#define HAVE_DS1631 
 
 
// Pic Pack includes 
#include "config.h" 
#include "pic_utils.h" 
#include "pic_serial.h" 
#include "pic_timer.h" 
#include "draw\draw.h"

#ifdef HAVE_DS1307
	#include "i2c.h"
	#include "ds1307.h"
#endif	

#ifdef HAVE_DS1631
	#include "i2c.h"
	#include "ds1631.h"
	#define DS1631_ADDR 0x00

#endif

uns8 seconds_trigger = 0;
 
// System includes 
#include "string.h" 
#include "stdlib.h" 
#include "sure_6432.h" 
 
#include "draw\draw_screen_buffer.h" 

// Time keeping variables
 
uns8 minutes =59; 
uns8 hours = 11; 
uns8 seconds = 59;
uns8 day = 1;
uns8 date = 1;
uns8 month = 1;

uns16 temp;
uns8 temp_requested = 0;

// Put received characters in here to see
// if a command has been typed

char serial_buffer[6]= '\0';

// See if we need to do a task
uns8 do_task = 0;	

// Interrupt routine - - - - - - - - - - 

void interrupt() { 
	timer_handle_0_isr();  
	serial_handle_tx_isr(); 
	serial_handle_rx_isr(); 
	if (test_bit(intcon, INT0IF)) {	
		clear_bit(intcon, INT0IF);	// clear the interrupt
		seconds_trigger = 1;
	}	
	
} 
 


void check_time() {

#ifndef HAVE_DS1307
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
#else
	uns8 year;

	
	minutes = rtc_get_minutes();
	hours   = rtc_get_hours();
	seconds = rtc_get_seconds();
	day 	= rtc_get_day();
	date 	= rtc_get_date();
	month 	= rtc_get_month();
	year 	= rtc_get_year();
	

#endif
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
		timer_setup_0(TIMER_16BIT_MODE, TIMER_PRESCALER_1_TO_2, 0xffff - 60000 -1); // was 1to2 65000
	#endif	  
	 
	
	serial_setup(BRGH_HIGH_SPEED, SPBRG_19200); 
	
 
	draw_setup_io(); 
	draw_init(); 

	#ifdef HAVE_DS1307 
		rtc_setup_io();
		make_input(int_port, int_pin);
		rtc_set_config(0b00010000);	// Enable interrupt once per second
		
		set_bit(intcon, INT0IE);	//	Assuming it's portb.0, this will interrupt 
	#endif
	
	#ifdef HAVE_DS1631
		ds1631_setup_io();

		// 12 bit temp, one shot mode
		ds1631_set_config(DS1631_ADDR, 0b00001101);
	#endif
	
	turn_peripheral_ints_on(); 
	turn_global_ints_on(); 
 
} 
 
// Process_rx
//
// Keep adding characters to our input buffer
// when we enter a command, then act on it

void process_rx() {
	
	uns8 len,	// length of string
		 rec;	// received character
	uns8 first_char;
	
	rec = serial_getc();	// get the character from the fifo
	// Neat trick to allow one button downloads
	if (rec == MAGIC_BOOSTBLOADER_REQUEST) {
		boostbloader();
	}	
	
	serial_putc(rec);	// print it out so we can see what we typed

	if (rec == '\r') {	// did we press return?
		first_char = serial_buffer[0];
		switch (first_char) {
			case 't':
				do_task = 1;
				break;
		    case 'c':
				do_task = 2;
				break;
			default: 
				if ((first_char == 's') && (strlen(serial_buffer)>2)) {
					uns8 setting = atoui_dec(&serial_buffer[2]);
					serial_print_str(" Setting to ");
					serial_print_int(setting);
					serial_putc(' ');
					switch (serial_buffer[1]) {
					case 's' :
						rtc_set_seconds(setting);
						serial_print_str("sec");
						break;
					case 'm' :
						rtc_set_minutes(setting);
						serial_print_str("min");
						break;
					case 'h' :
						rtc_set_hours(setting);
						serial_print_str("hour");
						break;
					case 'a' :
						rtc_set_day(setting);
						serial_print_str("day");
						break;
					case 't' :
						rtc_set_date(setting);
						serial_print_str("date");
						break;
					case 'o' :
						rtc_set_month(setting);
						serial_print_str("month");
						break;
					case 'y' :
						rtc_set_year(setting);
						serial_print_str("year");
						break;
					default:
						serial_print_str(" huh?");		
					}
					serial_putc('\n');	
				}	
				
		}		
		serial_buffer[0] = '\0';	// clear the buffer
        serial_putc('\n');	// print new line
        serial_putc('>');	// print the prompt
	} else {
		len = strlen(serial_buffer);	// add the character to our line input
		serial_buffer[len] = rec;
		serial_buffer[len+1] = '\0';
	}

}
 
 
void clock_tick(uns8 x, uns8 y, uns8 col) {
	draw_set_pixel(x, y, col);
	draw_set_pixel(x-1, y, 1);
	draw_set_pixel(x+1, y, 1);
	draw_set_pixel(x, y-1, 1);
	draw_set_pixel(x, y+1, 1);
}	 
 
 
void clock_number_ticks() { 
		clock_tick(61, 32,2); 
		clock_tick(57,	46,2); 
		clock_tick(46,	57,2); 
		clock_tick(31,	61,2); 
		clock_tick(17,	57,2); 
		clock_tick(6,	46,2); 
		clock_tick(2,	31,2); 
		clock_tick(6,	17,2); 
		clock_tick(17,	6,2); 
		clock_tick(32,	2,2); 
		clock_tick(46,	6,2); 
		clock_tick(57,	17,2); 
 
		//draw_set_pixel(31,31, 1); 
		//draw_set_pixel(32,31, 1); 
		//draw_set_pixel(31,32, 1); 
		//draw_set_pixel(32,32, 1); 
} 
 
 
rom char *minute_hand_y = { 
 4, 4, 4, 5, 6, 7, 9,11,13,16,18,20,23,26,29,
32,34,37,40,43,45,47,50,52,54,56,57,58,59,59,
59,59,59,58,57,56,54,52,50,47,45,42,40,37,34,
31,29,26,23,21,18,16,13,11, 9, 7, 6, 5, 4, 4}; 
 
rom char *minute_hand_x = { 
32,34,37,40,43,45,47,50,52,54,56,57,58,59,59,
59,59,59,58,57,56,54,52,50,47,45,43,40,37,34,
31,29,26,23,20,18,16,13,11, 9, 7, 6, 5, 4, 4,
 4, 4, 4, 5, 6, 7,9,11,13,16,18,21,23,26,29,}; 
 
 
 
 
 
rom char *hour_hand_y = { 
12	,12	,12	,13	,14	,14	,16	,17	,19	,21	,23	,24	,26	,28	,30	,
32	,34	,36	,38	,40	,42	,44	,46	,47	,49	,49	,50	,50	,51	,51	,
51	,51	,51	,50	,50	,49	,47	,46	,44	,42	,40	,39	,37	,35	,33	,
31	,29	,27	,25	,22	,21	,19	,17	,16	,14	,13	,13	,13	,12	,12	,}; 
 
rom char *hour_hand_x = { 
32	,34	,36	,38	,40	,41	,43	,45	,47	,48	,49	,50	,50	,51	,51	,
51	,51	,51	,50	,49	,48	,47	,45	,43	,41	,41	,39	,37	,35	,33	,
31	,29	,27	,26	,24	,22	,20	,18	,16	,15	,14	,13	,13	,12	,12	,
12	,12	,12	,13	,14	,15	,16	,18	,20	,22	,24	,25	,26	,28	,30	,}; 
 
 
 
 
 
rom char *second_hand_y = { 
2	,2	,3	,3	,5	,6	,8	,9	,11	,14	,17	,19	,22	,25	,29	,
32	,35	,38	,41	,44	,46	,49	,52	,54	,55	,57	,58	,60	,60	,61	,
61	,61	,60	,60	,58	,57	,55	,53	,51	,49	,46	,44	,41	,38	,34	,
31	,29	,25	,22	,19	,17	,14	,11	,9	,8	,6	,5	,3	,3	,2	,
};

rom char *second_hand_x = {
32	,35	,38	,41	,44	,46	,49	,51	,53	,55	,57	,58	,60	,60	,61	,
61	,61	,60	,60	,58	,57	,55	,53	,51	,49	,46	,44	,41	,38	,35	,
31	,28	,25	,22	,19	,17	,14	,11	,9	,8	,6	,5	,3	,3	,2	,
2	,2	,3	,3	,5	,6	,8	,10	,12	,14	,17	,19	,22	,25	,28	,
};
	
draw_test_hands() {
	uns8 count;
	
	for (count=0;count<60;count++) {
		draw_set_pixel(second_hand_x[count], second_hand_y[count],3);
		//draw_set_pixel(minute_hand_x[count], minute_hand_y[count],3);
		//draw_set_pixel(hour_hand_x[count],   hour_hand_y[count], 3);
	}
		
}	

void clock_second_hand(uns8 seconds) {
	uns8 startx, starty;
	
	if (seconds >= 45) { 	
		startx = 31; 
		starty = 31;
	} else if (seconds >= 30) { 
		startx = 31; 
		starty = 32;
	} else if (seconds >= 15) { 
		startx = 32; 
		starty = 32;
	} else { 
		startx = 32; 
		starty = 31; 
	} 

	draw_line(startx, starty, second_hand_x[seconds], second_hand_y[seconds], 1); 
}	
		 
void clock_minute_hand(uns8 minutes) { 
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
	 
void clock_hour_hand(uns8 hours, uns8 minutes) { 
	uns8 startx, starty, endx, endy; 
	uns8 index, lookup; 
	if (hours > 12) {
		hours = hours - 12;
	}	 
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
	draw_line(startx, starty, hour_hand_x[index], hour_hand_y[index], 2); 
	 
} 
 
void clock_date(uns8 date) {

char buffer[3];
	if (date > 9) {
		buffer[0] = '0' + date / 10;
		buffer[1] = '0' + date % 10;
	} else {
		buffer[0] = '0';
		buffer[1] = '0' + date;
	}
	buffer[2] = '\0';
	draw_print_str(0,  63, 11, 0, 3, buffer);
}

char *day_string = "Mo\0Tu\0We\0Th\0Fr\0Sa\0Su";
	 
void clock_day(uns8 day) {
	day = day - 1;
	day = day * 3;
	draw_print_str(54, 6, 11, 0, 3, &day_string[day]);
}

char *month_string = "Ja\0Fb\0Mr\0Ap\0Ma\0Ju\0Jl\0Au\0Se\0Oc\0No\0De\0";

void clock_month(uns8 month) {
	month = month - 1;
	month = month * 3;
    draw_print_str(54, 63, 16, 0, 3, &month_string[month]);	
}
 
void clock_temperature(uns16 temp) {
char buffer[5];
uns8 count;
uns8 temp_int;
bit negative = 0;

	temp_int = temp >> 8; 

	if (temp.15 == 1) {	// negative
		negative = 1;
		temp = temp | 0x0f;	// Set lower 4 bits
		temp = ~temp;	// Take complement
		temp = temp + 1;		// Add one
	}

	temp_int = temp >> 8;
	
	count = 0;
	if (negative) {
		buffer[0] = '-';
		count = 1;
	}
	
	if (temp_int > 9) {
		buffer[count++] = '0' + temp / 10;
		buffer[count++] = '0' + temp % 10;
	} else {
		buffer[count++] = '0';
		buffer[count++] = '0' + temp;
	}
	buffer[count++] = '\127'; // degree sign
	buffer[count++] = '\0';
	
	draw_print_str(0, 6, 16, 0, 3, buffer);
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
	//rtc_start_clock();

	check_time();
	seconds_trigger = 1;
	for(;;) {
		if (serial_rx_avail()) {
			process_rx();
		}

		if (seconds_trigger) {
			seconds_trigger = 0;
			check_time();
			#ifdef HAVE_DS1631
				if (temp_requested) {	// temp requested, so lets see if it has finished
					uns8 config = ds1631_get_config(DS1631_ADDR);
					if (test_bit(config, 7)) {	// the DONE bit
						temp_requested = 0;
						temp = ds1631_get_temp(DS1631_ADDR);
						serial_print_str("temp=");
						serial_print_int(temp);
						serial_print_nl();
					}	
				}
			#endif	
			draw_clear_screen();
			//draw_print_str(0, 6, 16, 0, 3, "28");
			//draw_print_str(54, 6, 11, 0, 3, "Mo");
			//draw_print_str(0,  63, 11, 0, 3, "08");
			//draw_print_str(54, 63,  16, 0, 3, "Fb"); //Ja Fb Mr Ap My Jn Jl Au Se Oc No De
			draw_circle2(31, 31, 31, 1); // outside
			clock_date(date);
			clock_day(day);
			clock_month(month);
			clock_minute_hand(minutes); 
			clock_hour_hand(hours, minutes); 
			#ifdef HAVE_DS1631
				clock_temperature(temp);
			#endif	
			clock_number_ticks();
			clock_second_hand(seconds);
			draw_paint(); 
			
			#ifdef HAVE_DS1631
				if (seconds % 5 == 0) {
					ds1631_convert_temp(DS1631_ADDR);	
					temp_requested = 1;	
					serial_print_str("Req temp\n");				
				}
			#endif	
		}

	}	
			 
} 
	  // void draw_print_str(uns8 x, uns8 y, uns8 width, uns8 start_pixel, uns8 colour, char *str)
#define TEST_IT 3

#if TEST_IT == 1 | TEST_IT == 2
	#warning "test_it = X!"
#endif	


/* 

test routines


// General Run
 
	if(1) {for(;;){ 
		check_time();

		draw_clear_screen();
		draw_print_str(0, 6, 16, 0, 3, "28");
		draw_print_str(54, 6, 11, 0, 3, "Mo");
		draw_print_str(0,  63, 11, 0, 3, "08");
		draw_print_str(54, 63,  16, 0, 3, "Fb"); //Ja Fb Mr Ap My Jn Jl Au Se Oc No De
		draw_circle2(31, 31, 31, 1); // outside
		//draw_circle2(31, 31, 29, 2); seconds
		//draw_circle2(31, 31, 27, 3);	// minutes
		//draw_circle2(31, 31, 18, 2);	//hour
		draw_minute_hand(minutes); 
		draw_hour_hand(hours, minutes); 
		draw_number_ticks();
		draw_second_hand(seconds);
		draw_paint(); 
		delay_ms(100);	 
	} 
}
	for (;;) {
		draw_clear_screen();
		draw_circle2(31, 31, 31, 1); 	// outside
		//draw_circle2(31, 31, 29, 3);	// seconds
		//draw_circle2(31, 31, 27, 2);	// minutes
		//draw_circle2(31, 31, 19, 2);	// hour		
		//draw_number_ticks();
		draw_paint(); 
		
		delay_ms(50);
		
		draw_clear_screen();
		draw_circle2(31, 31, 31, 1); // outside
		//draw_circle2(31, 31, 29, 3); // seconds
		//draw_circle2(31, 31, 27, 2);	// minutes
		//draw_circle2(31, 31, 19, 2);	//hour

		draw_test_hands();
		draw_number_ticks();		
		draw_paint();
		delay_ms(50);
		
*/