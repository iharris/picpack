//-----------------------------------------------------
// Pic Pack library
// 
// usb_config_serial.c
//
// Descriptor responses for CDC requests
//
// Ian Harris 2008
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if it breaks, you get to keep both pieces" license.
//-----------------------------------------------------

#include "config.h"

#include "pic_utils.h"
#include "pic_usb.h"

#ifdef USB_DEBUG
	#include "pic_serial.h"
#endif	

/*

notes - check bulk packet size for full speed
stall doesn't work on version 2.0, changed to version 1.1


*/
// Device descriptor
device_descriptor my_device_descriptor = {
	sizeof(my_device_descriptor), 	// bytes long
	dt_DEVICE, 	// DEVICE 01h
	0x0110,	// usb version 1.10
	2,		// class (communication device class)
	0,		// subclass (unused)
	0,		// protocol	(unused)
	8,		// max packet size for end point 0
	0x04d8,	// Microchip's vendor
	0x000B,	// Microchip's product 
	0x0200, // version 2.0 of the product
	1,		// string 1 for manufacturer
	2,		// string 2 for product
	0,		// string 3 for serial number
	1		// number of configurations
};


// The large chunk of descriptors that gets sent
// once we have sent the device descriptor.
// There are two interfaces. The first is the comm interface,
// the seconds is the data interface (over which the actual serial
// data travels

struct  {
	configuration_descriptor my_config;
	
	interface_descriptor my_comm_interface;
	CDC_header_functional_descriptor my_header;
	CDC_ACM_functional_descriptor my_ACM;
	CDC_union_functional_descriptor my_union;
	CDC_call_mgt_functional_descriptor my_call_mgt;
	endpoint_descriptor my_notification_ep;

	interface_descriptor my_data_interface;
	endpoint_descriptor my_data_out_ep;
	endpoint_descriptor my_data_in_ep;
	
	
} complete_serial_configuration = {
	{	// configuration descriptor - - - - - - - - - - 
		sizeof(configuration_descriptor),	// length,
		dt_CONFIGURATION,	// descriptor_type,
		sizeof(complete_serial_configuration),	// total_length;
		0x02,	// num_interfaces,
		0x01,	// configuration_value,
		0x00,	// configuration_string_id,
		0b10000000, // attributes (bus powered, no remote wake up)
		100,	// max_power; (200ma)
	},
	{	// Communication interface descriptor - - - - - - - - - - - -
		sizeof(interface_descriptor),	// length,
		dt_INTERFACE,	// descriptor_type,
		0x00,	// interface_number, (starts at zero)
		0x00,	// alternate_setting, (no alternatives)
		0x01,	// num_endpoints,
		0x02,	// interface_class, (Communication Interface Class)
		0x02,	// interface_subclass, (Abstract Control Model subclass)
		0x01,	// interface_protocol, (AT commands)
		0x00,	// interface_string_id;
	},
	{	// CDC header functional descriptor - - - - - - - - - - - - - -
		sizeof(CDC_header_functional_descriptor),	//	length,
		dt_CS_INTERFACE,	//	descriptor_type (CS_INTERFACE = 0x24)
		0x00,	//	descriptor_subtype (Header functional descriptor = 0x00)
		0x0110,	//	CDC_version (BCD release of CDC spec = 0x0110)
	},
	{	// Abstract Control Model functional descriptor - - - - - - - -
		sizeof(CDC_ACM_functional_descriptor),	//	length,
		dt_CS_INTERFACE,	//	descriptor_type (CS_INTERFACE = 0x24)
		0x02,	// descriptor_subtype (ACM functional descriptor subtype = 0x02)
		0x02,	// capabilities (Device supports the request combination of Set_Line_Coding,
				// Set_Control_Line_State, Get_Line_Coding, and the notification Serial_State.
				// See http://www.usb.org/developers/devclass_docs/usbcdc11.pdf p36
	},
	{ 	// Union functional descriptor - - - - - - - - - - - - - - - 
		sizeof(CDC_union_functional_descriptor),	// length
		dt_CS_INTERFACE,	// descriptor_type (CS_INTERFACE = 0x24)
		0x06,	// descriptor_subtype (Union functional descriptor subtype = 0x06)
		0x00,	// master_interface (Controlling interface number = 0x00)
		0x01,	// slave_interface (First slave interface = 0x01)
	},
	{	// Call management functional descriptor - - - - - - - - - - 
		sizeof(CDC_call_mgt_functional_descriptor),
		dt_CS_INTERFACE,	// descriptor_type (CS_INTERFACE = 0x24)
		0x01,	// descriptor_subtype (Call management functional descriptor subtype = 0x01)
		0x00,	// capabilities 
				// bit 1 =0* Device sends/receives call management information only over
				//           the Communication Class interface
				//       =1  over data class interface
				// bit 0 =0* Device does not handle call management itself
				//       =1  Device handles call management itself
		0x01	// Data_interface  (interface number of data class interface)
	},
	{	// Notification endpoint descriptor - - - - - - - - - - - - -
		sizeof(endpoint_descriptor),	// length,
		dt_ENDPOINT,	// descriptor_type,
		0b10000010,	// endpoint_address, (Endpoint 2, IN) 
		0b00000011,	// attributes; (Interrupt)
		8,	// max_packet_size;
		2,	// interval (10ms)
	},
	{	// Data interface descriptor - - - - - - - - - - - -
		sizeof(interface_descriptor),	// length,
		dt_INTERFACE,	// descriptor_type,
		0x01,	// interface_number, (starts at zero)
		0x00,	// alternate_setting, (no alternatives)
		0x02,	// num_endpoints,
		0x0A,	// interface_class, (Data interface class)
		0x00,	// interface_subclass, (Data Interface subclass)
		0x00,	// interface_protocol, 
		0x00,	// interface_string_id;
	},
	{	// Data OUT endpoint descriptor - - - - - - - - - - - - -
		sizeof(endpoint_descriptor),	// length,
		dt_ENDPOINT,	// descriptor_type,
		0b00000011,	// endpoint_address, (Endpoint 3, OUT) 
		0b00000010,	// attributes; (Bulk)
		8,	// max_packet_size;
		0,	// no interval
	},
	{	// Data IN endpoint descriptor - - - - - - - - - - - - -
		sizeof(endpoint_descriptor),	// length,
		dt_ENDPOINT,	// descriptor_type,
		0b10000011,	// endpoint_address, (Endpoint 3, IN) 
		0b00000010,	// attributes; (Bulk)
		8,	// max_packet_size;
		0,	// no interval
	},
	
};

