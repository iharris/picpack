#include "pcd8544.h"
#include "pic_serial.h"

void pcd8544_setup_io() {

	// Do this in software for the moment
	
	make_output(pcd8544_res_port,  pcd8544_res_pin);
	make_output(pcd8544_sclk_port, pcd8544_sclk_pin);
	make_output(pcd8544_sdin_port, pcd8544_sdin_pin);
	make_output(pcd8544_dc_port,   pcd8544_dc_pin);
	make_output(pcd8544_sce_port,  pcd8544_sce_pin);
	
	set_pin(pcd8544_res_port, pcd8544_res_pin);		// not in reset
	set_pin(pcd8544_sce_port,  pcd8544_sce_pin);	// ignore clock for the moment
	clear_pin(pcd8544_sclk_port, pcd8544_sclk_pin);	// clear clock
}

void pcd8544_init() {
	
	/*
	set_pin  (pcd8544_res_port, pcd8544_res_pin);		
	set_pin(pcd8544_sce_port,  pcd8544_sce_pin);
	*/
	
	// toggle reset
	// must be at least 100ns
	clear_pin(pcd8544_res_port, pcd8544_res_pin);		// pulse low for reset
	delay_ms(100);
	set_pin  (pcd8544_res_port, pcd8544_res_pin);		
	
	// Function set: PD=0 (chip active), V=1 (vertical addressing), H=1 (extended instructions)
	pcd8544_send_command(0b00100011);	// 0x21
	
	// Set Vop (contrast) to 72 - 7 least significant bits
	pcd8544_send_command(0b11001000);	// 0xc8
	
	// Set temperature coefficient to 2 - 2 least significant bits
	pcd8544_send_command(0b00000110);	// 0x06
	
	// Set bias mode to 3 (1:48) - 3 least significant bits
	pcd8544_send_command(0b00010011);	// 0x13

	// Function set: PD=0 (chip active), V=1 (vertical addressing), H=0 (basic instructions)
	pcd8544_send_command(0b00100010);	// 0x20

	// Display control: D=0, E=0 (Blank the screen)
	pcd8544_send_command(0b00001000);	// 0x08
	
	// Display control: D=1, E=0 (Normal mode)
	pcd8544_send_command(0b00001100);

}	

void pcd8544_send_command(uns8 cmd) {
	
	clear_pin(pcd8544_sce_port, pcd8544_sce_pin);
	clear_pin(pcd8544_dc_port, pcd8544_dc_pin);
	pcd8544_send_byte(cmd);
	set_pin(pcd8544_sce_port, pcd8544_sce_pin);
}

void pcd8544_send_data(uns8 data) {
	
	clear_pin(pcd8544_sce_port, pcd8544_sce_pin);
	set_pin(pcd8544_dc_port, pcd8544_dc_pin);
	pcd8544_send_byte(data);
	set_pin(pcd8544_sce_port, pcd8544_sce_pin);
}

void pcd8544_send_byte(uns8 b) {

uns8 count;

	// msb sent first
	for (count = 0; count < 8; count++) {
		change_pin(pcd8544_sdin_port, pcd8544_sdin_pin, b.7);
		b = b << 1;
		// toggle the clock
		set_pin(pcd8544_sclk_port, pcd8544_sclk_pin);
		clear_pin(pcd8544_sclk_port, pcd8544_sclk_pin);
	}	
}
