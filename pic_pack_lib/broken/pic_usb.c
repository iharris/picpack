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
#include "config.h"
#include "pic_usb_buffer_mgt.h"


// globally accessable variables

usb_state_type usb_state = st_POWERED;

setup_data_packet usb_sdp;

uns8 usb_address;
control_mode_type control_mode;
uns16 delivery_bytes_to_send,
	  delivery_bytes_max_send,
		delivery_bytes_sent;
uns8 *delivery_ptr;	
uns8 delivery_buffer_size;
uns8 *delivery_buffer;
buffer_descriptor *delivery_bd;

usb_status_type usb_status;

void usb_configure_endpoints() {
	
serial_print_str("Config eps ");
#ifdef USB_EP1
	set_bit  (uep1, EPHSHK);	// EP handshaking on
	#ifdef USB_EP1_OUT_SIZE
		set_bit(uep1, EPOUTEN);	// EP OUT enabled
	#else
		clear_bit(uep1, EPOUTEN);	// EP OUT disabled
	#endif
	#ifdef USB_EP1_IN_SIZE
		set_bit(uep1, EPINEN);	// EP IN enabled
	#else
		clear_bit(uep1, EPINEN);	// EP IN disabled
	#endif
	set_bit  (uep1, EPCONDIS);	// control transfers off
	
	// for IN
	#ifdef USB_EP1_IN_SIZE
		set_bit(bd1in.stat, DTS);	// turn on data togle sync TOGGLE
		clear_bit(bd1in.stat, KEN);	// clear the keep bit
		clear_bit(bd1in.stat, INCDIS);	// clear the increment disable
		clear_bit(bd1in.stat, DTSEN);
		clear_bit(bd1in.stat, BSTALL);	// clear stall bit
		clear_bit(bd1in.stat, BC9);
		clear_bit(bd1in.stat, BC8);
	
		clear_bit(bd1in.stat, UOWN);	// uC owns the buffer
	#endif
	// for OUT
	#ifdef USB_EP1_OUT_SIZE
		bd1out.count = USB_EP1_OUT_SIZE;
		bd1out.addr  = USB_EP1_OUT_ADDR;
		
		clear_bit(bd1out.stat, DTS);	// turn on data togle sync TOGGLE
		clear_bit(bd1out.stat, KEN);	// clear the keep bit
		clear_bit(bd1out.stat, INCDIS);	// clear the increment disable
		clear_bit(bd1out.stat, DTSEN);
		clear_bit(bd1out.stat, BSTALL);	// clear stall bit
		clear_bit(bd1out.stat, BC9);
		clear_bit(bd1out.stat, BC8);
		set_bit  (bd1out.stat, UOWN);	// SIE owns the buffer
		
	#endif
	

#endif

#ifdef USB_EP2
	set_bit  (uep2, EPHSHK);	// EP handshaking on
	#ifdef USB_EP2_OUT_SIZE
		set_bit(uep2, EPOUTEN);	// EP OUT enabled
	#else
		clear_bit(uep2, EPOUTEN);	// EP OUT disabled
	#endif
	#ifdef USB_EP2_IN_SIZE
		set_bit(uep2, EPINEN);	// EP IN enabled
	#else
		clear_bit(uep2, EPINEN);	// EP IN disabled
	#endif
	set_bit  (uep2, EPCONDIS);	// control transfers off

	// for IN
	#ifdef USB_EP2_IN_SIZE
	
		set_bit(bd2in.stat, DTS);	// turn on data togle sync TOGGLE
		clear_bit(bd2in.stat, KEN);	// clear the keep bit
		clear_bit(bd2in.stat, INCDIS);	// clear the increment disable
		clear_bit(bd2in.stat, DTSEN);
		clear_bit(bd2in.stat, BSTALL);	// clear stall bit
		clear_bit(bd2in.stat, BC9);
		clear_bit(bd2in.stat, BC8);
	
		clear_bit(bd2in.stat, UOWN);	// uC owns the buffer
	#endif
	// for OUT
	#ifdef USB_EP2_OUT_SIZE
		bd2out.count = USB_EP2_OUT_SIZE;
		bd2out.addr  = USB_EP2_OUT_ADDR;

		clear_bit(bd2out.stat, DTS);	// turn on data togle sync TOGGLE
		clear_bit(bd2out.stat, KEN);	// clear the keep bit
		clear_bit(bd2out.stat, INCDIS);	// clear the increment disable
		clear_bit(bd2out.stat, DTSEN);
		clear_bit(bd2out.stat, BSTALL);	// clear stall bit
		clear_bit(bd2out.stat, BC9);
		clear_bit(bd2out.stat, BC8);
		set_bit  (bd2out.stat, UOWN);	// SIE owns the buffer
	#endif
#endif

#ifdef USB_EP3
	set_bit  (uep3, EPHSHK);	// EP handshaking on
	#ifdef USB_EP3_OUT_SIZE
		set_bit(uep3, EPOUTEN);	// EP OUT enabled
	#else
		clear_bit(uep3, EPOUTEN);	// EP OUT disabled
	#endif
	#ifdef USB_EP3_IN_SIZE
		set_bit(uep3, EPINEN);	// EP IN enabled
	#else
		clear_bit(uep3, EPINEN);	// EP IN disabled
	#endif
	set_bit  (uep3, EPCONDIS);	// control transfers off
	// for IN
	#ifdef USB_EP3_IN_SIZE
		set_bit(bd3in.stat, DTS);	// ready for toggle
		clear_bit(bd3in.stat, KEN);	// clear the keep bit
		clear_bit(bd3in.stat, INCDIS);	// clear the increment disable
		clear_bit(bd3in.stat, DTSEN);
		clear_bit(bd3in.stat, BSTALL);	// clear stall bit
		clear_bit(bd3in.stat, BC9);
		clear_bit(bd3in.stat, BC8);
	
		clear_bit(bd3in.stat, UOWN);	// uC owns the buffer
	#endif
	// for OUT
	#ifdef USB_EP3_OUT_SIZE
		bd3out.count = USB_EP3_OUT_SIZE;
		bd3out.addr  = USB_EP3_OUT_ADDR;

		clear_bit(bd3out.stat, DTS);	// turn on data togle sync TOGGLE
		clear_bit(bd3out.stat, KEN);	// clear the keep bit
		clear_bit(bd3out.stat, INCDIS);	// clear the increment disable
		clear_bit(bd3out.stat, DTSEN);
		clear_bit(bd3out.stat, BSTALL);	// clear stall bit
		clear_bit(bd3out.stat, BC9);
		clear_bit(bd3out.stat, BC8);
		set_bit  (bd3out.stat, UOWN);	// SIE owns the buffer
	#endif

#endif
	
}

