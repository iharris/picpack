#include "sure_2416.h"
#include "pic_serial.h"

void sure_2416_setup() {
	
	//serial_print_str("Setting up 2416\n");
	
	make_output(sure_2416_cs1_port, sure_2416_cs1_pin);
	make_output(sure_2416_data_port, sure_2416_data_pin);
	make_output(sure_2416_wr_port, sure_2416_wr_pin);
	make_output(sure_2416_rd_port, sure_2416_rd_pin);

}

void sure_2416_init() {
	
	sure_2416_send_command(SURE_2416_CMD_SYS_DISABLE);
	sure_2416_send_command(SURE_2416_CMD_PMOS_16_COMMON);	// Correct hardware layout for the board
	
	sure_2416_send_command(SURE_2416_CMD_CLK_MASTER_MODE);	// We are the master
	sure_2416_send_command(SURE_2416_CMD_SYS_ENABLE); 
	sure_2416_send_command(0b00000011); //led on
}

void sure_2416_send_command(uns8 command) {

uns8 count;
	
	serial_print_str("A");
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

	delay_ms(1);

	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	delay_ms(1);

	set_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	delay_ms(1);
	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	

	// send  command
	// send 1
	serial_print_str("A");
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	delay_ms(1);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
	
	serial_print_str("A");
	// send 0
	clear_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	delay_ms(1);
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);

	serial_print_str("A");
	// send 0
	clear_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	delay_ms(1);
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);

	serial_print_str("B");
	// command bits 7 - 0
	for(count = 0 ; count < 8 ; count++) {
		serial_print_str("p = ");
		if (test_bit(command, 7)) {
			serial_putc('1');
			set_pin(sure_2416_data_port, sure_2416_data_pin);
		} else { serial_putc('0');
			clear_pin(sure_2416_data_port, sure_2416_data_pin);
		}
		serial_print_nl();	
		//change_pin_var(sure_2416_data_port, sure_2416_data_pin, test_bit(command, 7));
	delay_ms(1);
		// pulse wr
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
		// shift mem addr along
		command = command << 1;
	}
		serial_print_str("C");

	// the don't care pulse
	
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);

	// reset CS
	serial_print_str("D");
	delay_ms(1);

	set_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	delay_ms(1);
	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
}
	
void sure_2416_write(uns8 mem_addr, uns8 data) {

uns8 count;
	
	serial_print_str("A");
	
	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);

	// send WR command
	// send 1
	serial_print_str("A");
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	delay_ms(1);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
	
	serial_print_str("A");
	// send 0
	clear_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	delay_ms(1);
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);

	serial_print_str("A");
	// send 1
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	delay_ms(1);
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);

	serial_print_str("B");
	// write mem addr, bits 6 -> 0
	for(count = 0 ; count < 7 ; count++) {
		change_pin_var(sure_2416_data_port, sure_2416_data_pin, test_bit(mem_addr, 6));
	delay_ms(1);
		// pulse wr
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
		// shift mem addr along
		mem_addr = mem_addr << 1;
	}
		serial_print_str("C");

	// write data, bits 0 -> 3 (different from mem addr format)
	for(count = 0 ; count < 4 ; count++) {
		change_pin_var(sure_2416_data_port, sure_2416_data_pin, test_bit(data, 0));
	delay_ms(1);
		// pulse wr
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
		// shift mem addr along
		data = data >> 1;
	}
	// reset CS
	serial_print_str("D");
	delay_ms(1);

	set_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	delay_ms(1);
	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	
	
}


// could do this as a macro ?

void sure_2416_set_brightness(uns8 brightness) {
	// allows level 0 - 15
	sure_2416_send_command(0b10100000 || (brightness && 0x0f));
}	

void sure_2416_set_pixel(uns8 x, uns8 y, uns8 colour) {

uns8 common, panel, led_in_panel, inverted_x, out, mem_addr, bit_in_mem_addr, count, data;
	// first calculate memory address
	
	// y location is top left based
	common = 15 - y;
	
	// x location is kind of messed up
	panel = x / 8;	// which panel of the three is it that we need to change?
	led_in_panel = x - (panel * 8); 
	inverted_x = 7 - led_in_panel;
	out = panel * 8 + inverted_x;
	
	mem_addr = out * 4 + common / 4;
	bit_in_mem_addr = common && 0b00000011;
	
	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	set_pin  (sure_2416_rd_port, sure_2416_rd_pin);

	// send WR command
	// send 1
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	delay_ms(1);
	// pulse wr
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
	
	// send 0
	clear_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	delay_ms(1);
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);

	// send 1
	set_pin  (sure_2416_data_port, sure_2416_data_pin);
	// pulse wr
	delay_ms(1);
	clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
	set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);

	// write mem addr, bits 6 -> 0
	for(count = 0 ; count < 7 ; count++) {
		change_pin_var(sure_2416_data_port, sure_2416_data_pin, test_bit(mem_addr, 6));
	delay_ms(1);
		// pulse rd
		clear_pin(sure_2416_rd_port, sure_2416_rd_pin);
	delay_ms(1);
		set_pin  (sure_2416_rd_port, sure_2416_rd_pin);
	delay_ms(1);
		// shift mem addr along
		mem_addr = mem_addr << 1;
	}

	// Read data, bits 0 -> 3 (different from mem addr format)
	for(count = 0 ; count < 4 ; count++) {
		change_pin_var(sure_2416_data_port, sure_2416_data_pin, test_bit(data, 0));
		delay_ms(1);
		// pulse wr
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
	delay_ms(1);
		// shift mem addr along
		data = data >> 1;
	}

	// Retrieve 4 bits
	// read clocked out on falling edge of RD
	
	make_input(sure_2416_data_port, sure_2416_data_pin);

	for(count = 0 ; count < 4 ; count++) {
		delay_ms(1);
		// pulse wr
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
		delay_ms(1);

		data = data << 1;
		data.0 = test_pin(sure_2416_data_port, sure_2416_data_pin);

		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
		delay_ms(1);
		// shift mem addr along
		mem_addr = mem_addr << 1;
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
		change_pin_var(sure_2416_data_port, sure_2416_data_pin, test_bit(data, 0));
		delay_ms(1);
		// pulse wr
		clear_pin(sure_2416_wr_port, sure_2416_wr_pin);
		delay_ms(1);
		set_pin  (sure_2416_wr_port, sure_2416_wr_pin);
		delay_ms(1);
		// shift mem addr along
		data = data >> 1;
	}
	// reset CS
	serial_print_str("D");
	delay_ms(1);

	set_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	delay_ms(1);
	clear_pin(sure_2416_cs1_port, sure_2416_cs1_pin);
	
	
}
	
uns8 sure_2416_get_pixel(uns8 x, uns8 y) {
}

void sure_2416_clear_screen() {
}

