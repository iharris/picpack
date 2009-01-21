/*! \file usb_cdc_class.c
    \brief Pic CDC USB routines

	Communication Device Class USB routines

	Ian Harris 2009
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if it breaks, you get to keep both pieces" license
    
   
*/

#include "config.h"

// PicPack includes
#include "pic_usb.h"
#include "pic_usb_buffer_mgt.h"

#ifdef CDC_DEBUG
	#include "pic_serial.h"
#endif

// local includes
#include "usb_cdc_class.h"

// system includes
#include "memory.h"

// class definitions

#define req_SEND_ENCAPSULATED_COMMAND	0x00
#define req_GET_ENCAPSULATED_RESPONSE	0x01
#define	req_SET_COMM_FEATURE			0x02
#define req_GET_COMM_FEATURE			0x03
#define req_CLEAR_COMM_FEATURE			0x04
#define req_SET_LINE_CODING				0x20
#define req_GET_LINE_CODING				0x21
#define req_SET_CONTROL_LINE_STATE		0x22
#define req_SEND_BREAK					0x23

typedef  union _long_union{
		long as_long;
		uns8 as_byte_array[4];
} long_union;

/** Line coding struction that defines what (should) happen if we were actually
    USB to RS232 converter
*/    
typedef struct _line_coding	 {
	long_union	dte_rate;
	uns8	stop_bits;	// 0=1 stop bit, 1=1.5 stop bits, 2=2 stop bits
	uns8	parity;		// 0=None, 1=Odd, 2=Even, 3=Mark, 4=Space
	uns8	data_bits;	// 5,6,7,8 or 16 bits
}	line_coding;

/** Transmit fifo  */
uns8 cdc_tx_buffer[USB_CDC_TX_BUFFER_SIZE];
/** Transmit fifo start point */
uns8 cdc_tx_start=0;
/** Transmit fifo end point */
uns8 cdc_tx_end=0;

/** Receive fifo */
uns8 cdc_rx_buffer[USB_CDC_RX_BUFFER_SIZE];
/** Receive fifo start point */
uns8 cdc_rx_start = 0;
/** Receive fifo end point */
uns8 cdc_rx_end = 0;
	

uns8 class_data[8];	// we'll dump all our class data in here


void usb_handle_class_request_callback(setup_data_packet sdp) {

	switch (sdp.bRequest) {
		case req_SET_LINE_CODING:
			// we now expect the line coding to arrive in the data stage
			
			#ifdef CDC_DEBUG
				serial_print_str("SET_LINE ");
			#endif
			control_mode = cm_CTRL_WRITE_DATA_STAGE_CLASS;
			break;
		case req_GET_LINE_CODING:
			#ifdef CDC_DEBUG
				serial_print_str("GET_LINE ");
				serial_print_str(" len=");
				serial_print_int(sdp.wLength);
				serial_putc(' ');
			#endif
			//control_mode = cm_CTRL_READ_DATA_STAGE_CLASS;
			control_mode = cm_CTRL_READ_DATA_STAGE_CLASS;
			//  need to prime ep0 IN with some funky data here
			usb_send_data(/*ep*/ 0, /*data*/ &class_data, /*count*/ 7, /*first*/ 1);
			// actually we know this will be the last packet, so go straight to waiting for the status ack
			control_mode = cm_CTRL_READ_AWAITING_STATUS;
			
			break;
		case req_SET_CONTROL_LINE_STATE:
			#ifdef CDC_DEBUG
				serial_print_str("scls=");//dtr = bit 0, rts = bit 1
				serial_print_int_hex(sdp.wValue);
			#endif
			// no data, so just ack the status
			control_mode = cm_CTRL_WRITE_SENDING_STATUS;
			usb_send_status_ack();
			// Could put a callback here for your own code when DTR or RTS change
			break;
		default:
			#ifdef CDC_DEBUG
				serial_print_str("??r=");
				serial_print_int(sdp.bRequest);
			#endif	
	}
}

