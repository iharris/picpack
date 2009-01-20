/*! \file pic_rf_2401a.c
    \brief Pic Nordic nrf2401a routines

	Nordic nrf2401a routines

	Ian Harris 2008
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if you use it send me an email" license. Oh, and
	if it breaks, you get to keep both pieces.
    
   
*/

#include "pic_rf_2401a.h"
#include "pic_serial.h"
#include <stdlib.h>

void pic_rf_send_byte(uns8 b)
{
uns8 bit_counter;
	for(bit_counter = 0 ; bit_counter < 8 ; bit_counter++) {
		change_pin(rf_data_port, rf_data_pin, b.7);	// Put data on data pin
        set_pin(rf_clk1_port, rf_clk1_pin);			// clock it in (positive edge)
        clear_pin(rf_clk1_port, rf_clk1_pin);		// ready for next bit
            
        b <<= 1;	// Move all the bits left
	}	// repeat until finished
	
}	// pic_rf_send_byte

void pic_rf_send_bytes(char *bytes, uns8 num_bytes) {

uns8 byte_counter;
	for(byte_counter = 0 ; byte_counter < num_bytes ; byte_counter++) {
		pic_rf_send_byte(bytes[byte_counter]);
	}
}		
	
void pic_rf_quick_init(char *my_config, uns8 my_channel, bit my_receive_on) {

uns8 byte_counter;
	make_output(rf_data_port, rf_data_pin); // make data pin output
    clear_pin(rf_clk1_port, rf_clk1_pin);		// make sure it's 0

    pic_rf_chip_enable(0);
    pic_rf_chip_select(1);	// config mode

	for(byte_counter = 0 ; byte_counter < 15 ; byte_counter++) {
		pic_rf_send_byte(my_config[byte_counter]);
	}
	rf_current_channel = my_channel;
	rf_current_mode_receive = my_receive_on;

    pic_rf_chip_select(0);	// config mode ended
    pic_rf_chip_enable(1);	// on the air!

}	
	

void pic_rf_init(rf_config *my_config)
{
uns8 temp;
uns8 options;

	make_output(rf_data_port, rf_data_pin); // make data pin output
    clear_pin(rf_clk1_port, rf_clk1_pin);		// make sure it's 0

    pic_rf_chip_enable(0);
    pic_rf_chip_select(1);	// config mode
	
	pic_rf_send_byte(my_config->payload_width_ch2);
	pic_rf_send_byte(my_config->payload_width_ch1);
	pic_rf_send_bytes(my_config->address_ch2, 5);
	pic_rf_send_bytes(my_config->address_ch1, 5);
	
	options = my_config->options;

	temp = my_config->address_width << 2;
	temp.1 = options.OP_LONG_CRC; // |= my_config->long_crc << 1;
	temp.0 = options.OP_ENABLE_CRC; // |= my_config->enable_crc;

	pic_rf_send_byte(temp);

	temp = options & 0b11100000;  //pull off top three bits
	//temp.7 = options.ENABLE_CH2; // |= my_config->enable_ch2 << 7;
	//temp.6 = options.ENABLE_SHOCKBURST; // |= my_config->enable_shockburst << 6;
	//temp.5 = options.ENABLE_1_MBPS; //my_config->enable_1_mbps << 5;
	temp |= (my_config->crystal & 0b00000111) << 2; // bits 4,3,2 - mask 3 bit range
	temp |= (my_config->output_power & 0b00000011); // bits 1,0 - mask 2 bit range
	
	pic_rf_send_byte(temp);
    	
	temp = my_config->channel << 1;
	rf_current_channel = my_config->channel;
	
	temp |= options.OP_ENABLE_RECEIVE; // my_config->enable_receive;
	rf_current_mode_receive = options.OP_ENABLE_RECEIVE;
	
	pic_rf_send_byte(temp);
	
    pic_rf_chip_select(0);	// config mode ended
    pic_rf_chip_enable(1);	// on the air!
    
  }
	
	
void pic_rf_receive(uns8* data, uns8 bytes_to_receive) {
uns8 byte_count, bit_count, temp;

	//!!pic_rf_chip_enable(0);	// save power
	bit  my_store_gie = intcon.GIE;        
    kill_interrupts();

	make_input(rf_data_port, rf_data_pin); // make data pin input
	
    for (byte_count = 0; byte_count < bytes_to_receive; byte_count++) {
    	
		for (bit_count = 0; bit_count < 8; bit_count++) {
    	    temp <<= 1;
            temp.0 = test_pin(rf_data_port, rf_data_pin);
			set_pin(rf_clk1_port, rf_clk1_pin);			// clock it out 
			clear_pin(rf_clk1_port, rf_clk1_pin);		// ready for next bit
 		}
 		data[byte_count] =  temp;
 	}		

	//!pic_rf_chip_enable(1);	// turn chip back on
	intcon.GIE = my_store_gie;
}	

void pic_rf_transmit(char* data, uns8 bytes_to_transmit) {

uns8 byte_count, bit_count, temp;
    
	bit  my_store_gie = intcon.GIE;        
    kill_interrupts();
    
	if (rf_current_mode_receive) {
		pic_rf_set_mode(TRANSMIT_MODE);
	}	

	make_output(rf_data_port, rf_data_pin); // make data pin output
	
    pic_rf_send_bytes_inline(data, bytes_to_transmit);	
    
	pic_rf_chip_enable(0);	// On low, enable shockburst

	delay_ms(1);

 	pic_rf_set_mode(RECEIVE_MODE);	// Go back to receive mode

    intcon.GIE = my_store_gie;
}	

void pic_rf_set_mode(uns8 mode)	// zero for transmit, 1 for receive 
{
	bit  my_store_gie = intcon.GIE;        
    kill_interrupts();

	make_output(rf_data_port, rf_data_pin); // make data pin output
    pic_rf_chip_enable(0);
    pic_rf_chip_select(1);	// config mode
	
	change_pin(rf_data_port, rf_data_pin, mode);	// send a zero
    set_pin(rf_clk1_port, rf_clk1_pin);			// clock it in (positive edge)
    clear_pin(rf_clk1_port, rf_clk1_pin);		// ready for next bit
	
    pic_rf_chip_select(0);	// config mode ended
    pic_rf_chip_enable(1);
	
	rf_current_mode_receive = mode;

    intcon.GIE = my_store_gie;
}

void pic_rf_set_channel(uns8 channel)
{
	bit  my_store_gie = intcon.GIE;        
    kill_interrupts();

	clear_bit(tris_array[rf_data_port - PORTA], rf_data_pin); // make data pin output

    pic_rf_chip_enable(0);
    pic_rf_chip_select(1);	// config mode
	
	rf_current_channel = channel;
	channel <<= 1;
	channel |= rf_current_mode_receive;
	
	pic_rf_send_byte(channel);	
	
    pic_rf_chip_select(0);	// config mode ended
    pic_rf_chip_enable(1);

    intcon.GIE = my_store_gie;
}

void pic_rf_setup() {

	make_output(rf_data_port, rf_data_pin); // make data pin output
	make_output(rf_cs_port, rf_cs_pin); // make cs pin output
	make_output(rf_ce_port, rf_ce_pin); // make ce pin output
	make_input (rf_dr1_port, rf_dr1_pin); // make dr1 pin input
	make_output(rf_clk1_port, rf_clk1_pin); // make clk1 pin output
		
}


		