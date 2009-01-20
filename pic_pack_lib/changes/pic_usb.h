/*! \file pic_usb.h
    \brief Pic USB routines

	It's the way cool interrupt driven usb library

	Ian Harris 2008
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if it breaks, you get to keep both pieces" license
    
   
*/

#include "pic_utils.h"

#define USB_EP0_BUFFER_SIZE 8

typedef enum _usb_state_type  
{
	st_POWERED, 
	st_DEFAULT, 
	st_ADDRESS, 
	st_CONFIGURED
} usb_state_type;

extern usb_state_type usb_state;
extern uns8 usb_address;

typedef struct _buffer_descriptor {
	uns8	stat,
			count;
	uns16	addr;
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

// PIDs

// Token

#define pid_OUT		0b00000001
#define pid_IN		0b00001001
#define pid_SOF		0b00000101
#define pid_SETUP	0b00001101

// Data

#define pid_DATA0	0b00000011
#define pid_DATA1	0b00001011
#define pid_DATA2	0b00000111
#define pid_MDATA	0b00001111

// Handshake

#define pid_ACK		0b00000010
#define pid_NAK		0b00001010
#define pid_STALL	0b00001110
#define pid_NYET	0b00000110


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

// Descriptor types

#define dt_DEVICE			0x01
#define dt_CONFIGURATION	0x02
#define dt_STRING			0x03
#define dt_INTERFACE		0x04
#define dt_ENDPOINT			0x05
#define dt_DEVICE_QUALIFIER	0x06
#define dt_OTHER_SPEED_CONFIG	0x07
#define dt_INTERFACE_POWER	0x08
#define dt_OTG				0x09
#define dt_DEBUG			0x0a
#define dt_INTERFACE_ASSOC	0x0b
#define dt_HID				0x21
#define dt_HID_REPORT		0x22



typedef struct _device_descriptor {
	uns8	length,
			descriptor_type;
	uns16	usb_version;	// BCD
	uns8	device_class,
			device_subclass,
			device_protocol;
	uns8	max_packet_size_ep0;
	uns16	vendor_id,
			product_id,
			device_release;	// BCD
	uns8	manufacturer_string_id,
			product_string_id,
			serial_string_id,
			num_configurations;
} device_descriptor;			

typedef struct _configuration_descriptor {
	uns8	length,
			descriptor_type;
	uns16	total_length;
	uns8	num_interfaces,
			configuration_value,
			configuration_string_id,
			attributes,	// bit 7=1 must be 1
						// bit 6=1 self powered =0 bus powered 
						// bit 5=1 remote wakeup supported
			max_power;
} configuration_descriptor;


typedef struct _interface_descriptor {
	uns8	length,
			descriptor_type,
			interface_number,
			alternate_setting,
			num_endpoints,
			interface_class,
			interface_subclass,
			interface_protocol,
			interface_string_id;
} interface_descriptor;			

typedef struct _endpoint_descriptor {
	uns8	length,
			descriptor_type,
			endpoint_address,
			attributes;
	uns16	max_packet_size;
	uns8	interval;		
} endpoint_descriptor;

typedef struct _hid_descriptor {
	uns8	length,
			descriptor_type;
	uns16	hid_spec;	// bcd
	uns8	country_code,
			num_class_descriptors,
			class_descriptor_type;
	uns16	class_descriptor_length;
	
} hid_descriptor;	

extern uns8 *delivery_ptr;	



typedef enum _delivery_mode_type {
	dm_IDLE,
	dm_SEND_DATA
} delivery_mode_type;	

typedef enum _usb_status_type {
	us_IDLE,
	us_SET_ADDRESS
} usb_status_type;

typedef enum _direction_type {
	IN,
	OUT
} direction_type;

extern void usb_get_descriptor_callback(uns8 descriptor_type, uns8 descriptor_num,
                                 uns8 **rtn_descriptor_ptr, uns16 *rtn_descriptor_size);

void usb_init();
void usb_handle_isr();
void usb_turn_module_on();
void usb_send_data(uns8 endpoint, uns8 *data, uns8 send_count, uns8 max_send);
usb_state_type usb_get_state();

