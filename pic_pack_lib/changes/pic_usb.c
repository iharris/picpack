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

usb_state_type usb_state = st_POWERED;

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

uns8 buffer_0_out[8]@0x0500;
uns8 buffer_0_in [8]@0x0508;
uns8 buffer_1_out[8]@0x0510;
uns8 buffer_1_in [8]@0x0518;

uns8 * ep_in_buffer_locator[] = {
	(uns8 *)0x0508,	// 0 IN
	(uns8 *)0x0518, // 1 IN
};	
	
uns16    ep_in_buffer_size[] = {
	8,	// 0 IN
	8,	// 1 IN
};

uns8 usb_address;
delivery_mode_type delivery_mode;
uns16	delivery_bytes_to_send,
		delivery_bytes_max_send,
		delivery_bytes_sent;
uns8 *delivery_ptr;	
uns8 delivery_buffer_size;
uns8 *delivery_buffer;
buffer_descriptor *delivery_bd;

usb_status_type usb_status;


void usb_stall_ep0() {
	set_bit(bd0in.stat, BSTALL);	// stall
	set_bit(bd0in.stat, UOWN);	// SIE owns the buffer
	set_bit(bd0out.stat, BSTALL);	// stall
	set_bit(bd0out.stat, UOWN);	// SIE owns the buffer
}


void usb_send_data_chunk() {
	
uns8 count;

	count = 0;

	while ((count < delivery_buffer_size) &&
	       (delivery_bytes_sent < delivery_bytes_to_send) && 
	       (delivery_bytes_sent < delivery_bytes_max_send)) {
	    //buffer_0_in[count] = *delivery_ptr;
	    delivery_buffer[count] = *delivery_ptr;
	    delivery_ptr++;
	    delivery_bytes_sent++;
	    count++;
	}       	
	
	if ((count < delivery_buffer_size) && 
	    ((delivery_bytes_sent == delivery_bytes_max_send) ||
	    (delivery_bytes_sent == delivery_bytes_to_send)))  {
		delivery_mode = dm_IDLE;	// we're done
	}	
	
	//bd0in.count = count;
	delivery_bd->count = count;
	//bd0in.addr = (uns16)&buffer_0_in;
	delivery_bd->addr = (uns16)delivery_buffer;
	//toggle_bit(bd0in.stat, DTS);
	toggle_bit(delivery_bd->stat, DTS);
	//clear_bit(bd0in.stat, KEN);	// clear the keep bit
	clear_bit(delivery_bd->stat, KEN);	// clear the keep bit
	//clear_bit(bd0in.stat, INCDIS);	// clear the increment disable
	clear_bit(delivery_bd->stat, INCDIS);	// clear the increment disable
	//set_bit  (bd0in.stat, DTSEN);
	set_bit  (delivery_bd->stat, DTSEN);
	//clear_bit(bd0in.stat, BSTALL);	// clear stall bit
	clear_bit(delivery_bd->stat, BSTALL);	// clear stall bit
	//clear_bit(bd0in.stat, BC9);
	clear_bit(delivery_bd->stat, BC9);
	//clear_bit(bd0in.stat, BC8);
	clear_bit(delivery_bd->stat, BC8);

	//set_bit  (bd0in.stat, UOWN);	// SIE owns the buffer
	set_bit  (delivery_bd->stat, UOWN);	// SIE owns the buffer
}

void usb_send_data(uns8 endpoint, uns8 *data, uns8 send_count, uns8 max_send) {

uns8 count;
uns16 bd_calc;
	serial_print_str("ep=");
	serial_print_int(endpoint);
	// it's goint to be an IN endpoint
	bd_calc = 0x404 + endpoint * 8;
	delivery_bd = (buffer_descriptor *) bd_calc;
	delivery_buffer = ep_in_buffer_locator[endpoint];
	delivery_buffer_size = ep_in_buffer_size[endpoint];
	
	delivery_bytes_sent = 0;
	delivery_bytes_to_send = send_count;
	delivery_bytes_max_send = max_send;
	delivery_ptr = data;
	clear_bit(delivery_bd->stat, DTS);	// ready to get toggled
	delivery_mode = dm_SEND_DATA;

	serial_print_str(" dbd=");
	serial_print_int_hex_16bit((uns16)delivery_bd);
	serial_print_str(" dbu=");
	serial_print_int_hex_16bit((uns16)delivery_buffer);
	serial_print_str(" dbs=");
	serial_print_int_hex_16bit(delivery_buffer_size);

	usb_send_data_chunk();
}	

void usb_send_empty_data_pkt() {
	//delivery_buffer_size = 8;
	//delivery_bytes_sent = 0;
	//delivery_bytes_to_send = 0;
	//delivery_bytes_max_send = 0;
	//delivery_ptr = (uns8 *) 0;
	//clear_bit(bd0in.stat, DTS);	// ready to get toggled
	usb_send_data(0, (uns8 *)0, 0, 0);
}

