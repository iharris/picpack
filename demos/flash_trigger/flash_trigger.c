/*
 
 flash_trigger.c
 -----------------
 
 Ian Harris 2009
 imharris [at] gmail.com

 Released under the "do whatever you like with this
 but if if it breaks, you get to keep both pieces" license.	

 Using the pic_mt board and microphone board from sparkfun,
 trigger a flash when sound exceeding a specified level is detected.


*/

// defines pulled from Olimex example code

#define	COL1		test_pin(PORTB, 0)	
#define	COL2		test_pin(PORTB, 1)	
#define	COL3		test_pin(PORTB, 2)	

#define	OFF		0
#define	ON		1

#define BUTTON_NONE			0
#define BUTTON1_ON			1
#define BUTTON1_OFF			2
#define BUTTON2_ON			3
#define BUTTON2_OFF			4
#define BUTTON3_ON			5
#define BUTTON3_OFF			6
#define BUTTON4_ON			7
#define BUTTON4_OFF			8
#define BUTTON5_ON			9
#define BUTTON5_OFF			10
#define BUTTON6_ON			11
#define BUTTON6_OFF			12

#define	BUZZ1		portc.4			
#define	BUZZ2		portc.5

// System includes

#include <system.h>
#include <string.h>
#include <stdlib.h>
#include <eeprom.h>

// Local includes

#include "config.h"

#include "pic_timer.h"
#include "pic_serial.h"
#include "pic_tick.h"
#include "pic_utils.h"
#include "lcd.h"


// Global variables

// Delay 10ms default
uns16 delay = 10;

// Store button state
uns8 button_1, button_2, button_3, button_4, button_5, button_6;

// trigger when sound detected more than this value from "zero"
// keeping in mind that zero is 512 since the DC level of the waveform
// is half way between 0 and 1023 (no negative values here)
uns16 trigger_value = 5;

// read_buffer for serial commands
char read_buffer[6]= '\0';
uns8 read_buffer_pos = 0;


// do we have a task to do?
uns8 do_task = 0;
uns8 pkts;

