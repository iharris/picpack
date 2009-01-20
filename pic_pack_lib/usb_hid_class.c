/*! \file usb_hid_class.c
    \brief Pic Human Interface Device USB routines

	Human Interface Device USB routines

	Ian Harris 2008
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if it breaks, you get to keep both pieces" license
    
   
*/

#include "config.h"

// PicPack includes
#include "pic_usb.h"
#include "pic_usb_buffer_mgt.h"
#include "pic_serial.h"

// local includes
#include "usb_hid_class.h"

// system includes
#include "memory.h"


void usb_handle_class_ctrl_read_callback() {
}	

void usb_handle_class_ctrl_write_callback(uns8 *data, uns16 count) {
}

void usb_handle_class_request_callback(setup_data_packet sdp) {
	serial_print_str("Class request: ");
	serial_print_int(sdp.bRequest);
	
	switch(sdp.bRequest) {
		case req_GET_REPORT:
			break;
		case req_GET_IDLE:
			break;
		case req_GET_PROTOCOL:
			break;
		case req_SET_REPORT:
			break;
		case req_SET_IDLE:
			serial_print_str(" Set_idle ");
			// we don't support whatever they want
			usb_stall_ep0();
			break;
		case req_SET_PROTOCOL:
			break;
		
	}

}	
	