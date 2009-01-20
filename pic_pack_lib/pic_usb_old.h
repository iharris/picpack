/*! \file pic_usb.h
    \brief Pic USB routines

	It's the way cool interrupt driven usb library

	Ian Harris 2008
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if it breaks, you get to keep both pieces" license
    
   
*/

#include "pic_utils.h"

typedef enum _usb_state_type  
{
	stPOWERED, 
	stDEFAULT, 
	stADDRESS, 
	stCONFIGURED
} usb_state_type;

extern usb_state_type usb_state;
extern uns8 usb_address;

typedef struct _buffer_descriptor {
	uns8	stat,
			count;
	uns8	addr_l, addr_h;
} buffer_descriptor;

typedef struct _setup_data_packet {
	uns8	bmRequestType,
			bRequest;
	uns16	wValue,
			wIndex,
			wLength;
} setup_data_packet;

// bits of setup_data_packet - - - - - - - - - - - - - - - - - 
// bmRequestType
// -------------
	#define DATA_STAGE_DIR	7	// 0= OUT or no data stage, 1= IN

	#define REQUEST_TYPE1	6	// 00= USB standard request
	#define REQUEST_TYPE0	5	// 01= Request for a specific class
							// 10= Vendor specific
							
	// Bits 4-0 define recipient bits
	// 0b00000= Device
	// 0b00001= Specific interface
	// 0b00010= Endpoint
	// 0b00011= Other element (whatever that means)




// Bits of BDn.stat when UOWN = 0
#define UOWN	7	// 0 if uC owns buffer
#define DTS		6	// 1 data 1 packet, 0 data 0 packet
#define KEN		5	// 1 USB will keep buffer indefinately, 0 USB will hand back buffer
#define INCDIS	4	// 1 Address increment disabled (for SPP), 0 address increment enabled
#define DTSEN	3	// 1 data toggle sync on, 0 data toggle sync off
#define BSTALL	2	// 1 buffer stall enabled, 0 buffer stall disabled
#define BC9		1	// bit 9 of buffer count
#define BC8		0	// bit 8 of buffer count

// Bits of BDn.stat when UOWN = 0
//      UOWN	7	// 1 if SIE owns buffer
//
#define PID3	5	// received token pid value of last transfer
#define PID2	4
#define PID1	3
#define PID0	2
//	    BC9 	1
//		BC8		0


// For BDn.count + bdN.stat BC9/8
// IN:
//  before - defines total number of bytes to be transferred
//  after  - returns number of bytes sent to the host
//
// OUT:
//  before - defines number of bytes that can be received and stored
//  after  - returns actual number of bytes received


// PIDs

// Token

#define pid_OUT		0b0001
#define pid_IN		0b1001
#define pid_SOF		0b0101
#define pid_SETUP	0b1101

// Data

#define pid_DATA0	0b0011
#define pid_DATA1	0b1011
#define pid_DATA2	0b0111
#define pid_MDATA	0b1111

// Handshake

#define pid_ACK		0b0010
#define pid_NAK		0b1010
#define pid_STALL	0b1110
#define pid_NYET	0b0110


// Standard USB requests

#define req_Get_Status			0x00
#define req_Clear_Feature		0x01
#define req_Set_Feature			0x03
#define req_Set_Address			0x05
#define req_Get_Descriptor		0x06
#define req_Set_Descriptor		0x07
#define req_Get_Configuration	0x08
#define req_Set_Configuration	0x09
#define req_Get_Interface		0x0a
#define req_Set_Interface		0x0b
#define req_Synch_Frame			0x0c

typedef enum _delivery_mode_type {
	dm_IDLE,
	dm_SEND_DESCRIPTOR
} delivery_mode_type;	


void usb_init();
void usb_handle_isr();
void usb_enable_module();