uns8 buffer_byte;

void usb_send_one_byte(uns8 data) {
	//delivery_buffer_size = 1;
	//delivery_bytes_sent = 0;
	//delivery_bytes_to_send = 0;
	//delivery_bytes_max_send = 0;
	buffer_byte = data;
	//delivery_ptr = (uns8 *) &buffer_byte;
	//clear_bit(bd0in.stat, DTS);	// ready to get toggled
	//usb_send_data_chunk();
	usb_send_data(0, (uns8 *) &buffer_byte, 1, 1);
}


void usb_handle_standard_request(setup_data_packet sdp) {

	switch (sdp.bRequest) {
		case req_Get_Descriptor:
			uns8 *descriptor_ptr;
			uns16 descriptor_size;
			
			serial_print_str(" GD ");
			uns8 descriptor_type = sdp.wValue >> 8; // high byte is descriptor
			uns8 descriptor_num  = sdp.wValue & 0xff; // low byte is particular descriptor
			serial_print_int(descriptor_type);

			usb_get_descriptor_callback(descriptor_type, descriptor_num, &delivery_ptr, &delivery_bytes_to_send);
			if (delivery_ptr != 0) {	// we've got something
				//delivery_mode = dm_SEND_DATA;
				//delivery_bytes_max_send = sdp.wLength;	// maximum host wants
				//delivery_bytes_sent = 0;	// clear our sent countr
				//delivery_buffer_size = USB_EP0_BUFFER_SIZE;
				//clear_bit(bd0in.stat, DTS);	// ready to get toggled
				//usb_send_data_chunk();
				serial_print_str(" !0 ");
				usb_send_data(0, delivery_ptr, delivery_bytes_to_send, sdp.wLength);
			} else {
				usb_stall_ep0();
			}
			break;
		case req_Set_Address:
			serial_print_str(" SA ");
			usb_address = sdp.wValue & 0xff;
			serial_print_int_hex(uaddr);
			usb_status = us_SET_ADDRESS;
			usb_send_empty_data_pkt();
			break;
		case req_Set_Configuration:
			serial_print_str(" SC ");
			//sdp.wValue & 0xff; // MORE WORK see p136
			usb_send_empty_data_pkt();
			usb_state = st_CONFIGURED;
			
			// set up end point 1 for requests
			set_bit  (uep1, EPHSHK);	// EP0 handshaking on
			clear_bit(uep1, EPOUTEN);	// EP0 OUT disabled 
			set_bit  (uep1, EPINEN); 	// EP0 IN enabled 
			set_bit  (uep1, EPCONDIS);	// EP0 control transfers off

			clear_bit(bd1in.stat, DTS);
			set_bit  (bd1in.stat, DTSEN);
			clear_bit(bd1in.stat, UOWN);	//ours!
			break;
		default:
			serial_print_str(" SR? ");
			serial_print_int(sdp.bRequest);
			break;
			
	}	
}	