uns8 string_00 [] = 
	{
		4,	// length,
		dt_STRING,	// descriptor type
		9,	// magic for US english
		4
	};
uns8 string_01[] =
	{
		// string 0 (1) - Manufacturer
		24,	// length,
		dt_STRING,	// descriptor_type;
		
		'P', 0, 
		'i', 0,
		'c', 0,
		'P', 0,
		'a', 0,
		'c', 0,
		'k', 0,
		' ', 0,
		'I', 0,
		'n', 0,
		'c', 0
	};
uns8 string_02[] =
	{	
		20, // length	
		dt_STRING,	// descriptory_type;
		'S', 0,
		'e', 0,
		'r', 0,
		'i', 0,
		'a', 0,
		'l', 0,
		'U', 0,
		'S', 0,
		'B', 0,
	};

// This is the routine you need to define in order to respond to descriptor requests
void usb_get_descriptor_callback(uns8 descriptor_type, uns8 descriptor_num,
                                 uns8 **rtn_descriptor_ptr, uns16 *rtn_descriptor_size) {
	
	uns8 *descriptor_ptr;
	uns16 descriptor_size;
	
	descriptor_ptr = (uns8 *) 0;	// this means we didn't find it
	switch (descriptor_type) {
		case dt_DEVICE:
			#ifdef USB_DEBUG
				serial_print_str(" dev ");
			#endif
			descriptor_ptr = (uns8 *)&my_device_descriptor;
			descriptor_size = sizeof(my_device_descriptor);
			break;
		case dt_CONFIGURATION:
			#ifdef USB_DEBUG
				serial_print_str(" con ");
			#endif
			descriptor_ptr = (uns8 *) &complete_serial_configuration;
			descriptor_size = sizeof(complete_serial_configuration);
			break;
		case dt_STRING:
			#ifdef USB_DEBUG
				serial_print_str(" str ");
				serial_print_int(descriptor_num);
				serial_print_str(" ");
			#endif
			switch (descriptor_num) {
				case 00: 
					descriptor_size = sizeof(string_00);
					descriptor_ptr = string_00;
					break;
				case 01: 
					descriptor_size = sizeof(string_01);
					descriptor_ptr = string_01;
					break;
				case 02: 
					descriptor_size = sizeof(string_02);
					descriptor_ptr = string_02;
					break;
				// Add other strings here if you'd like	
			}		
			break;
		case dt_DEVICE_QUALIFIER:
			#ifdef USB_DEBUG
				serial_print_str(" DQ ");
			#endif
			// we don't handle this, send a stall
			// Should send a stall here :)
			// Must fix this for USB 2.0 one of these days...
			break;
		default:
			// Whoops, we don't know how to handle this request
			#ifdef USB_DEBUG
				serial_print_str("?? ");
				serial_print_int(descriptor_type);
				serial_print_spc();
			#endif
	}
	*rtn_descriptor_ptr = descriptor_ptr;
	*rtn_descriptor_size = descriptor_size;		
}

