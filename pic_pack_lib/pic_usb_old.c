/*! \file pic_usb.c
    \brief Pic USB routines

	It's the way cool interrupt driven usb library

	Ian Harris 2008
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if it breaks, you get to keep both pieces" license
    
   
*/

#include "pic_usb.h"
#include "pic_serial.h"
#include "memory.h"

// globally accessable variables

usb_state_type usb_state;

buffer_descriptor bd0out@0x400;
buffer_descriptor bd0in @0x404;
buffer_descriptor bd1out@0x408;
buffer_descriptor bd1in @0x40C;
buffer_descriptor bd2out@0x410;
buffer_descriptor bd2in @0x414;
buffer_descriptor bd3out@0x418;
buffer_descriptor bd3in @0x41c;
buffer_descriptor bd4out@0x420;
buffer_descriptor bd4in @0x424;
buffer_descriptor bd5out@0x428;
buffer_descriptor bd5in @0x42C;
buffer_descriptor bd6out@0x430;
buffer_descriptor bd6in @0x434;
buffer_descriptor bd7out@0x438;
buffer_descriptor bd7in @0x43C;

uns8 usb_address;
delivery_mode_type delivery_mode;
uns16 delivery_bytes_to_send,
delivery_bytes_sent;

void usb_handle_get_descriptor(uns8 descriptor_type, uns8 descriptor_num) {
}	

void usb_handle_standard_request(setup_data_packet sdp) {

	switch (sdp.bRequest) {
		case req_Get_Descriptor:
			delivery_mode = dm_SEND_DESCRIPTOR;
			delivery_bytes_to_send = sdp.wLength;
			delivery_bytes_sent = 0;
			uns8 descriptor_type = sdp.wValue >> 8; // high byte is descriptor
			uns8 descriptor_num  = sdp.wValue & 0xff; // low byte is particular descriptor
			serial_print_str("desc: ");
			serial_print_int(descriptor_type);
			serial_print_str(" num ");
			serial_print_int(descriptor_num);
			usb_handle_get_descriptor(descriptor_type, descriptor_num);
			break;
	}		
}	


void usb_handle_transaction() {

uns8 end_point, pid;

	end_point = ustat >> 3;
	
	serial_print_str("U:Tran ");
//read USTAT for endpoint information
	if (test_bit(ustat, DIR)) {
		serial_print_str("IN ");
	} else {
		serial_print_str("OUT ");
	}
	serial_print_str(" EP: ");
	serial_print_int(end_point);
	
	if (end_point == 0) {
		pid = (bd0out.stat >> 2) & 0x0f;	// mask out pid
		serial_print_str(" pid: 0x");
		serial_print_int_hex(pid);
		serial_print_str(" bytes: ");
		serial_print_int(bd0out.count);
		return;
		if (pid == pid_SETUP) {
			setup_data_packet *sdp = (setup_data_packet*) 0x0500;
			setup_data_packet my_sdp;
			memcpy( (void*)&my_sdp, (void *)sdp, 8);
			// Is it a standard request?
			if (!test_bit(my_sdp.bmRequestType, REQUEST_TYPE1) && // std request
			    !test_bit(my_sdp.bmRequestType, REQUEST_TYPE0)) {
				serial_print_str("Std req ");
				if ((my_sdp.bmRequestType & 0b00011111) == 0b00000) {
					serial_print_str(" 2dev ");
				} else if ((my_sdp.bmRequestType & 0b00011111) == 0b00001) {	
					serial_print_str(" 2int ");
				} else if ((my_sdp.bmRequestType & 0b00011111) == 0b00011) {	
					serial_print_str(" 2oth ");
				}
				serial_print_str(" req no ");
				serial_print_int(my_sdp.bRequest);
				usb_handle_standard_request(my_sdp);	
			}	
		}    	
	}	
}

void usb_handle_reset() {
	usb_address = 0;
	uaddr = 0;
	
	// clear fifo
	clear_bit(uir, TRNIF);
	clear_bit(uir, TRNIF);
	clear_bit(uir, TRNIF);
	clear_bit(uir, TRNIF);
	
	// init buffers
	
	// EP0 OUT
	bd0out.count = 8;	// 8 byte buffer
	bd0out.addr_l = 0x00;
	bd0out.addr_h = 0x05; // starting at 0x0500
	
	clear_bit(bd0out.stat, DTS);	// turn on data togle sync TOGGLE
	clear_bit(bd0out.stat, KEN);	// clear the keep bit
	clear_bit(bd0out.stat, INCDIS);	// clear the increment disable
	set_bit  (bd0out.stat, DTSEN);
	clear_bit(bd0out.stat, BSTALL);	// clear stall bit
	clear_bit(bd0out.stat, BC9);
	clear_bit(bd0out.stat, BC8);

	set_bit  (bd0out.stat, UOWN);	// SIE owns the buffer
								// since we expect frist transaction to be SETUP
								
	// EP0 IN
	bd0in.count = 8;	// 8 byte buffer
	bd0in.addr_h = 0x05;
	bd0in.addr_l = 0x08;//  = 0x0508;	
	clear_bit(bd0in.stat, DTS);	// turn on data togle sync TOGGLE
	clear_bit(bd0in.stat, KEN);	// clear the keep bit
	clear_bit(bd0in.stat, INCDIS);	// clear the increment disable
	clear_bit(bd0in.stat, BSTALL);	// clear stall bit
	clear_bit(bd0in.stat, BC9);
	clear_bit(bd0in.stat, BC8);
	
	clear_bit(bd0in.stat, UOWN);	// uC owns the buffer

	serial_print_str("U:Reset ");
	serial_print_int_hex(uir);
	
}
	
void usb_handle_stall() {
	serial_print_str("U:Stall ");
}



void usb_handle_isr() {
	
	if (test_bit(pir2, USBIF)) {
		clear_bit(pir2, USBIF);
		
		if (test_bit(uir, TRNIF)) {
			usb_handle_transaction();
			clear_bit(uir, TRNIF);
		}
		
		if (test_bit(uir, URSTIF)) {
			usb_handle_reset();
			//clear_bit(uir, URSTIF);
			uir = 0;
		}	
		
		if (test_bit(uir, STALLIF)) {
			usb_handle_stall();
			clear_bit(uir, STALLIF);
		}	
	}
}


void usb_init() {

	usb_state = stPOWERED;
	
	// init hardware
	clear_bit(ucfg, UTRDIS);	// enable internal tranceiver
	set_bit(ucfg, FSEN);	// clear for low speed, set for high speed
	set_bit  (ucfg, UPUEN);	// enable on-chip pull-ups

	clear_bit(ucfg, PPB1);	// disable double buffering for now
	clear_bit(ucfg, PPB0);	

	// if using ping pong buffers, need to do this:
	// set_bit(ucon, PPBRST);	// reset ping pong buffers to even
	// clear_bit(ucon, PPBRST);
	
	
	// init endpoints
	
	set_bit(uep0, EPHSHK);	// EP0 handshaking on
	set_bit(uep0, EPOUTEN);	// EP0 OUT enable 
	set_bit(uep0, EPINEN); 	// EP0 IN enable 
	clear_bit(uep0, EPCONDIS);	// EP0 control transfers on (and IN and OUT)
	
	// init interrupts

	set_bit(uie, STALLIE);	// interrupt on stall
	set_bit(uie, TRNIE);	//   on transaction complete
	set_bit(uie, URSTIE);	//   on reset 
	set_bit(pie2, USBIE);	// general USB interrupts


}

void usb_enable_module() {
		// enable usb module
	
	set_bit(ucon, USBEN);	// enable USB serial interface engine (SIE)
}
