#include "pic_rf_24l01.h"
#include "pic_serial.h"
#include <stdlib.h>

uns8 pic_rf_receive(uns8 *data, uns8 bytes_to_receive) {

uns8 fifo_status;
uns8 res;

	res = 0;
	
	// Could check status here, but we know why it is...
	//clear_pin(rf_ce_port, rf_ce_pin); // Go into standby		

	pic_rf_read_register_inline(RF_RD_REG_FIFO_STATUS, &fifo_status, 1);
    while (!test_bit(fifo_status, 0)) {	// Fifo has something in it 
    	//serial_putc('|');
		pic_rf_read_register_inline(RF_R_RX_PAYLOAD, data, bytes_to_receive);	// receive
		pic_rf_send_command_inline(RF_WR_REG_STATUS, "\x40", 1);
		res++;
		pic_rf_read_register_inline(RF_RD_REG_FIFO_STATUS, &fifo_status, 1);
	}
	// clear rf interrupt
	//pic_rf_send_command_inline (RF_FLUSH_RX,  0, 0 );	
	//set_pin(rf_ce_port, rf_ce_pin); // Back on the air
	return res;
}		
		

uns8 pic_rf_send_command(uns8 cmd, uns8 *data, uns8 data_len) {

uns8 byte_counter, status;

    clear_pin(rf_csn_port, rf_csn_pin);
	status = pic_rf_send_byte(cmd); 
	for(byte_counter = 0 ; byte_counter < data_len ; byte_counter++) {
		pic_rf_send_byte(data[byte_counter]);
	}							 
	
    set_pin(rf_csn_port, rf_csn_pin);
    return status;
}	

uns8 pic_rf_send_command_single(uns8 cmd, uns8 data) {

uns8 byte_counter, status;

    clear_pin(rf_csn_port, rf_csn_pin);
	status = pic_rf_send_byte(cmd);
	pic_rf_send_byte(data);
    set_pin(rf_csn_port, rf_csn_pin);
    return status;
}	


uns8 pic_rf_read_register(uns8 cmd, uns8 *data, uns8 data_len) { // returns status

uns8 byte_counter, status;

    clear_pin(rf_csn_port, rf_csn_pin);
	status = pic_rf_send_byte(cmd); 
	for(byte_counter = 0 ; byte_counter < data_len ; byte_counter++) {
		data[byte_counter] = pic_rf_send_byte(0);
	}							 
	
    set_pin(rf_csn_port, rf_csn_pin);
    return status;
}

uns8 pic_rf_read_register_int(uns8 cmd, uns8 *data, uns8 data_len) { // returns status

uns8 byte_counter, status;

    clear_pin(rf_csn_port, rf_csn_pin);
	status = pic_rf_send_byte_int(cmd); 
	for(byte_counter = 0 ; byte_counter < data_len ; byte_counter++) {
		data[byte_counter] = pic_rf_send_byte_int(0);
	}							 
	
    set_pin(rf_csn_port, rf_csn_pin);
    return status;
}




uns8 pic_rf_send_byte(uns8 b)
{
uns8 bit_counter, status;
	// - For debug: print_int_hex(b); putc(' ');
	for(bit_counter = 0 ; bit_counter < 8 ; bit_counter++) {
		change_pin(rf_mosi_port, rf_mosi_pin, b.7);	// Put data on data pin
        set_pin(rf_sck_port, rf_sck_pin);			// clock it in (positive edge)
        status <<= 1;
        status.0 = test_pin(rf_miso_port, rf_miso_pin);
        clear_pin(rf_sck_port, rf_sck_pin);		// ready for next bit
            
        b <<= 1;	// Move all the bits left
	}	// repeat until finished
	return status;
}	// pic_rf_send_byte


uns8 pic_rf_send_byte_int(uns8 b)
{
uns8 bit_counter, status;
	// - For debug: print_int_hex(b); putc(' ');
	for(bit_counter = 0 ; bit_counter < 8 ; bit_counter++) {
		change_pin(rf_mosi_port, rf_mosi_pin, b.7);	// Put data on data pin
        set_pin(rf_sck_port, rf_sck_pin);			// clock it in (positive edge)
        status <<= 1;
        status.0 = test_pin(rf_miso_port, rf_miso_pin);
        clear_pin(rf_sck_port, rf_sck_pin);		// ready for next bit
            
        b <<= 1;	// Move all the bits left
	}	// repeat until finished
	return status;
}	// pic_rf_send_byte

	
void pic_rf_quick_init(char *my_config, uns8 my_channel, bit my_receive_on) {


}	
	

