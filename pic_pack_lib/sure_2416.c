#include "sure_2416.h"
#include "pic_serial.h"

void sure_2416_setup() {
	
	//serial_print_str("Setting up 2416\n");
	
	make_output(sure_2416_cs1_port, sure_2416_cs1_pin);
	make_output(sure_2416_data_port, sure_2416_data_pin);
	make_output(sure_2416_wr_port, sure_2416_wr_pin);
	make_output(sure_2416_rd_port, sure_2416_rd_pin);

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