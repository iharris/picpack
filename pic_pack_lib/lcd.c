//-----------------------------------------------------
/// \file pic_serial.c
// 
// lcd.c
//
/// LCD communicaiton routines
//
/// Ian Harris 2008
/// imharris [at] gmail.com
//
/// Released under the "do whatever you like with this
/// but if you use it send me an email" license. Oh, and
/// if it breaks, you get to keep both pieces.
//-----------------------------------------------------

#include "lcd.h"
#include <string.h>
#include <stdlib.h>

void lcd_toggle_e() {
	set_pin(lcd_e_port, lcd_e_pin);
	delay_us(50);	// Despite the datasheet, 50us seems to be the right number...
	clear_pin(lcd_e_port, lcd_e_pin);
	delay_us(50);	// Despite the datasheet, 50us seems to be the right number...
}	

void lcd_write_nibble(uns8 data) {
	// There would be a more efficient way to do this if all the pins
	// were on the same port - but we can't guarantee that. At least
	// this way all pins are protected from read before write problems.
	
	change_pin(lcd_db4_port, lcd_db4_pin, data.0);
	change_pin(lcd_db5_port, lcd_db5_pin, data.1);
	change_pin(lcd_db6_port, lcd_db6_pin, data.2);
	change_pin(lcd_db7_port, lcd_db7_pin, data.3);
}	

void lcd_write_byte(uns8 data) {

	lcd_write_nibble(data >> 4);	// Write upper nibble
	lcd_toggle_e();
	lcd_write_nibble(data);	// Write lower nibble
	lcd_toggle_e();
}


void lcd_setup() {
	
	// Set up tris bits
	make_output(lcd_e_port, lcd_e_pin);
	make_output(lcd_rs_port, lcd_rs_pin);
	make_output(lcd_rw_port, lcd_rw_pin);
	make_output(lcd_db7_port, lcd_db7_pin);
	make_output(lcd_db6_port, lcd_db6_pin);
	make_output(lcd_db5_port, lcd_db5_pin);
	make_output(lcd_db4_port, lcd_db4_pin);
	
	clear_pin(lcd_e_port, lcd_e_pin);
	clear_pin(lcd_rs_port, lcd_rs_pin);
	clear_pin(lcd_rw_port, lcd_rw_pin);
}


void lcd_init() {
	
	delay_ms(16);
	lcd_write_nibble(0x03);
	lcd_toggle_e();
	delay_ms(5);
	lcd_write_nibble(0x03);
	lcd_toggle_e();
	delay_ms(1);
	lcd_write_nibble(0x03);
	lcd_toggle_e();
	lcd_write_nibble(0x02); //0b 0010 1000
	lcd_toggle_e();
	
	// Now we are in 4bit mode

	lcd_write_command(0b00101000); // 0x28 numlines=1 font=0
	
	lcd_write_command(0b00001000); // disp off, curs off blink off
	
	lcd_write_command(0b00000110); // cursor move, inc, no shift

	lcd_write_command(0b00001100); // disp on, curs on blink on

	lcd_write_command(LCD_CLEAR_DISP);
	lcd_write_command(LCD_RETURN_HOME);

}

void lcd_write_command(uns8 data) {

	lcd_wait_busy();	

	clear_pin(lcd_rs_port, lcd_rs_pin);
	clear_pin(lcd_rw_port, lcd_rw_pin);

	lcd_write_byte(data);
}

void lcd_write_data(uns8 data) {
	lcd_wait_busy();	

	set_pin(lcd_rs_port, lcd_rs_pin);
	clear_pin(lcd_rw_port, lcd_rw_pin);
	
	lcd_write_byte(data);
}

void lcd_write_data_str(char *str) {

	lcd_wait_busy();	

	set_pin(lcd_rs_port, lcd_rs_pin);
	clear_pin(lcd_rw_port, lcd_rw_pin);

	
	while (*str) {
		lcd_write_byte(*str++);
	}	
}		

void lcd_write_data_int(uns16 i) {

char buffer[6];

	itoa( i, buffer, 10 );
	lcd_write_data_str(buffer);
}


void lcd_wait_busy() {

		
	set_bit(tris_array[lcd_db7_port - PORTA], lcd_db7_pin); // db7 input
	set_bit(tris_array[lcd_db6_port - PORTA], lcd_db6_pin); // db6 input
	set_bit(tris_array[lcd_db5_port - PORTA], lcd_db5_pin); // db5 input
	set_bit(tris_array[lcd_db4_port - PORTA], lcd_db4_pin); // db4 input


	clear_pin(lcd_rs_port, lcd_rs_pin);
	set_pin(lcd_rw_port, lcd_rw_pin);

	char counter = 0;
	
	set_pin(lcd_e_port, lcd_e_pin);
	// Wait for completion of the operation, with a timeout of ~.5 seconds
	// LCD d7 is high if the operation is complete.
	while ((test_pin(lcd_db7_port, lcd_db7_pin) == 1) && counter < 0xF0){
		clear_pin(lcd_e_port, lcd_e_pin);
		set_pin(lcd_e_port, lcd_e_pin);
		delay_us(100);
		clear_pin(lcd_e_port, lcd_e_pin);
		//delay_us(100);
		counter++;
	}


	// Check if the previous loop timed out 	
	if (counter == 0xF0) {
	}
	
	clear_bit(tris_array[lcd_db7_port - PORTA], lcd_db7_pin); // db7 output
	clear_bit(tris_array[lcd_db6_port - PORTA], lcd_db6_pin); // db6 output
	clear_bit(tris_array[lcd_db5_port - PORTA], lcd_db5_pin); // db5 output
	clear_bit(tris_array[lcd_db4_port - PORTA], lcd_db4_pin); // db4 output

	return;
}	

void lcd_set_ddram_pos(uns8 x) {
   x.7 = 1;	// Set MSB bit, which indicates we are setting the RAM address
   lcd_write_command(x);
}	

void lcd_set_cgram_pos(uns8 x) {
   x.7 = 0;	// indicate we are setting cgram
   x.6 = 1;	
   lcd_write_command(x);
}

void lcd_cursor_home() {
	lcd_write_command(LCD_CLEAR_DISP);	
}