/*void pic_rf_init_new(rf_config *my_config)
{
uns8 temp;
uns8 options;
uns8 datum;
uns8 data[5];

	start_crit_sec();
	
	clear_pin(rf_ce_port, rf_ce_pin); // go into standby, so we can configure
	delay_ms(100);

	// Set payload to payload_width_ch1
	
	pic_rf_send_command_single(RF_WR_REG_RX_PW_P0,  // write register 0x11 = RX_PW_P0 receive payload width
						rf_config.payload_width_ch1/8); 	// 7:6 reserved=0 5:0 bytes in payload

	
	if (rf_config.address_width <= 24) {
		datum = 0b00000001;	// 3 bytes
	} else if (rf_config.address_width = 32) {
		datum = 0b00000010;	// 4 bytes
	} else {
		datum = 0b00000011;	// 5 bytes
	}	
		
	pic_rf_send_command_single(RF_WR_REG_SETUP_AW, // write register 0x03 = SETUP_AW Address widths
	                    datum);		// 7-2 reserved=0,  01-3 bytes 10-4 bytes 11-5 bytes
	
	pic_rf_send_command(RF_WR_REG_TX_ADDR,  // write register 0x10 TX_ADDR
			            rf_config.address, rf_config.payload_width_ch1/8);	

	
	//data[0] = 0b00111100;
	datum = 0b00110000;
	if (test_bit(rf_config.options, OP_ENABLE_CRC));
	temp = pic_rf_send_command(RF_WR_REG_CONFIG_REG, // write register 0x00 = CONFIG_REG
	                    "\x3c", 1); // 7-reserved=0,6-MASK_RX_DR=0,5-MASK_TX_DS=1,4-MASK_MAX_RT=1,
								  // 3-EN_CRC=1,2-CRC0=1 (1=2 byte)
								  // 1-PWR_UP=0,0-PRIM_RX=0
	//data[0] = 0b00000000;			           				  
	pic_rf_send_command(RF_WR_REG_SETUP_RETR, // write register 0x04 = SETUP_RETR
	                    "\x00", 1);  	// 7-4 retry delay=0,
										// 3-0 Auto retransmit count=0
	//data[0] =  0b00000001;
	pic_rf_send_command(RF_WR_REG_SETUP_AW, // write register 0x03 = SETUP_AW Address widths
	                    "\x01", 1);		// 7-2 reserved=0, 1-0 *01*-3 bytes 10-4 bytes 11-5 bytes
	
	//data[0] = 0b00000111;
	pic_rf_send_command(RF_WR_REG_RF_SETUP, // write register 6 = RF_SETUP
	                    "\x07", 1);		// 7-5 reserved=0, 4 PLL_LOCK=0, 3 RF_DR=0, 2-1 RF_PWR=11, 0 LNA_HCURR=1
	//data[0] = 0b00000010;
	pic_rf_send_command (RF_WR_REG_RF_CH, 	// write register 0x05 = RF_CH
	                     "\x02", 1);	// 7 reserved=0, 6:0 RF_CH = 2 

	data[0] = 0b11100111;	// 0xe7 LSB
	data[1] = 0b11100111;	// 0xe7
	data[2] = 0b11100111;	// 0xe7 MSB
	pic_rf_send_command(RF_WR_REG_TX_ADDR,  // write register 0x10 TX_ADDR
			            &data, 3);


	pic_rf_send_command(RF_WR_REG_RX_ADDR_P0, // write register 0x0a = RX_ADDR_P0
						&data, 3);

	pic_rf_send_command(RF_WR_REG_EN_AA, // write register 0x01 = EN_AA
					 "\x00", 1);  // 7:6 reserved=0, 5 ENAA_P5=0, 4 ENAA_P4=0, ... 0 ENAA_P0=0
	
	// 0b00111111 
	pic_rf_send_command(RF_WR_REG_CONFIG_REG,  // write register 0x00 = CONFIG_REG
	                    "\x3f", 1); // 7-reserved=0,6-MASK_RX_DR=0,5-MASK_TX_DS=1,4-MASK_MAX_RT=1,
								  // 3-EN_CRC=1,2-CRC0=1 (1=2 byte)
								  // 1-PWR_UP=1,0-PRIM_RX=1
	delay_ms(2);	// 1.5ms settling after power up
	pic_rf_send_command (RF_FLUSH_TX, 	
	                     0, 0	);	
	pic_rf_send_command (RF_FLUSH_RX, 	
	                     0, 0	);	
	delay_ms(2);	// 1.5ms settling after power up

	set_pin(rf_ce_port, rf_ce_pin); // We're on the air (receive)
	
	end_crit_sec();
	serial_print_str("got=");
	serial_print_int(temp);
	serial_print_str(" ");
    
    rf_current_mode_receive = 1;
 }
*/
void pic_rf_init(rf_config *my_config)
{
uns8 temp;
uns8 options;
uns8 data[5];
	
	start_crit_sec();
	
	clear_pin(rf_ce_port, rf_ce_pin); // go into standby, so we can configure
	delay_ms(100);
	//data[0] = 0b00111100;
	temp = pic_rf_send_command(RF_WR_REG_CONFIG_REG, // write register 0x00 = CONFIG_REG
	                    "\x3c", 1); // 7-reserved=0,6-MASK_RX_DR=0,5-MASK_TX_DS=1,4-MASK_MAX_RT=1,
								  // 3-EN_CRC=1,2-CRC0=1 (1=2 byte)
								  // 1-PWR_UP=0,0-PRIM_RX=0
	//data[0] = 0b00000000;			           				  
	pic_rf_send_command(RF_WR_REG_SETUP_RETR, // write register 0x04 = SETUP_RETR
	                    "\x00", 1);  	// 7-4 retry delay=0,
										// 3-0 Auto retransmit count=0
	//data[0] =  0b00000001;
	pic_rf_send_command(RF_WR_REG_SETUP_AW, // write register 0x03 = SETUP_AW Address widths
	                    "\x01", 1);		// 7-2 reserved=0, 1-0 *01*-3 bytes 10-4 bytes 11-5 bytes
	
	//data[0] = 0b00000111;
	pic_rf_send_command(RF_WR_REG_RF_SETUP, // write register 6 = RF_SETUP
	                    "\x07", 1);		// 7-5 reserved=0, 4 PLL_LOCK=0, 3 RF_DR=0, 2-1 RF_PWR=11, 0 LNA_HCURR=1
	//data[0] = 0b00000010;
	pic_rf_send_command (RF_WR_REG_RF_CH, 	// write register 0x05 = RF_CH
	                     "\x02", 1);	// 7 reserved=0, 6:0 RF_CH = 2 

	data[0] = 0b11100111;	// 0xe7 LSB
	data[1] = 0b11100111;	// 0xe7
	data[2] = 0b11100111;	// 0xe7 MSB
	pic_rf_send_command(RF_WR_REG_TX_ADDR,  // write register 0x10 TX_ADDR
			            &data, 3);


	pic_rf_send_command(RF_WR_REG_RX_ADDR_P0, // write register 0x0a = RX_ADDR_P0
						&data, 3);

	pic_rf_send_command(RF_WR_REG_EN_AA, // write register 0x01 = EN_AA
					 "\x00", 1);  // 7:6 reserved=0, 5 ENAA_P5=0, 4 ENAA_P4=0, ... 0 ENAA_P0=0
	
	// Set payload to 21 bytes
	// 0b00010101
	//PKT_PACKET_SIZE
	pic_rf_send_command(RF_WR_REG_RX_PW_P0,  // write register 0x11 = RX_PW_P0 receive payload width
						"\x15", 1); 	// 7:6 reserved=0 5:0 bytes in payload
	// 0b00111111 
	pic_rf_send_command(RF_WR_REG_CONFIG_REG,  // write register 0x00 = CONFIG_REG
	                    "\x3f", 1); // 7-reserved=0,6-MASK_RX_DR=0,5-MASK_TX_DS=1,4-MASK_MAX_RT=1,
								  // 3-EN_CRC=1,2-CRC0=1 (1=2 byte)
								  // 1-PWR_UP=1,0-PRIM_RX=1
	delay_ms(2);	// 1.5ms settling after power up
	pic_rf_send_command (RF_FLUSH_TX, 	
	                     0, 0	);	
	pic_rf_send_command (RF_FLUSH_RX, 	
	                     0, 0	);	
	//clear interrupts
	pic_rf_send_command_inline(RF_WR_REG_STATUS, "\x40", 1);
	delay_ms(2);	// 1.5ms settling after power up

	set_pin(rf_ce_port, rf_ce_pin); // We're on the air (receive)
	
	end_crit_sec();
	serial_print_str("got=");
	serial_print_int_hex(temp);
	serial_print_str(" ");
    
    rf_current_mode_receive = 1;
 }