void beep(uns8 pitch, uns8 length) {

	while(--length) {
		BUZZ2 = OFF;
		BUZZ1 = ON;					
		delay_us(pitch);
		BUZZ1 = OFF;
		BUZZ2 = ON;
	 	delay_us(pitch);
	}
	

void beep1(void) {

	beep(125, 150);
}

void beep2(void) {

	beep(250, 75);
}

void beep3(void) {

	beep(80, 75);

}

void print_armed() {
	lcd_write_command(LCD_LINE2);
	lcd_write_data_str("<ARMED>");
}

uns8 button_scan() {

	// pulled straight from the Olimex example code

	//set COL1 ---------------------------------------------
	portb |= 0x07;
	trisb |= 0x07;
	clear_bit(trisb, 0);	 						
	delay_ms(2);
	
	//button 1 is pressed		
	if((COL2==1)&&(B1==OFF)) { B1=ON; return BUTTON1_ON; }
	if((COL2==0)&&(B1==ON))  { B1=OFF; return BUTTON1_OFF; }
	
	//button 5 is pressed
	if((COL3==1)&&(B5==OFF)) { B5=ON; return BUTTON5_ON; }	
	if((COL3==0)&&(B5==ON))  { B5=OFF; return BUTTON5_OFF; }
	
	//set COL1 ---------------------------------------------
	portb |= 0x07;
	trisb |= 0x07;
	clear_bit(trisb,1);								
	delay_ms(2);
	
	//button 4 is pressed
	if((COL1==1)&&(B4==OFF)) { B4=ON; return BUTTON4_ON; }
	if((COL1==0)&&(B4==ON))  { B4=OFF; return BUTTON4_OFF; }			
	
	//button 3 is pressed
	if((COL3==1)&&(B3==OFF)) { B3=ON; return BUTTON3_ON; }
	if((COL3==0)&&(B3==ON))  { B3=OFF; return BUTTON3_OFF; }
	
	//set COL1 ---------------------------------------------
	portb |= 0x07;
	trisb |= 0x03;
	clear_bit(trisb, 2);							
	delay_ms(2);
	
	//button 2 is pressed
	if((COL1==1)&&(B2==OFF)) { B2=ON; return BUTTON2_ON; }
	if ((COL1==0)&&(B2==ON)) { B2=OFF; return BUTTON2_OFF; }
	
	//button 6 is pressed
	if((COL2==1)&&(B6==OFF)) { B6=ON; return BUTTON6_ON; }
	if((COL2==0)&&(B6==ON))  { B6=OFF; return BUTTON6_OFF; }				

	return BUTTON_NONE;	
}




void timer_0_callback() {
	handle_tick();
}	


void interrupt( void ) {

	timer_handle_0_isr();

	serial_handle_tx_isr();
	serial_handle_rx_isr();
	
}



void process_rx() {
	uns8 len, rec;
	uns8 first_char;

	rec = serial_getc();
	
	serial_putc(rec);

	//Neat trick to allow one button downloads
	if (rec == MAGIC_BOOSTBLOADER_REQUEST) {
		boostbloader();
	}	
	
	if (rec == '\r') {
		first_char = read_buffer[0];
		switch (first_char) {
			case 'r':
				do_task = 1;
				break; 
			default:
				break;
		}		
		read_buffer[0] = '\0';
		read_buffer_pos = 0;
        serial_print_nl();
        serial_putc('>');			
	} else {
		read_buffer[read_buffer_pos] = rec;
		read_buffer_pos++;
		read_buffer[read_buffer_pos] = '\0';
	}	

}


void configure_system() {
	    
	button_1 = button_2 = button_3 = button_4 = button_5 = button_6 = 0;

	kill_interrupts();

    pir1 = 0;	// clear peripheral flag registers
    pir2 = 0;

	adcon1 = 14;	// set an0 for analog input (pic18)
	
	// Set up analog input 
	clear_bit(adcon2, ADCS2);
	set_bit(adcon2, ADCS1);
	clear_bit(adcon2, ADCS0);
	
	set_bit(adcon2, ADFM);
	set_bit(adcon0, ADON);	
	
	make_output(PORTB, 4);	// turn on buzzer ability 
	
	delay_ms(250);	// Let everything settle
	delay_ms(250);	// Let everything settle
	delay_ms(250);	// Let everything settle
	delay_ms(250);	// Let everything settle
	delay_ms(250);	// Let everything settle

	#ifdef _PIC16
//		timer_setup_0(TIMER_8BIT_MODE, TIMER_PRESCALER_1_TO_8, 0xff - 156 - 1);	// 0.25ms at 20Mhz
	#endif

//oring	#ifdef _PIC18
//		timer_setup_0(TIMER_16BIT_MODE, TIMER_PRESCALER_OFF, 0xffff - 1250 );	// 0.25ms at 20Mhz
//	#endif	 
	#ifdef _PIC18
		timer_setup_0(TIMER_16BIT_MODE, TIMER_PRESCALER_OFF, 0xffff - 5000 );	// 1ms at 20Mhz
	#endif	 


	serial_setup(BRGH_HIGH_SPEED, SPBRG_19200); 	// 1,25 = 19200 // 51 = 2400 // 8mhz 9600 = 0,12

//	set_bit(intcon, INT0IE);	// Enable interrupts on rb0
	// for 16f, use INTIE
//	clear_bit(intcon2, INTEDG0); // falling edge (active low for nrf24l01) 
	
	timer_start_0();


	lcd_setup();
	lcd_init();
	
	make_output(PORTA, 1);
	
	turn_peripheral_ints_on();
	turn_global_ints_on();

}	

void print_delay() {
	lcd_write_command(LCD_LINE1);
	lcd_write_data_str("Del=");
	lcd_write_data_int(delay);
	lcd_write_data_str(" Sen=");
	lcd_write_data_int(trigger_value);
	lcd_write_data_str("  ");
	
}


void main( void )

{

uns8 pkt_rx_status;
uns16 tick, new_tick;
uns16 analog_input;
uns8 triggered;
uns16 biggest, smallest;
uns8 but;
uns8 count_down = 0;

	biggest = 0;
	smallest = 1023;
	
	configure_system();

	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);


	serial_print_str("\n\nPkt demo\n");
	serial_print_str( "\n<");
	serial_print_str(__TIME__);
	serial_print_str(">\n");

	delay_ms(100);
	print_armed();
	tick = tick_get_count();
	print_delay();
	
	while (1) {

	if (serial_rx_avail()) {
		process_rx();
	}	  

	 	but = ButtonScan();
	
		if(but==BUTTON1_ON) {
			serial_print_str("1_ON ");  
			if (delay < 250) { 
				delay = delay + 5;
				print_delay();
				beep2();
			}
		} else if (but == BUTTON1_OFF) {
			serial_print_str("1_OFF ");
		} else if (but == BUTTON2_ON) {
			serial_print_str("2_ON "); 
			if (trigger_value > 0) {
				trigger_value = trigger_value - 1;
				print_delay();
				beep1();
			}	
		} else if(but==BUTTON2_OFF) 	{ serial_print_str("2_OFF ");  
									}	
		else if(but==BUTTON3_ON)  	{ serial_print_str("3_ON ");  }
		else if(but==BUTTON3_OFF) 	{ serial_print_str("3_OFF ");  }
		else if(but==BUTTON4_ON)  	{ serial_print_str("4_ON "); 
										if (trigger_value < 25) {
												trigger_value = trigger_value + 1;
												print_delay();
												beep1();
										}	
									}
		else if(but==BUTTON4_OFF) 	{ serial_print_str("4_OFF ");  }
		else if(but==BUTTON5_ON)  	{ serial_print_str("5_ON ");  }
		else if(but==BUTTON5_OFF) 	{ serial_print_str("5_OFF ");  }
		else if(but==BUTTON6_ON)  	{ serial_print_str("6_ON "); 										
											if (delay > 0) {
												delay= delay - 5;
												print_delay();
												beep2();
										}	
									}
		else if(but==BUTTON6_OFF) 	{ serial_print_str("6_OFF ");  }



	new_tick = tick_get_count();
	
		if (tick_calc_diff(tick, new_tick) > 1000) {
			tick = new_tick;
			if (count_down > 0) {
				beep3();
				lcd_write_command(LCD_LINE2);
				lcd_write_data_int(count_down);
				lcd_write_data_str("     ");
				count_down--;
				if (count_down==0) {
					print_armed();
					beep2();
					delay_ms(100);
					beep2();
					delay_ms(100);
				}	
			}
		}	
		triggered = 0;
		while (!triggered) {
			set_bit(adcon0, DONE);
			while (test_bit(adcon0, DONE)) {
			}
			analog_input = adresh;
			analog_input = analog_input << 8;
			analog_input = analog_input + adresl;
			if (analog_input < smallest) {
				smallest = analog_input;
			}
			if (analog_input > biggest) {
				biggest = analog_input;
			}		
		//	if (analog_input > trigger_value) {
				triggered = 1;
		//	}
		}
	//	clear_pin(PORTB, 4);
	//	delay_ms(100);
	//	set_pin(PORTB, 4);

		if ((count_down == 0) && 
		   ((analog_input > 512 + trigger_value) || (analog_input < 512 - trigger_value))) {
			clear_pin(PORTB, 4);
			delay_ms(delay);
			set_pin(PORTA, 1);
			lcd_write_command(LCD_LINE2);
			lcd_write_data_str(" FLASH ");
			delay_ms(250);
			clear_pin(PORTA, 1);
			count_down = 5;
			tick = tick_get_count();
		} else {
			set_pin(PORTB, 4);
		}	
					
	}
		


}


