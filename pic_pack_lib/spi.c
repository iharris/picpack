/*
 
 spi.c

 Single pin interface routines

 Ian Harris 2008
 imharris [at] gmail.com

 Released under the "do whatever you like with this
 but if you use it send me an email" license. Oh, and
 if it breaks, you get to keep both pieces.

*/

#include "spi.h"

void spi_write(uns8 data) {

uns8 count;
	for (count = 0; count < 8; count++) {
		clear_pin(spi_clk_port, spi_clk_pin);	// set to low
		change_pin(spi_data_port, spi_data_pin, data.7);
		data = data << 1;
		set_pin(spi_clk_port, spi_clk_pin);	// set to low
	}	
		
}



void spi_write_lsb(uns8 data) {

uns8 count;


	for (count = 0; count < 8; count++) {
		clear_pin(spi_clk_port, spi_clk_pin);	// set to low
		change_pin(spi_data_port, spi_data_pin, data.0);
		data = data >> 1;
		set_pin(spi_clk_port, spi_clk_pin);	// set to low
	}	
		
}
	
void spi_setup() {
	clear_bit(tris_array[spi_clk_port - PORTA], spi_clk_pin);   // output
	clear_bit(tris_array[spi_data_port - PORTA], spi_data_pin);   // output

}	

void spi_write_sure(uns8 data) {

uns8 count;
uns8 data_in;

	data_in = data;
	for (count = 0; count < 8; count++) {
		change_pin(spi_data_port, spi_data_pin, data_in.0);
		clear_pin(spi_clk_port, spi_clk_pin);	// set to low
		set_pin(spi_clk_port, spi_clk_pin);	// set to low
		data_in = data_in >> 1;
	}	
}


void spi_pulse_0() {
		change_pin(spi_data_port, spi_data_pin, 0);
		clear_pin(spi_clk_port, spi_clk_pin);	// set to low
		set_pin(spi_clk_port, spi_clk_pin);	// set to low
}

void spi_pulse_1() {
		change_pin(spi_data_port, spi_data_pin, 1);
		clear_pin(spi_clk_port, spi_clk_pin);	// set to low
		set_pin(spi_clk_port, spi_clk_pin);	// set to low
}