void pic_rf_transmit(uns8 *data, uns8 bytes_to_transmit) {

uns8 byte_count, bit_count, temp, cd;
    start_crit_sec();
    
	pic_rf_set_mode(TRANSMIT_MODE);
	
	//pic_rf_send_command (RF_FLUSH_TX, 0, 0	);	
	pic_rf_read_register_inline(RF_RD_REG_CD, &cd, 1);
	serial_print_str("\n cd=");
	serial_print_int(cd);
	serial_print_nl();
	pic_rf_send_command(RF_W_TX_PAYLOAD, data, bytes_to_transmit);

	set_pin(rf_ce_port, rf_ce_pin);
	delay_us(10);	// 10us pulse - send packet out on airways
	clear_pin(rf_ce_port, rf_ce_pin);
	delay_us(130);	// TX settling
 	pic_rf_set_mode(RECEIVE_MODE);	// go back to receive mode

	end_crit_sec();
}	
inline void pic_rf_receive_inline(uns8 *data, uns8 bytes_to_receive) {
        pic_rf_read_register_inline(RF_R_RX_PAYLOAD, data, bytes_to_receive);
}

void pic_rf_receive2(uns8 *data, uns8 bytes_to_receive) {
	
	pic_rf_receive_inline(data, bytes_to_receive);
}


