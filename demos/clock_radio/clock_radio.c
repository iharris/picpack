//-----------------------------------------------------
// Pic Pack library
// 
// clock_radio.c
//
// It's the clock radio we made ourselves
// 
// 
//
// Ian Harris 2009
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if it breaks, you get to keep both pieces" license
//-----------------------------------------------------

// Pic Pack includes
#include "config.h"
#include "pic_utils.h"
#include "pic_serial.h"
#include "ar1000.h"
#include "lcd.h"
#include "ds1307.h"


// System includes
#include "string.h"
#include "stdlib.h"

// Don't forget your config.h!

// Put received characters in here to see
// if they make a command
char serial_buffer[6]= '\0';

// See if we need to do a task
uns8 do_task = 0;

// Flag if we received something
bit receive = 0;

// Dummy variable to show we can set it 
uns8 send_to = 0;

// Interrupt routine - - - - - - - - - -

void interrupt() {
	
		serial_handle_tx_isr();
		serial_handle_rx_isr();
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

	// neat trick to allow one button downloads
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
		    case 'x':
				do_task = 2;
				break;
			case 'c':
				do_task = 3;
				break;
			case 'i':
				do_task = 4;
				break;
			case 'r':
				do_task = 5;
				break;
			case 'e':
				do_task = 6;
				break;
			case 'q':
				do_task = 7;
				break;
			case 'u':
				do_task = 8;
				break;
			case 'm':
				do_task = 9;
				break;
			default:
				// More complicated example
				// Let's set a variable based on input
				// You can type:
				// s05
				// And press enter, it will set "send_to" to this (in hex)
			//	if ((first_char == 's') && (strlen(serial_buffer)==3)) {
			//		send_to = atoui_hex(&serial_buffer[1]);
			//		serial_print_str(" Set send_to to ");
			//		serial_print_int(send_to);	// Of course, this could be anything
			//		serial_putc('\n');
			//	};
				if ((first_char == 'v') && (strlen(serial_buffer)==3)) {
					send_to = atoui_dec(&serial_buffer[1]);
					serial_print_str("v=");
					serial_print_int(send_to);	// Of course, this could be anything
					serial_putc('\n');
				};
				ar1000_set_volume(send_to);
				
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

void print_reg(uns8 reg, uns16 data) {
	serial_putc('R');
	serial_print_int(reg);
	serial_putc('=');
	serial_print_int_hex_16bit(data);
}	

void print_bit(uns16 reg, uns8 b) {
	if (test_bit(reg, b)) {
		serial_putc('1');
	} else {
		serial_putc('0');
	}
}

inline void print_status(uns16 status) {
			serial_print_str(" ch=");
			uns16 channel = status >> 7;
			serial_print_int(channel+690);
			serial_putc(' ');

			serial_print_str(" RDSDR=");
			print_bit(status, STATUS_RDS_DATA_READY);
			
			serial_print_str(" STC=");
			print_bit(status, STATUS_SEEK_TUNE_COMPLETE);
			
			serial_print_str(" SFAIL=");
			print_bit(status, STATUS_SEEK_FAIL);
			
			serial_print_str(" ST=");
			print_bit(status, STATUS_STEREO);
			
			serial_print_str(" L2=");
			serial_print_int_hex(status &0x03);
			serial_print_nl();
}


void print_registers() {

uns16 reg;

	reg = ar1000_read_register(AR1000_R0);
	print_reg(AR1000_R0, reg);
	serial_print_nl();
	
	reg = ar1000_read_register(AR1000_R1);
	print_reg(AR1000_R1, reg);
    serial_print_str(" RDS_EN=");
    print_bit(reg, R1_RDS_ENABLE);
    serial_print_str(" RDS_I_EN=");
    print_bit(reg, R1_RDS_INT_ENABLE);
    serial_print_str(" STC_I_EN=");
    print_bit(reg, R1_STC_INT_ENABLE);
    serial_print_str(" DEEMP=");
    if (test_bit(reg, R1_DEEMP_SETTING)) {
    	serial_print_str("75us");
    } else {
		serial_print_str("50us");
	}
    serial_print_str(" F_MONO=");
    print_bit(reg, R1_FORCE_MONO);
    serial_print_str(" S_MUTE=");
    print_bit(reg, R1_SOFT_MUTE_ENABLE);
    serial_print_str(" H_MUTE=");
    print_bit(reg, R1_HARD_MUTE_ENABLE);

	reg = reg & 0b0000000111111111;
	serial_print_str(" L9=");
	serial_print_int_hex_16bit(reg);
	serial_print_nl();

	reg = ar1000_read_register(AR1000_R2);
	print_reg(AR1000_R2, reg);
    serial_print_str(" TUNE_EN=");
    print_bit(reg, R2_TUNE_ENABLE);
    uns16 freq = reg;
    freq = freq >> 6;
    freq = freq & 0b0000000111111111;
    freq = freq + 690;
    serial_print_str(" freq=");
    serial_print_int(freq);
    serial_print_str(" L6=");
    reg = reg & 0b0000000000111111;
    serial_print_int_hex_16bit(reg);
    serial_print_nl();
    
	reg = ar1000_read_register(AR1000_R3);
	print_reg(AR1000_R3, reg);
     
    serial_print_str(" S_UP=");
    print_bit(reg, R3_SEEK_UP);
    serial_print_str(" S_EN=");
    print_bit(reg, R3_SEEK_ENABLE);
    serial_print_str(" CH=");
    print_bit(reg, R3_SEEK_CHANNEL_SPACING);
  
    serial_print_str(" BND=");
    if (test_bit(reg, R3_BAND_1)) {
    	serial_print_str("Jap ");
    	if (test_bit(reg, R3_BAND_0)) {
    		serial_putc('W');
    	} else {
			serial_putc('N');
		}	
    } else {
		serial_print_str("US/EU");
	}	

	uns8 vol = reg >> 7;
	vol = vol & 0b00001111;
	serial_print_str(" VOL=");
	serial_print_int_hex(vol);
	
	serial_print_str(" STH=");
	serial_print_int(reg & 0b0000000001111111);
	
	serial_print_nl();
	
	reg = ar1000_read_register(AR1000_R14);
	print_reg(AR1000_R14, reg);	
	reg = reg >> 12;
	serial_print_str(" VOL2=");
	serial_print_int_hex(reg);
	
	serial_print_nl();
	
	reg = ar1000_read_register(AR1000_R10);
	print_reg(AR1000_R10, reg);
	
	serial_print_str("SWRAP=");
	print_bit(reg, R10_SEEK_WRAP_ENABLE);
    serial_print_nl();
    
	reg = ar1000_read_register(AR1000_STATUS);
	print_reg(AR1000_STATUS, reg);
	
	serial_print_nl();
	
	print_status(reg);
	serial_print_nl();

	/*uns16 status = ar1000_read_register(17);
	serial_print_str("\n[17]");
	print_status(status);
	status = ar1000_read_register(18);
	serial_print_str("\n[18]");
	print_status(status);
	status = ar1000_read_register(19);
	serial_print_str("\n[19]");
	print_status(status); */
}

// Init by sending 0x01 to 0x11 then 0x00
// d15-d8
// then d7 - d0

/* Tune
Set hmute Bit
Clear TUNE Bit
Clear SEEK Bit
Set BAND/SPACE/CHAN Bits
Enable TUNE Bit
Wait STC flag (Seek/Tune Complete, in “Status” register)
Clear hmute Bit
Update Functions (optional)

*/

/* seek
Set hmute Bit
Clear TUNE Bit
Set CHAN Bits
Clear SEEK Bit
Set SEEKUP/SPACE/BAND/SEEKTH Bits
Enable SEEK Bit
Wait STC flag (Seek/Tune Complete, in “Status” register)
Clear hmute Bit
Update Functions (optional, but remember to update CHAN with the seek in
READCHAN before next seek)
*/
// Handle any command line tasks we need to do
// Very handy to print out variables, do commands,
// print out some debug, etc...

inline void handle_tasks() {
	switch (do_task) {
		case 1:
			serial_print_str("S");
			ar1000_seek(761, 1);
			serial_print_str("\n");
			break;
		case 2:
			serial_print_str("0x\n");
			uns16 res = ar1000_read_register(send_to);
			serial_print_int_hex_16bit(res);
			serial_print_nl();
			break;
		case 3:
			serial_print_str("R\n");
			print_registers();


			
			break;
		case 4:
			serial_print_str("i\n");
			ar1000_init(); 
			break;
		case 5:
			serial_print_str("tst\n");
			//ar1000_seek2(); 
			ar1000_test();
			break;
		case 6:
			serial_print_str("S0x\n");
			ar1000_write_register(0, 0xffff);
			serial_print_nl();
			break;
		case 7:
			serial_print_str("s2\n");
			ar1000_seek2(); 
			break;
		case 8:
			serial_print_str("tu\n");
			ar1000_tune(1009); 
			break;
		case 9:
			serial_print_str("m\n");
			ar1000_seek_more(); 
			break;
			
			
	} // switch		

	do_task = 0;
}



// configure_system
//
// Get the pic ready for everything we want to do

void configure_system() {
	    
	kill_interrupts();	// turn off interrupts just in case

	turn_analog_inputs_off();	// kill those pesky analogue inputs
	
	
	serial_setup(BRGH_HIGH_SPEED, SPBRG_19200);

	ar1000_setup_io();
	//ar1000_init();
	
	turn_peripheral_ints_on();
	turn_global_ints_on();
}




void main() {
	
	configure_system();

	delay_ms(100);

	serial_print_str("\n\nAR1000\n");
	serial_print_str( "\n<");
	//serial_print_str(__TIME__);
	//serial_putc(' ');
	//serial_print_str(__DATE__);
	serial_print_str(">\n");

	while(1){

		if (serial_rx_avail()) {
			process_rx();
		}
		handle_tasks();	  

	}	// while (1)
}	// main