void usb_handle_class_ctrl_write_callback(uns8 *data, uns16 count) {

	switch (usb_sdp.bRequest) {
		case req_SET_LINE_CODING:
			// dump it into class_data
			memcpy(/* dst */ (void *)&class_data,/* src */ (void *)data, count);
			
			// Now we need to send an ACK status back
			control_mode = cm_CTRL_WRITE_SENDING_STATUS;
			usb_send_status_ack();
			line_coding *my_lc;
			my_lc = (line_coding*) &class_data;
			#ifdef CDC_DEBUG
				serial_print_int_hex(my_lc->dte_rate.as_byte_array[0]);
				serial_print_int_hex(my_lc->dte_rate.as_byte_array[1]);
				serial_print_int_hex(my_lc->dte_rate.as_byte_array[2]);
				serial_print_int_hex(my_lc->dte_rate.as_byte_array[3]);
				serial_print_str(" st=");
				serial_print_int(my_lc->stop_bits);
				serial_print_str(" p=");
				serial_print_int(my_lc->parity);
				serial_print_str(" db=");
				serial_print_int(my_lc->data_bits);
			#endif
			break;	
		default:
			#ifdef CDC_DEBUG
				serial_print_str(" ??ctrl write cb req=");
				serial_print_int_hex(usb_sdp.bRequest);
				serial_putc(' ');
			#endif	
			break;
	}		
}	

void usb_handle_class_ctrl_read_callback() {
		switch (usb_sdp.bRequest) {
			case req_GET_LINE_CODING:
				// we know we've already sent everything, so now wait for status
				control_mode = cm_CTRL_READ_AWAITING_STATUS;
				break;
			default:
				#ifdef CDC_DEBUG
					serial_print_str(" cl read ?? ");
					serial_print_int(usb_sdp.bRequest);
				#endif	
		}		
			
}

void usb_ep_data_out_callback(uns8 end_point, uns8 *buffer,
                              uns16 byte_count) {
	uns8 cdc_rx_next;
	#ifdef CDC_DEBUG
		serial_print_str(" EP data out: ");
		serial_print_int(byte_count);
		serial_print_str(" bytes ");
	#endif	
	// We have some data!
	if (end_point == USB_CDC_DATA_ENDPOINT) {	// it's the data end point
		uns8 count;
		for (count = 0; count < byte_count; count++) {
			cdc_rx_next = cdc_rx_end + 1;	// get next buffer position	
			if (cdc_rx_next == USB_CDC_RX_BUFFER_SIZE) {	// if we're at the end
				cdc_rx_next = 0;	// then wrap to the beginning
			}
			if (cdc_rx_next != cdc_rx_start) { // if space in the fifo
				cdc_rx_buffer[cdc_rx_end] = buffer[count]; // put it in
				cdc_rx_end = cdc_rx_next;  // and move pointer along
			} // else... just ignore it, we've lost a byte, no room in the inn
		}	
	} else {
		#ifdef CDC_DEBUG
			serial_print_str("data for ep ");
			serial_print_int(end_point);
		#endif	
	}
}		



void usb_ep_data_in_callback(uns8 end_point, uns16 byte_count) {
	#ifdef CDC_DEBUG
		serial_print_str(" EP data in: ");
		serial_print_int(byte_count);
		serial_print_str(" bytes ");
	#endif	
	// data has been sent, so do we need to send more?
	if (end_point == USB_CDC_DATA_ENDPOINT) {	// it's the data end point
		usb_cdc_handle_tx();
	}	
}	



void usb_cdc_putc(uns8 c) {
uns8 cdc_tx_next;
bit  my_store_gie;
#ifdef CDC_IDE_DEBUG
return;
#endif

	cdc_tx_next = cdc_tx_end + 1;	// get next buffer position
	if (cdc_tx_next == USB_CDC_TX_BUFFER_SIZE) {	// if we're at the end
		cdc_tx_next = 0;	// wrap to the beginning
	}
	
	if ((!intcon.GIE) && (cdc_tx_next == cdc_tx_start)) {
		return;
	}
	while (cdc_tx_next == cdc_tx_start) { 
	}		
	my_store_gie = intcon.GIE;	// store interrupt state
	kill_interrupts();	// turn off global interrupts
	
	cdc_tx_buffer[cdc_tx_end] = c; // put it in
	cdc_tx_end = cdc_tx_next;  // move pointer along
	
	intcon.GIE = my_store_gie;	// restore interrupt state
	
}