void usb_stall_ep0() {
	set_bit(bd0in.stat, BSTALL);	// stall
	set_bit(bd0in.stat, UOWN);	// SIE owns the buffer
	set_bit(bd0out.stat, BSTALL);	// stall
	set_bit(bd0out.stat, UOWN);	// SIE owns the buffer
	//set_bit(uep0,EPSTALL);
}

void usb_send_data(uns8 ep, uns8 *data, uns8 send_count, bit first) {
uns8 count;
buffer_descriptor *bd;
uns8 *buffer;

	// this is going to be an IN transaction
	
	// need to grab buffer descriptor
	buffer = ep_in_buffer_location[ep];
	
	bd = ep_in_bd_location[ep];
	
	if (test_bit(bd->stat, UOWN)) {
		serial_print_str(" don't own it! ");
		return;
	}
	serial_putc('>');
	
	count = 0;	
	while ((count < send_count)) {
	    buffer[count] = data[count];
	    count++;
	}       	
	

	bd->count = count;
	bd->addr = (uns16)buffer;
	if (first) {
		clear_bit(bd->stat, DTS);
	}

	toggle_bit(bd->stat, DTS);
	clear_bit(bd->stat, KEN);	// clear the keep bit
	clear_bit(bd->stat, INCDIS);	// clear the increment disable
	set_bit  (bd->stat, DTSEN);
	clear_bit(bd->stat, BSTALL);	// clear stall bit
	clear_bit(bd->stat, BC9);
	clear_bit(bd->stat, BC8);

	set_bit  (bd->stat, UOWN);	// SIE owns the buffer
}	



