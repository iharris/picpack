//-----------------------------------------------------
// Pic Pack library
// 
// usb_config_mouse.c
//
// All the mouse specific USB code lives here
//
// Ian Harris 2008
// imharris [at] gmail.com
//
// Released under the "do whatever you like with this
// but if it breaks, you get to keep both pieces" license.
//-----------------------------------------------------

#include "pic_utils.h"
#include "pic_usb.h"
#include "pic_serial.h"

device_descriptor my_device_descriptor = {
	18, 	// bytes long
	dt_DEVICE, 	// DEVICE 01h
	0x0200,	// usb version 2.00
	0,		// class
	0,		// subclass
	0,		// protocol
	8,		// max packet size for end point 0
	0x04d8,	// Microchip's vendor
	0x000A,	// Microchip's product 
	0x0200, // version 2.0 of the product
	1,		// string 1 for manufacturer
	2,		// string 2 for product
	0,		// string 3 for serial number
	1		// number of configurations
};

const char mouse_report_descriptor[] = {
   0x05, 0x01,                    //  USAGE_PAGE (Generic Desktop)
   0x09, 0x02,                    //  USAGE (Mouse)
   0xa1, 0x01,                    //  COLLECTION (Application)
   0x09, 0x01,                    //  USAGE (Pointer)
   0xa1, 0x00,                    //  COLLECTION (Physical)
   0x05, 0x09,                    //  USAGE_PAGE (Button)
   0x19, 0x01,                    //  USAGE_MINIMUM (Button 1)
   0x29, 0x03,                    //  USAGE_MAXIMUM (Button 3)
   0x15, 0x00,                    //  LOGICAL_MINIMUM (0)
   0x25, 0x01,                    //  LOGICAL_MAXIMUM (1)
   0x95, 0x03,                    //  REPORT_COUNT (3)
   0x75, 0x01,                    //  REPORT_SIZE (1)
   0x81, 0x02,                    //  INPUT (Data,Var,Abs)
   0x95, 0x01,                    //  REPORT_COUNT (1)
   0x75, 0x05,                    //  REPORT_SIZE (5)
   0x81, 0x01,                    //  INPUT (Cnst,Ary,Abs)
   0x05, 0x01,                    //  USAGE_PAGE (Generic Desktop)
   0x09, 0x30,                    //  USAGE (X)
   0x09, 0x31,                    //  USAGE (Y)
   0x15, 0x81,                    //  LOGICAL_MINIMUM (-127)
   0x25, 0x7f,                    //  LOGICAL_MAXIMUM (127)
   0x75, 0x08,                    //  REPORT_SIZE (8)
   0x95, 0x02,                    //  REPORT_COUNT (2)
   0x81, 0x06,                    //  INPUT (Data,Var,Rel)
   0xc0,                          //  END_COLLECTION
   0xc0                           //  END_COLLECTION
};


struct  {
	configuration_descriptor my_config;
	interface_descriptor my_interface;
	hid_descriptor my_hid;
	endpoint_descriptor my_ep;
} complete_mouse_configuration = {
	{	// configuration descriptor - - - - - - - - - - 
		0x09,	// length,
		dt_CONFIGURATION,	// descriptor_type,
		sizeof(complete_mouse_configuration),	// total_length;
		0x01,	// num_interfaces,
		0x01,	// configuration_value,
		0x00,	// configuration_string_id,
		0b10000000, // attributes (bus powered, no remote wake up)
		100,	// max_power; (200ma)
	},
	{	// interface descriptor - - - - - - - - - - - -
		0x09,	// length,
		dt_INTERFACE,	// descriptor_type,
		0x00,	// interface_number, (starts at zero)
		0x00,	// alternate_setting, (no alternatives)
		0x01,	// num_endpoints,
		0x03,	// interface_class, (HID)
		0x01,	// interface_subclass, (boot - is this necessary?)
		0x02,	// interface_protocol, (mouse)
		0x00,	// interface_string_id;
	},
	{	// hid descriptor - - - - - - - - - - - - - - -
		0x09,	// length,
		dt_HID,	// descriptor_type;
		0x0110,	// hid_spec in BCD (1.10)
		0x00,	// country_code, (0=not country specific)
		0x01,	// num_class_descriptors, (1)
		0x22,	// class_descriptor_type; (0x22 = report)
		sizeof(mouse_report_descriptor)	// class_descriptor_length; (report descriptor length)
	},
	{	// endpoint descriptor - - - - - - - - - - - - -
		0x07,	// length,
		dt_ENDPOINT,	// descriptor_type,
		0b10000001,	// endpoint_address, (Endpoint 1, IN)
		0b00000011,	// attributes; (Interrupt)
		3,	// max_packet_size;
		10,	// interval (10ms)
	}	
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
		'J', 0,
		'o', 0,
		'y', 0,
		'-', 0,
		'M', 0,
		'o', 0,
		'u', 0,
		's', 0,
		'e', 0,
	};