uns8 usb_cdc_getc(void)
{
	uns8 cdc_rx_char, cdc_rx_next;
	
	while(cdc_rx_end == cdc_rx_start);	// wait until there is something received

	start_crit_sec();	// make sure nobody else can muck with the buffer
	
	cdc_rx_char = cdc_rx_buffer[cdc_rx_start];	// get character from the front of the buffer
	cdc_rx_start++;	// increment fifo start
	if (cdc_rx_start == USB_CDC_RX_BUFFER_SIZE) {	// if we're at the end
		cdc_rx_start = 0;	// then wrap to the beginning
	}
    
    end_crit_sec();	// now they can muck with the buffer
    
    return (cdc_rx_char);	// return the result we first thought of

}	// -- getc    


void usb_cdc_handle_tx()
{
uns8 cdc_tx_next;
uns8 count;
uns16 buffer_size;
uns8 *buffer;
buffer_descriptor *bd;

	bd = ep_in_bd_location[USB_CDC_DATA_ENDPOINT];
	if (test_bit(bd->stat, UOWN)) {	// if there's already something in play
		return;	// give up
	}
	
	buffer_size = ep_in_buffer_size[USB_CDC_DATA_ENDPOINT];
	buffer = ep_in_buffer_location[USB_CDC_DATA_ENDPOINT];

	if (cdc_tx_end == cdc_tx_start) { // anything in the fifo?
	   return; // nope
	}
	#ifdef CDC_DEBUG
		serial_putc('<');
	#endif
	start_crit_sec();
	
	count = 0;
	while ((cdc_tx_end != cdc_tx_start) && (count < buffer_size)) {
		
		cdc_tx_next = cdc_tx_start + 1;	// get next position
		if (cdc_tx_next == USB_CDC_TX_BUFFER_SIZE) {	// if we're at the end of the buffer
			cdc_tx_next = 0;	// wrap to the beginning
		}
		buffer[count] = cdc_tx_buffer[cdc_tx_start];	// transmit the character
		#ifdef CDC_DEBUG
			serial_putc(buffer[count]);
		#endif	
		count++;
		cdc_tx_start = cdc_tx_next;	// move start position of fifo
	} 
	if (count > 0) {
		bd->count = count;
		bd->addr = (uns16)buffer;

		toggle_bit(bd->stat, DTS);
		clear_bit(bd->stat, KEN);	// clear the keep bit
		clear_bit(bd->stat, INCDIS);	// clear the increment disable
		set_bit  (bd->stat, DTSEN);
		clear_bit(bd->stat, BSTALL);	// clear stall bit
		clear_bit(bd->stat, BC9);
		clear_bit(bd->stat, BC8);

		set_bit  (bd->stat, UOWN);	// SIE owns the buffer
	}
	end_crit_sec();
	#ifdef CDC_DEBUG
		serial_putc('>');
		serial_print_str("send=");
		serial_print_int(count);
		serial_putc(' ');
	#endif	

}

uns8 usb_cdc_rx_avail() { return cdc_rx_start != cdc_rx_end; }
uns8 usb_cdc_tx_empty() { return cdc_tx_start == cdc_tx_end; }

void usb_cdc_print_str(char *str) {

uns8 count;
buffer_descriptor *bd;

	for(count = 0 ; str[count] != 0; count++)
    {
        usb_cdc_putc(str[count]);
    }
    
    // This will give possibly quicker send:
    
    //bd = ep_in_bd_location[CDC_DATA_ENDPOINT];
    //if (!test_bit(bd->stat, UOWN)) {
    //	usb_cdc_handle_tx();
    //}	
    // otherwise we wait for the SOF interrupt to send
}    

void usb_SOF_callback(uns16 frame) {
	// we don't care about the frame number, we only care if there's something to send...
	usb_cdc_handle_tx();	// start transmission
}

void usb_cdc_setup() {
	line_coding *my_lc;
	my_lc = (line_coding*) &class_data;
	my_lc->dte_rate.as_long = 0x80250000;
	my_lc->stop_bits = 0;	// = 1 stop bit
	my_lc->data_bits = 8;	// = 8 data bits
	my_lc->parity = 0;		// = No parity
}	

void usb_cdc_print_int(uns16 i) {

char buffer[6];	// up to 5 characters plus \0
uns8 count = 5;
	buffer[5] = '\0';
	do {
		count--;
		buffer[count] = '0' + i % 10;
		i = i / 10;
	} while (i > 0);	
	while (buffer[count]) {
        usb_cdc_putc(buffer[count]);
        count++;
    }  
	//serial_print_str(&buffer[count]);	//	print it out 
//	for(count = 0 ; str[count] != 0; count++)
 //   {
 //   }

}	