void usb_send_data_chunk() {
	
uns8 count;

	count = 0;
	//serial_putc('C');
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
		control_mode = cm_CTRL_READ_AWAITING_STATUS;	// we're done with data stage
	}	
	//serial_print_int(count);
	
	
	bd0in.count = count;
	bd0in.addr = (uns16)&buffer_0_in;
	toggle_bit(bd0in.stat, DTS);
	clear_bit(bd0in.stat, KEN);	// clear the keep bit
	clear_bit(bd0in.stat, INCDIS);	// clear the increment disable
	set_bit  (bd0in.stat, DTSEN);
	clear_bit(bd0in.stat, BSTALL);	// clear stall bit
	clear_bit(bd0in.stat, BC9);
	clear_bit(bd0in.stat, BC8);

	set_bit  (bd0in.stat, UOWN);	// SIE owns the buffer
	/*
	delivery_bd->count = count;
	delivery_bd->addr = (uns16)delivery_buffer;
	toggle_bit(delivery_bd->stat, DTS);
	clear_bit(delivery_bd->stat, KEN);	// clear the keep bit
	clear_bit(delivery_bd->stat, INCDIS);	// clear the increment disable
	set_bit  (delivery_bd->stat, DTSEN);
	clear_bit(delivery_bd->stat, BSTALL);	// clear stall bit
	clear_bit(delivery_bd->stat, BC9);
	clear_bit(delivery_bd->stat, BC8);

	set_bit  (delivery_bd->stat, UOWN);	// SIE owns the buffer
	*/
}


void usb_send_empty_data_pkt() {
	delivery_buffer_size = USB_EP0_IN_SIZE;
	delivery_bd = &bd0in;	
	delivery_buffer = &buffer_0_in;	
	delivery_bytes_sent = 0;
	delivery_bytes_to_send = 0;
	delivery_bytes_max_send = 0;
	delivery_ptr = (uns8 *) 0;
	clear_bit(bd0in.stat, DTS);	// ready to get toggled 
	usb_send_data_chunk();
}

uns8 buffer_byte;

void usb_send_one_byte(uns8 data) {
	delivery_buffer_size = USB_EP0_IN_SIZE;
	delivery_bytes_sent = 0;
	delivery_bytes_to_send = 1;
	delivery_bytes_max_send = 0;
	buffer_byte = data;
	delivery_ptr = (uns8 *) &buffer_byte;
	delivery_bd = &bd0in;
	delivery_buffer = &buffer_0_in;
	
	clear_bit(bd0in.stat, DTS);	// ready to get toggled
	usb_send_data_chunk();
}

void usb_prime_ep0_out() {
	serial_putc('P');
	bd0out.count = USB_EP0_OUT_SIZE;
	bd0out.addr = USB_EP0_OUT_ADDR;
	//changed from clear to set
	set_bit(bd0out.stat, DTS);	// turn on data togle sync TOGGLE
	clear_bit(bd0out.stat, KEN);	// clear the keep bit
	clear_bit(bd0out.stat, INCDIS);	// clear the increment disable
	// !! changed here to not be dtsen....
	clear_bit  (bd0out.stat, DTSEN);
	clear_bit(bd0out.stat, BSTALL);	// clear stall bit
	clear_bit(bd0out.stat, BC9);
	clear_bit(bd0out.stat, BC8);

	set_bit  (bd0out.stat, UOWN);	// SIE owns the buffer
}						  