void usb_handle_transaction() {

uns8 end_point, pid;

	end_point = ustat >> 3;
	pid = (bd0out.stat >> 2) & 0x0f;	// mask out pid
	
	serial_print_str("T ");
//read USTAT for endpoint information



	if (test_bit(ustat, DIR)) {
		serial_print_str(" DI");
	} else {
		serial_print_str(" DO");
		
	}
	//serial_print_str(" EP: ");
	//serial_print_int_hex(end_point);
		//serial_print_str(" pid:0x");
		//serial_print_int_hex(pid);
	
	if (end_point == 0) {
		//serial_print_str(" bytes: ");
		//serial_print_int(bd0out.count);
		
		if (pid == pid_SETUP) {

			clear_bit(ucon, PKTDIS);	// Trap for young players right there - and do it early!
										// see pic errata

			setup_data_packet *sdp = (setup_data_packet*) &buffer_0_out;
			setup_data_packet my_sdp;
			memcpy( (void*)&my_sdp, (void *)sdp, 8);
			//serial_print_str(" SETUP ");

			// Is it a standard request?
			if (!test_bit(my_sdp.bmRequestType, REQUEST_TYPE1) && // std request
			    !test_bit(my_sdp.bmRequestType, REQUEST_TYPE0)) {
				//serial_print_str("Std req ");
				if ((my_sdp.bmRequestType & 0b00011111) == 0b00000) {
					//serial_print_str(" 2dev ");
				} else if ((my_sdp.bmRequestType & 0b00011111) == 0b00001) {	
					//serial_print_str(" 2int ");
				} else if ((my_sdp.bmRequestType & 0b00011111) == 0b00011) {	
					//serial_print_str(" 2oth ");
				}
				
				//serial_print_str(" req no ");
				//serial_print_int(my_sdp.bRequest);
				usb_handle_standard_request(my_sdp);	
			} else if (my_sdp.bRequest == req_Get_Interface) {
				serial_print_str(" RQ:");
				if (test_bit  (bd0in.stat, UOWN)) {
					serial_print_str("P!");	// SIE owns the buffer
				} else {
					serial_print_str("ok");
				}	
				usb_send_one_byte(1);
				
			}	
			bd0out.count = 8;	// 8 byte buffer
			bd0out.addr = 0x0500;
	
			clear_bit(bd0out.stat, DTS);	// turn on data togle sync TOGGLE
			clear_bit(bd0out.stat, KEN);	// clear the keep bit
			clear_bit(bd0out.stat, INCDIS);	// clear the increment disable
			set_bit  (bd0out.stat, DTSEN);
			clear_bit(bd0out.stat, BSTALL);	// clear stall bit
			clear_bit(bd0out.stat, BC9);
			clear_bit(bd0out.stat, BC8);

			set_bit  (bd0out.stat, UOWN);	// SIE owns the buffer
								
				
		} else if (pid == pid_IN) {
			//serial_print_str(" IN ");
			if (delivery_mode == dm_SEND_DATA) {
				usb_send_data_chunk();
			}
		} else if (pid == pid_ACK) {
			serial_print_str("A\n");
			if (delivery_mode == dm_SEND_DATA) {	
				serial_print_str("m");
				usb_send_data_chunk();
			}
			if (usb_status == us_SET_ADDRESS) {
				//serial_print_str(" addr ");
				usb_state = st_ADDRESS;
				uaddr = usb_address;
				usb_status = us_IDLE;
			} 
	
		} else {
			serial_print_str(" UKPID = ");
			serial_print_int(pid);
		}	

		if (!test_bit(bd0out.stat, UOWN)) {
			bd0out.count = 8;	// 8 byte buffer
			bd0out.addr = 0x0500;
	
			clear_bit(bd0out.stat, DTS);	// turn on data togle sync TOGGLE
			clear_bit(bd0out.stat, KEN);	// clear the keep bit
			clear_bit(bd0out.stat, INCDIS);	// clear the increment disable
			set_bit  (bd0out.stat, DTSEN);
			clear_bit(bd0out.stat, BSTALL);	// clear stall bit
			clear_bit(bd0out.stat, BC9);
			clear_bit(bd0out.stat, BC8);

			set_bit  (bd0out.stat, UOWN);	// SIE owns the buffer
		}						// since we expect frist transaction to be SETUP


	}	else {
		serial_print_str(" OEE ");
		serial_print_str(" EP: ");
		serial_print_int_hex(end_point);
		serial_print_str(" pid:0x");
		serial_print_int_hex(pid);


	}	
}

void usb_handle_reset() {
	
	usb_address = 0;
	// the hardware's copy of the address, uaddr, is automatically cleared
	// on reset
	
	delivery_mode = dm_IDLE;	// not delivering anything
	usb_status = us_IDLE;
	
	// clear fifo
	clear_bit(uir, TRNIF);
	clear_bit(uir, TRNIF);
	clear_bit(uir, TRNIF);
	clear_bit(uir, TRNIF);
	
	// init buffers
	
	// EP0 OUT
	bd0out.count = 8;	// 8 byte buffer
	bd0out.addr = 0x0500;
	
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
	bd0in.addr = 0x0508;
	clear_bit(bd0in.stat, DTS);	// turn on data togle sync TOGGLE
	clear_bit(bd0in.stat, KEN);	// clear the keep bit
	clear_bit(bd0in.stat, INCDIS);	// clear the increment disable
	clear_bit(bd0in.stat, BSTALL);	// clear stall bit
	clear_bit(bd0in.stat, BC9);
	clear_bit(bd0in.stat, BC8);
	
	clear_bit(bd0in.stat, UOWN);	// uC owns the buffer

	serial_print_str("\nR ");
	serial_print_int_hex(uir);
	
}
	
void usb_handle_stall() {
	serial_print_str(" U:Stall ");
}



void usb_handle_isr() {
	
	if (test_bit(pir2, USBIF)) {
		clear_bit(pir2, USBIF);
		
		if (test_bit(uir, TRNIF)) {
			serial_putc('!');
			usb_handle_transaction();
			clear_bit(uir, TRNIF);
		}
		
		if (test_bit(uir, URSTIF)) {
			usb_handle_reset();
			clear_bit(uir, URSTIF);
		}	
		
		if (test_bit(uir, STALLIF)) {
			usb_handle_stall();
			clear_bit(uir, STALLIF);
		}	
	}
}


void usb_init() {

	usb_state = st_POWERED;
	
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

void usb_turn_module_on() {
	// enable usb module
	
	set_bit(ucon, USBEN);	// enable USB serial interface engine (SIE)
	usb_state = st_DEFAULT;
}

usb_state_type usb_get_state() {
	return usb_state;
}	