void pic_rf_set_mode(uns8 requested_mode)	// zero for transmit, 1 for receive 
{
uns8 config_reg;
	
	start_crit_sec();

	if ((requested_mode == TRANSMIT_MODE) && (rf_current_mode_receive)) {
		// RX -> TX
		// need to lower CE to go into stand by mode
		clear_pin(rf_ce_port, rf_ce_pin); // Go into standby		
		pic_rf_read_register(RF_RD_REG_CONFIG_REG, &config_reg, 1);
		clear_bit(config_reg, CONFIG_PRIM_RX);
		pic_rf_send_command(RF_WR_REG_CONFIG_REG, &config_reg, 1);
		rf_current_mode_receive = 0;
	} else if ((requested_mode == RECEIVE_MODE) && (!rf_current_mode_receive)) {
		// TX -> RX
		// CE should already be low, so we're in standby mode
		pic_rf_read_register(RF_RD_REG_CONFIG_REG, &config_reg, 1);
		set_bit(config_reg, CONFIG_PRIM_RX);
		pic_rf_send_command(RF_WR_REG_CONFIG_REG, &config_reg, 1);
		set_pin(rf_ce_port, rf_ce_pin); // we're on the air! (rx)
		rf_current_mode_receive = 1;
	}
	
	end_crit_sec();
}

void pic_rf_set_channel(uns8 channel)
{
	start_crit_sec();

	clear_pin(rf_ce_port, rf_ce_pin);
	pic_rf_send_command (RF_WR_REG_RF_CH, 	// write register 0x05 = RF_CH
	                     &channel, 1);		// 7 reserved=0, 6:0 RF_CH = channel 
	if (rf_current_mode_receive) {
		set_pin(rf_ce_port, rf_ce_pin); // Go back on the air!
	}
	rf_current_channel = channel;
	
	end_crit_sec();
}

void pic_rf_setup() {

	make_output(rf_ce_port,   rf_ce_pin); 
	make_output(rf_csn_port,  rf_csn_pin);
	make_output(rf_sck_port,  rf_sck_pin); 
	make_output(rf_mosi_port, rf_mosi_pin);
	make_input (rf_miso_port, rf_miso_pin);
	make_input (rf_irq_port,  rf_irq_pin);

    set_pin(rf_csn_port, rf_csn_pin);
	clear_pin(rf_ce_port, rf_ce_pin);
}
	