void usb_handle_standard_request() {

	switch (usb_sdp.bRequest) {
		case req_Get_Descriptor:
			serial_print_str(" Get_Desc: ");
			uns8 descriptor_type = usb_sdp.wValue >> 8; // high byte is descriptor
			uns8 descriptor_num  = usb_sdp.wValue & 0xff; // low byte is particular descriptor
			serial_print_int(descriptor_type);

			usb_get_descriptor_callback(descriptor_type, descriptor_num, &delivery_ptr, &delivery_bytes_to_send);
			if (delivery_ptr != 0) {	// we've got something
				control_mode = cm_CTRL_READ_DATA_STAGE;
				delivery_bytes_max_send = usb_sdp.wLength;	// maximum host wants
				delivery_bytes_sent = 0;	// clear our sent countr
				delivery_buffer_size = USB_EP0_IN_SIZE;
				delivery_bd = &bd0in;
				delivery_buffer = (uns8 *)USB_EP0_IN_ADDR;
				clear_bit(bd0in.stat, DTS);	// ready to get toggled
				usb_send_data_chunk();
			} else {
				serial_print_str(" <stall> ");
				usb_stall_ep0();
			}
		
			break;
		case req_Set_Address:
			serial_print_str(" Set_Addr:");
			usb_address = usb_sdp.wValue & 0xff;
			serial_print_int_hex(usb_address);
			usb_status = us_SET_ADDRESS;
			
			// Send a status ack - when we confirm that, THEN change address
			usb_send_status_ack();
			control_mode = cm_CTRL_WRITE_SENDING_STATUS;
			break;
		case req_Set_Configuration:
			serial_print_str(" Set_Config ");
			//sdp.wValue & 0xff; // MORE WORK see p136
			control_mode = cm_CTRL_WRITE_SENDING_STATUS;
			usb_send_status_ack();
			
			usb_state = st_CONFIGURED;
			
			// device is up - so now get the endpoints happy
			usb_configure_endpoints();
			// and do a call back to let the app know we're ready
			#ifdef USB_CALLBACK_ON_DEVICE_CONFIGURED
				usb_device_configured_callback();
			#endif	
			break;
		case req_Get_Interface:
				serial_print_str(" Get_Int ");
				control_mode = cm_CTRL_READ_DATA_STAGE;
				usb_send_one_byte(1);
		case req_Get_Status:
				// this is wrong - needs to send two bytes !!
				serial_print_str(" Get_Status ");
				#ifdef USB_SELF_POWERED
					usb_send_one_byte(1);
				#else
					usb_send_one_byte(0);	// bus powered
				#endif
				break;	
		default:
			serial_print_str(" ??SR ");
			serial_print_int(usb_sdp.bRequest);
			break;
			
	}	
}	