void usb_get_descriptor_callback(uns8 descriptor_type, uns8 descriptor_num,
                                 uns8 **rtn_descriptor_ptr, uns16 *rtn_descriptor_size) {
	
	uns8 *descriptor_ptr;
	uns16 descriptor_size;
	
	descriptor_ptr = (uns8 *) 0;	// this means we didn't find it
	switch (descriptor_type) {
		case dt_DEVICE:
			serial_print_str(" dev ");
			descriptor_ptr = (uns8 *)&my_device_descriptor;
			descriptor_size = sizeof(my_device_descriptor);
			break;
		case dt_CONFIGURATION:
			serial_print_str(" con ");
			descriptor_ptr = (uns8 *) &complete_mouse_configuration;
			descriptor_size = sizeof(complete_mouse_configuration);
			break;
		case dt_STRING:
			serial_print_str(" str ");
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
					
			}		
			break;
		case dt_DEVICE_QUALIFIER:
			serial_print_str(" DQ ");
			// we don't handle this, send a stall
			break;
		case dt_HID_REPORT:
			descriptor_size = sizeof(mouse_report_descriptor);
			descriptor_ptr = (uns8 *) &mouse_report_descriptor;
			break;
		default:
			serial_print_str("?? ");
			serial_print_int(descriptor_type);
			serial_print_spc();
			
	}
	*rtn_descriptor_ptr = descriptor_ptr;
	*rtn_descriptor_size = descriptor_size;		
}


/*void usb_get_descriptor_callback(uns8 descriptor_type, uns8 descriptor_num,
                                 uns8 **rtn_descriptor_ptr, uns8 *rtn_descriptor_size) {
	switch (descriptor_type) {
		case dt_DEVICE:
			serial_print_str(" dev ");
			//delivery_buffer = 0x0508;
			usb_send_descriptor(sizeof(my_device_descriptor), (uns8 *) &my_device_descriptor, 
			delivery_buffer_size = 8;
			delivery_bytes_sent = 0;
			delivery_bytes_to_send = sizeof(my_device_descriptor);
			delivery_ptr = (uns8 *) &my_device_descriptor;
			clear_bit(bd0in.stat, DTS);	// ready to get toggled
			usb_send_data_chunk();
			break;
		case dt_CONFIGURATION:
			serial_print_str(" con ");
			//delivery_buffer = 0x0508;
			delivery_buffer_size = 8;
			delivery_bytes_sent = 0;
			delivery_bytes_to_send = sizeof(complete_mouse_configuration);
			delivery_ptr = (uns8 *) &complete_mouse_configuration;
			clear_bit(bd0in.stat, DTS);	// ready to get toggled
			usb_send_data_chunk();
			break;
		case dt_STRING:
			serial_print_str(" str ");
			delivery_buffer_size = 8;
			delivery_bytes_sent = 0;
			switch (descriptor_num) {
				case 00: 
					delivery_bytes_to_send = sizeof(string_00);
					delivery_ptr = string_00;
					break;
				case 01: 
					delivery_bytes_to_send = sizeof(string_01);
					delivery_ptr = string_01;
					break;
				case 02: 
					delivery_bytes_to_send = sizeof(string_02);
					delivery_ptr = string_02;
					break;
					
			}		
			clear_bit(bd0in.stat, DTS);	// ready to get toggled
			usb_send_data_chunk();
			break;
		case dt_DEVICE_QUALIFIER:
			serial_print_str(" DQ ");
			usb_stall_ep0();
			break;
		case dt_HID_REPORT:
			serial_print_str(" HR ");
			delivery_buffer_size = 8;
			delivery_bytes_sent = 0;
			delivery_bytes_to_send = sizeof(mouse_report_descriptor);
			delivery_ptr = (uns8 *) &mouse_report_descriptor;
			clear_bit(bd0in.stat, DTS);	// ready to get toggled
			usb_send_data_chunk();
			break;
		default:
			serial_print_str("?? ");
			serial_print_int(descriptor_type);
			serial_print_spc();
			usb_stall_ep0();
			
	}		
}*/