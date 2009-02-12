#include "sure_2416.h"
#include "pic_serial.h"

void sure_2416_setup() {
	
	//serial_print_str("Setting up 2416\n");
	
	make_output(sure_2416_cs1_port, sure_2416_cs1_pin);
	make_output(sure_2416_data_port, sure_2416_data_pin);
	make_output(sure_2416_wr_port, sure_2416_wr_pin);
	make_output(sure_2416_rd_port, sure_2416_rd_pin);

	set_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin(sure_2416_rd_port, sure_2416_rd_pin);
	set_pin(sure_2416_cs1_port, sure_2416_cs1_pin);  

}

void sure_2416_init() {
	
	sure_2416_send_command(SURE_2416_CMD_SYS_DISABLE);
	sure_2416_send_command(SURE_2416_CMD_PMOS_16_COMMON);	// Correct hardware layout for the board
	
	sure_2416_send_command(SURE_2416_CMD_CLK_MASTER_MODE);	// We are the master
	sure_2416_send_command(SURE_2416_CMD_SYS_ENABLE); 
	sure_2416_send_command(SURE_2416_CMD_LEDS_ON); //led on
}

void sure_2416_send_command(uns8 command) {

uns8 count;
	
	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	
	// send  command
	// send 1
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	
	// send 0
	clear_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

	// send 0
	clear_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

	// command bits 7 - 0
	for(count = 0 ; count < 8 ; count++) {

		if (test_bit(command, 7)) {
			set_pin(sure_2416_data_port, sure_2416_data_pin);
		} else {
			clear_pin(sure_2416_data_port, sure_2416_data_pin);
		}
		//change_pin_var(sure_2416_data_port, sure_2416_data_pin, test_bit(command, 7));
		// pulse wr
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
		// shift mem addr along
		command = command << 1;
	}

	// the don't care pulse
	
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

	// reset CS??

// don't think we need this
//	set_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
//	delay_ms(1);
//	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);

	set_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
}
	
void sure_2416_write(uns8 mem_addr, uns8 data) {

uns8 count;
	
	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	
	// send WR command

	// send 1
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	
	// send 0
	clear_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

	// send 1
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

	// write mem addr, bits 6 -> 0
	for(count = 0 ; count < 7 ; count++) {
		if (test_bit(mem_addr, 6)) {
			set_pin(sure_2416_data_port, sure_2416_data_pin);
		} else { 
			clear_pin(sure_2416_data_port, sure_2416_data_pin);
		}
	
		//change_pin_var(sure_2416_data_port, sure_2416_data_pin, test_bit(mem_addr, 6));
		// pulse wr
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
		// shift mem addr along
		mem_addr = mem_addr << 1;
	}

	// write data, bits 0 -> 3 (different from mem addr format)
	for(count = 0 ; count < 4 ; count++) {
		change_pin_var(sure_2416_data_port, sure_2416_data_pin, test_bit(data, 0));
		// pulse wr
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
		// shift mem addr along
		data = data >> 1;
	}
	// reset CS

//	set_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
//	delay_ms(1);
//	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	set_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	
	
}


// could do this as a macro ?

void sure_2416_set_brightness(uns8 brightness) {
	// allows level 0 - 15
	sure_2416_send_command(0b10100000 | (brightness & 0b00001111));
}	

void sure_2416_set_pixel(uns8 x, uns8 y, uns8 colour) {

uns8 common, panel, led_in_panel, inverted_x, out, mem_addr, bit_in_mem_addr, count, data;

	// first calculate memory address
	
	// y location on panels is top left based
	
	common = 15 - y;
	
/* Previous calculations:
	panel = x / 8;	// which panel of the three is it that we need to change?
	led_in_panel = x - (panel * 8); 
	inverted_x = 7 - led_in_panel;
	out = panel * 8 + led_in_panel; //inverted_x;
	mem_addr = out * 4 + common / 4;
*/
	bit_in_mem_addr = common & 0b00000011;
	

	mem_addr = x * 4 + common / 4;


	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);

	// send WR command

	// send 1
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	
	// send 0
	clear_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

	// send 1
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

	// write mem addr, bits 6 -> 0
	for(count = 0 ; count < 7 ; count++) {
		if (test_bit(mem_addr, 6)) {
			set_pin(sure_2416_data_port, sure_2416_data_pin);
		} else { 
			clear_pin(sure_2416_data_port, sure_2416_data_pin);
		}

		//change_pin_var(sure_2416_data_port, sure_2416_data_pin, test_bit(mem_addr, 6));
		// pulse rd
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

		// shift mem addr along
		mem_addr = mem_addr << 1;
	}

	// Retrieve 4 bits
	// read clocked out on falling edge of RD
	
	make_input(sure_2416_data_port, sure_2416_data_pin);

	for(count = 0 ; count < 4 ; count++) {
		// pulse rd
		clear_pin(sure_2416_rd_port, sure_2416_rd_pin);

		data = data >> 1;
		data.3 = test_pin(sure_2416_data_port, sure_2416_data_pin);

		set_pin  (sure_2416_rd_port, sure_2416_rd_pin);

	}

	make_output(sure_2416_data_port, sure_2416_data_pin);

	// now we have the data, we need to change the bit
	if (colour) {
		set_bit(data, bit_in_mem_addr);
	} else {
		clear_bit(data, bit_in_mem_addr);
	}	
	
	// Now write it back out again
	
	// write data, bits 0 -> 3 (different from mem addr format)
	for(count = 0 ; count < 4 ; count++) {
		//change_pin_var(sure_2416_data_port, sure_2416_data_pin, test_bit(data, 0));
		if (test_bit(data, 0)) {
			set_pin(sure_2416_data_port, sure_2416_data_pin);
		} else { 
			clear_pin(sure_2416_data_port, sure_2416_data_pin);
		}

		// pulse wr
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
		// shift data along
		data = data >> 1;
	}
	
	
	// reset CS
	// don't think this is necessary
//	set_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
//	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	set_pin  (sure_2416_cs1_port, sure_2416_cs1_pin);
	
	
}
	
void sure_2416_fill(uns8 colour) {
	uns8 mem_address;
	uns8 fill;
	
	if (colour) { 
		fill = 0b00001111;
	} else {
		fill = 0b00000000;
	}
		
	for(mem_address = 0 ; mem_address < 96 ; mem_address++) {
		sure_2416_write(mem_address, fill);
	}
}		

void sure_2416_fill2(uns8 colour) {

uns16 count;
	sure_2416_send_command(SURE_2416_CMD_LEDS_OFF);
	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	
	// send WR command

	// send 1
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	
	// send 0
	clear_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

	// send 1
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

	// send mem address of zero
	clear_pin(sure_2416_data_port, sure_2416_data_pin);
	
	// write mem addr, bits 6 -> 0
	for(count = 0 ; count < 7 ; count++) {
			
		//change_pin_var(sure_2416_data_port, sure_2416_data_pin, test_bit(mem_addr, 6));
		// pulse wr
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
		// shift mem addr along

	}
	if (colour) {
		set_pin(sure_2416_data_port, sure_2416_data_pin);
	} else {
		clear_pin(sure_2416_data_port, sure_2416_data_pin);
	}
	// we need to toggle 384 times
	
	for(count = 0 ; count < 384 ; count++) {
		// pulse wr
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
		// shift mem addr along

	}
	// reset CS

	set_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	sure_2416_send_command(SURE_2416_CMD_LEDS_ON);

	
}