void usb_handle_transaction() {

uns8 end_point, pid;

	end_point = ustat >> 3;
	
	//serial_print_str("U:Tran ");
//read USTAT for endpoint information



	if (test_bit(ustat, DIR)) {
		serial_print_str("\nDI ");
		pid = (bd0in.stat >> 2) & 0x0f;	// mask out pid
		//serial_print_int(bd0in.count);
		//serial_print_spc();

	} else {
		serial_print_str("\nDO ");
		pid = (bd0out.stat >> 2) & 0x0f;	// mask out pid
		//serial_print_int(bd0out.count);
		//serial_print_spc();
	
	}
	if (end_point != 0) {
		serial_putc('E');
		serial_print_int_hex(end_point);
		serial_print_spc();
	}	
	//serial_print_str(" pid:0x");
	//serial_print_int_hex(pid);
	
	if (end_point == 0) {
		//serial_print_str(" bytes: ");
		//serial_print_int(bd0out.count);
		
		if (pid == pid_SETUP) {

			memcpy(/*dst*/ (void*)&usb_sdp, /*src*/ (void *)&buffer_0_out, 8);
			serial_print_int_hex(usb_sdp.bmRequestType);

			usb_prime_ep0_out();
			serial_print_str("Setup ");	
			//if (test_bit(usb_sdp.bmRequestType, DATA_STAGE_DIR)) {
				//serial_print_str(" D:IN ");
			//} else {
				//serial_print_str(" D:OUT/NO ");
			//}
			serial_print_str(" len: ");
			serial_print_int(usb_sdp.wLength);	
			// Is it a standard request?
			//serial_putc('\\');
			
			if (!test_bit(usb_sdp.bmRequestType, REQUEST_TYPE1) && // std request
			    !test_bit(usb_sdp.bmRequestType, REQUEST_TYPE0)) {
				serial_print_str(" Std req ");
				if ((usb_sdp.bmRequestType & 0b00011111) == 0) {
					//serial_print_str(" 2dev ");
				} else if ((usb_sdp.bmRequestType & 0b00011111) == 1) {	
					//serial_print_str(" 2int ");
				} else if ((usb_sdp.bmRequestType & 0b00011111) == 0b00011) {	
					//serial_print_str(" 2oth ");
				}
				
				usb_handle_standard_request();	
			} else if (!test_bit(usb_sdp.bmRequestType, REQUEST_TYPE1) && // class request
			       test_bit(usb_sdp.bmRequestType, REQUEST_TYPE0)) {
				serial_print_str(" class ");
				#ifdef USB_CALLBACK_ON_CTRL_CLASS
					usb_handle_class_request(&usb_sdp);
				#endif	
			} else {
				//serial_print_str(" req no ");
				//serial_print_int(usb_sdp.bRequest);
				// serial_print_spc();
				serial_print_str(" ??req t ");
				serial_print_int(usb_sdp.bmRequestType);
			}

	
		} else if (pid == pid_IN) {
			//serial_print_str(" IN ");
			if (control_mode == cm_CTRL_READ_DATA_STAGE) {
				// it's ours, so send next chunk
				usb_send_data_chunk();
			} else if (control_mode == cm_CTRL_WRITE_SENDING_STATUS) {
				serial_print_str("std ");
				control_mode = cm_IDLE;
				if (usb_status == us_SET_ADDRESS) {
					//serial_print_str(" addr to ");
					usb_state = st_ADDRESS;
					uaddr = usb_address;
					//serial_print_int(uaddr);
					//serial_putc('!');
					usb_status = us_IDLE;
				} 				
			} else if (control_mode == cm_CTRL_READ_AWAITING_STATUS) {
				// Must have been last IN of the read, so still waiting for status
				nop(); // boostc bug
			} else if (control_mode == cm_CTRL_READ_DATA_STAGE_CLASS) {
				// Must be more to come
				#ifdef USB_CALLBACK_ON_CTRL_CLASS
					usb_handle_ctrl_read_class();
				#else
					nop(); // otherwise boostc bug
				#endif	
			} else {
				serial_print_str(" ?? cm=");
				serial_print_int((uns8)control_mode);
				serial_print_spc();
			}
		} else if (pid == pid_ACK) {
			serial_print_str("A\n");
			if (control_mode == cm_CTRL_READ_DATA_STAGE) {	
				serial_print_str(" &2 ");
				usb_send_data_chunk();
			} else if (control_mode == cm_CTRL_READ_DATA_STAGE_CLASS) {
				#ifdef USB_CALLBACK_ON_CTRL_CLASS
					usb_handle_ctrl_read_class();
				#else
					nop(); // boostc bug
				#endif	
			} else if (control_mode == cm_CTRL_WRITE_SENDING_STATUS) {
				serial_print_str(" st sent ");
				control_mode = cm_IDLE;
			}	
	
		} else if (pid == pid_OUT) {
			// We've done an out
			//serial_print_str(" OUT ");
			if (control_mode == cm_CTRL_READ_AWAITING_STATUS) {
				serial_print_str(" status done ");
				control_mode = cm_IDLE;
			} else if (control_mode == cm_CTRL_WRITE_DATA_STAGE_CLASS) {
				#ifdef USB_CALLBACK_ON_CTRL_CLASS
					usb_handle_ctrl_write_class((uns8 *)&buffer_0_out, bd0out.count);
					// !! should include bc bits here for total count
					// this only works for 8 bit data packets
				#else
					nop();
				#endif	
			} else if (control_mode == cm_CTRL_READ_DATA_STAGE) {
				serial_print_str(" Status early ");
				control_mode = cm_IDLE;
			} else {
			
				serial_print_str("unk pid_OUT ");
				serial_print_int((uns8)control_mode);
			}
			usb_prime_ep0_out();
				
		} else {
			serial_print_str(" UKPID = ");
			serial_print_int(pid);
		}
		clear_bit(ucon, PKTDIS);	// Trap for young players right there - and do it early!
									// see pic errata

	} else {
		buffer_descriptor *bd;
		if (test_bit(ustat, DIR)) {
			// in
			#ifdef USB_EP_DATA_CALLBACK
				bd = ep_in_bd_location[end_point];
				usb_ep_data_in_callback(end_point, bd->count);
			#else
				nop();
			#endif	
		} else { // out

			bd = ep_out_bd_location[end_point];
			// issue callback
			#ifdef USB_EP_DATA_CALLBACK
				usb_ep_data_out_callback(end_point, ep_out_buffer_location[end_point],
									bd->count);
			#endif						
			// re-prime endpoint
			bd->count = ep_out_buffer_size[end_point];
			
			// Address shouldn't change, so don't need to update it

			clear_bit(bd->stat, DTS);	// turn on data togle sync TOGGLE
			clear_bit(bd->stat, KEN);	// clear the keep bit
			clear_bit(bd->stat, INCDIS);	// clear the increment disable
			clear_bit(bd->stat, DTSEN);
			clear_bit(bd->stat, BSTALL);	// clear stall bit
			clear_bit(bd->stat, BC9);
			clear_bit(bd->stat, BC8);
			set_bit  (bd->stat, UOWN);	// SIE owns the buffer
						
				
		}   
	}	
}

void usb_handle_reset() {
	usb_address = 0;
	//uaddr = 0;
	
	control_mode = cm_IDLE;
	usb_status   = us_IDLE;
	
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
	clear_bit  (bd0out.stat, DTSEN);	// !!!!!!
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
		#ifdef USB_CALLBACK_ON_SOF
			if (test_bit(uir, SOFIF)) {
				usb_SOF_callback(ufrm);	// sourceboost defines this
				clear_bit(uir, SOFIF);
			}
		#endif
	}
}


void usb_setup() {

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

	// Config buffer descriptor table
	ep_in_bd_location[0] = &bd0in;
	#if USB_HIGHEST_EP >= 1
		ep_in_bd_location[1] = &bd1in;
	#endif
	#if USB_HIGHEST_EP >= 2
		ep_in_bd_location[2] = &bd2in;
	#endif
	#if USB_HIGHEST_EP >= 3
		ep_in_bd_location[3] = &bd3in;
	#endif
	#if USB_HIGHEST_EP >= 4
		ep_in_bd_location[4] = &bd4in;
	#endif

	ep_out_bd_location[0] = &bd0out;
	#if USB_HIGHEST_EP >= 1
		ep_out_bd_location[1] = &bd1out;
	#endif
	#if USB_HIGHEST_EP >= 2
		ep_out_bd_location[2] = &bd2out;
	#endif
	#if USB_HIGHEST_EP >= 3
		ep_out_bd_location[3] = &bd3out;
	#endif
	#if USB_HIGHEST_EP >= 4
		ep_out_bd_location[4] = &bd4out;
	#endif
	
	// init endpoints
		
	set_bit(uep0,   EPHSHK);	// EP0 handshaking on
	set_bit(uep0,   EPOUTEN);	// EP0 OUT enable 
	set_bit(uep0,   EPINEN); 	// EP0 IN enable 
	clear_bit(uep0, EPCONDIS);	// EP0 control transfers on (and IN and OUT)
	
	// init interrupts

	set_bit(uie,  STALLIE);	// interrupt on stall
	set_bit(uie,  TRNIE);	//   on transaction complete
	set_bit(uie,  URSTIE);	//   on reset 
	set_bit(pie2, USBIE);	// general USB interrupts
	#ifdef USB_CALLBACK_ON_SOF 
		set_bit(uie, SOFIE);
	#endif	

}

void usb_enable_module() {
		// enable usb module
	uir = 0;
	set_bit(ucon, USBEN);	// enable USB serial interface engine (SIE)
	usb_state = st_DEFAULT;
}

usb_state_type usb_get_state() {
	return usb_state;
}	