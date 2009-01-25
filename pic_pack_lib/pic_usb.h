/*! \file pic_usb.h
    \brief Pic USB routines

	It's the way cool interrupt driven USB library

	Ian Harris 2009
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if it breaks, you get to keep both pieces" license

Put the following in your config.h

// - - - - - - - - - - - - - - - - - - - - 
// pic_usb defines
// - - - - - - - - - - - - - - - - - - - - 

// Use this define if you would like to get USB negotiation and data transfer information
// out the serial (UART) port.  You'll also need to include pic_serial.c in your project.
//#define USB_DEBUG

// Use this define if you would like a high level (ie, lots) of USB debug information
// printed out the serial (UART) port.
//#define USB_DEBUG_HIGH

// Define the highest numbered endpoint you will use (in this case, we choose 3).
#define USB_HIGHEST_EP 3

// Define either USB_SELF_POWERED or USB_BUS_POWERED
// #define USB_SELF_POWERED
#define USB_BUS_POWERED

// Define your endpoint buffers.  These start at 0x500 for a 18f4550.
// You'll always need endpoint 0, which is the control transfer endpoint,
// and others as well depending on your use.  You don't have to declare the endpoints
// you don't use, even if they're not sequential.

#define USB_EP0_OUT_SIZE 8
#define USB_EP0_OUT_ADDR 0x0500

#define USB_EP0_IN_SIZE 8
#define USB_EP0_IN_ADDR 0x0508

// EP1 not used

#define USB_EP2_IN_SIZE		8
#define USB_EP2_IN_ADDR		0x0510

#define USB_EP3_OUT_SIZE	8
#define USB_EP3_OUT_ADDR	0x0518
#define USB_EP3_IN_SIZE		8
#define USB_EP3_IN_ADDR		0x0520

// Use this define if you want to get a callback each SOF (Start Of Frame), generally every 1ms
//#define USB_CALLBACK_ON_SOF
// if you define it, you'll need to include this routine in your code:
// void usb_SOF_callback(uns16 frame) {
//	}

// Use this define if you would like to know when your device has been configured and is
// ready for use
//#define USB_CALLBACK_ON_DEVICE_CONFIGURED
// if you define it, you'll need to include this routine in your code:
// void usb_device_configured_callback() {
//	}

// Use this define if your device uses class control transfers, 
// eg, CDC (virtual serial port) is one that does
//#define USB_CALLBACK_ON_CTRL_CLASS
// if you define it, you'll need to include these routines in your code:
//void usb_handle_class_ctrl_read_callback();	
//void usb_handle_class_ctrl_write_callback(uns8 *data, uns16 count);
//void usb_handle_class_request_callback(setup_data_packet sdp);

// Use this define if you would like to get notified when data has arrived or been succesfully
// sent
//#define USB_EP_DATA_CALLBACK
// if you define it, you'll need to include these routines in your code:
//void usb_ep_data_out_callback(uns8 end_point, uns8 *buffer_location, uns16 byte_count);
//void usb_ep_data_in_callback(uns8 end_point, uns16 byte_count);

Put the following in your ISR

usb_handle_isr();

Put the following in your system setup routine    

// Setup USB
usb_setup();

// Turn on USB interrupts
void turn_usb_ints_on();

// Turn on global interrupts
turn_global_ints_on();

When you're ready to start the USB subsystem and negotiate address, send
descriptors etc, call

usb_enable_module();
   
*/

#ifndef __PIC_USB_H
#define __PIC_USB_H

#include "pic_utils.h"

/** Handle the different states that USB device can be in */
typedef enum _usb_state_type  
{
	/** USB device is powered up, ready to start negotiating */
	st_POWERED,
	/** USB device is now negotiating */ 
	st_DEFAULT, 
	/** USB device now has an address */
	st_ADDRESS, 
	/** USB device is completely configured and ready to rock and roll */
	st_CONFIGURED
} usb_state_type;

/** Store the current USB device state */
extern usb_state_type usb_state;

/** Store the usb address */
extern uns8 usb_address;

/** Describes the USB endpoint buffer descriptor */
typedef struct _buffer_descriptor {
	uns8	stat,
			count;
	uns16	addr;
} buffer_descriptor;

/** Describes a set up data packet, part of the control transfer */
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

/** Describe the state of the control transfer */ 
typedef enum _control_mode_type {
	/** No control transfer taking place */
	cm_IDLE,
	/** Device receiving data during the data stage */
	cm_CTRL_WRITE_DATA_STAGE,
	/** Device receiving data during the data stage destined for the class */
	cm_CTRL_WRITE_DATA_STAGE_CLASS,
	/** Device sending data during the data stage */
	cm_CTRL_READ_DATA_STAGE,
	/** Device class is sending data during the data stage */
	cm_CTRL_READ_DATA_STAGE_CLASS,
	/** Device is awaiting reception of status after sending data */
	cm_CTRL_READ_AWAITING_STATUS,
	/** Device is sending status after receiving data */
	cm_CTRL_WRITE_SENDING_STATUS,
} control_mode_type;	

/** Handle the special case of when we send a status ack and THEN change the address.
So we need to know that the USB device is in that micro state (ie, received the new address
but not yet sent the status */

typedef enum _usb_status_type {
	us_IDLE,
	us_SET_ADDRESS
} usb_status_type;

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

#define dt_DEVICE				0x01
#define dt_CONFIGURATION		0x02
#define dt_STRING				0x03
#define dt_INTERFACE			0x04
#define dt_ENDPOINT				0x05
#define dt_DEVICE_QUALIFIER		0x06
#define dt_OTHER_SPEED_CONFIG	0x07
#define dt_INTERFACE_POWER		0x08
#define dt_OTG					0x09
#define dt_DEBUG				0x0a
#define dt_INTERFACE_ASSOC		0x0b
#define dt_HID					0x21
#define dt_HID_REPORT			0x22

#define	dt_CS_INTERFACE			0x24

/** Device descriptor */
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

/** Configuration descriptor */
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

/** Interface descriptor */
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

/** Endpoint descriptor */
typedef struct _endpoint_descriptor {
	uns8	length,
			descriptor_type,
			endpoint_address,
			attributes;
	uns16	max_packet_size;
	uns8	interval;		
} endpoint_descriptor;

/** Human Interface Device descriptor */
typedef struct _hid_descriptor {
	uns8	length,
			descriptor_type;
	uns16	hid_spec;	// bcd
	uns8	country_code,
			num_class_descriptors,
			class_descriptor_type;
	uns16	class_descriptor_length;
	
} hid_descriptor;	

typedef struct _CDC_header_functional_descriptor {
	uns8	length,
			descriptor_type,	// CS_INTERFACE = 0x24
			descriptor_subtype;	// Header functional descriptor = 0x00
	uns16	CDC_version;		// BCD release of CDC spec = 0x0110
} CDC_header_functional_descriptor;

typedef struct _CDC_ACM_functional_descriptor {
	uns8	length,
			descriptor_type,	// CS_INTERFACE = 0x24
			descriptor_subtype,	// ACM functional descriptor subtype = 0x02
			capabilities;
} CDC_ACM_functional_descriptor;

typedef struct _CDC_union_functional_descriptor {
	uns8	length,
			descriptor_type,	// CS_INTERFACE = 0x24
			descriptor_subtype,	// Union functional descriptor subtype = 0x06
			master_interface,	// Controlling interface number (00)
			slave_interface;	// First slave interface (01)
} CDC_union_functional_descriptor;
				
typedef struct _CDC_call_mgt_functional_descriptor {
	uns8	length,
			descriptor_type,	// CS_INTERFACE = 0x24
			descriptor_subtype,	// Call management functional descriptor subtype = 0x01
			capabilities,
			data_interface;
} CDC_call_mgt_functional_descriptor;

/** Store the last setup data packet */
extern setup_data_packet usb_sdp;

/** Store the control mode state */
extern control_mode_type control_mode;

/** Send a status acknowledge by sending an empty data packet */
#define usb_send_status_ack() usb_send_empty_data_pkt()

/**
	\brief Setup USB hardware ready for use
	
	usb_setup() configures the PIC USB hardware ready for use and prepares the internal
	data structures used to keep track of where the endpoint buffers are.
	
	After calling usb_setup(), you are ready to call usb_enable_module() to actually start
	USB negotiations.  Ensure that you have usb_handle_isr() in your interrupt service routine.

*/
void usb_setup();

/**
	\brief Enables the USB hardware and starts USB negotiations
	
	After you've called usb_setup(), you can call usb_enable_module() whenever you're ready for
	USB negotiations to occur.  Normally, this would need to occur relatively quickly after power-up if
	your PIC is powered by USB and it's purpose is to talk over USB.  This is normally called from your
	main() routine once all other configuration is done.
	
	Once the USB module has successfully negotiated a connection with the host, usb_device_configured_callback()
	will be called if you have requested this in your config.h file.  This will indicate a successful connection.
	Because of the way USB works, there is no way to tell that it *hasn't* worked, except via a timer - if you haven't
	had a good connection in several seconds, you can assume it has failed (although this may just mean the user
	is hunting for a driver disk etc).

*/
void usb_enable_module();

/**
	\brief Handle USB interrupts
	
	usb_handle_isr() should be inserted in your interrupt service routine.  Alternatively, if you
	have reason not to want to do interrupt-driven USB, for example, a bootloader, you can poll this
	routine.
	
	Make sure you call turn_usb_ints() and turn_global_ints_on() to ensure interrupts occur.
	
	It will check for any of the USB interrupt flags and handle:
	USB transactions, 
	USB reset,
	USB stall,
	USB Start Of Frame (including calling usb_SOF_callback() if configured in your config.h
	and most importantly
	USB transaction, which is where all the hard work is done.
	
*/
void usb_handle_isr();
/**
	\brief Send data over an endpoint pipe
	
	Use this routine to send data across the USB pipe.

	\param ep Endpoint that the data should be sent from
	\param data pointer to the data
	\param send_count the number of bytes to send
	\param first True if this is the first in a series of sends.  Generally, this can be set to False, since it will automatically
	be set to the right value on endpoint creation.  However, in the case of control transfers, the data stage needs
	to have the first parameter set to True to ensure the DTS bit is set correctly.
*/
void usb_send_data(uns8 ep, uns8 *data, uns8 send_count, bit first);

/**
	\brief Send an empty data packet
	
	Use this routine to send an data across the USB pipe on endpoint 0.  This is the equivalent of sending a status acknowledge.

*/
void usb_send_empty_data_pkt();

/**
	\brief Send a stall on control transfer endpoint
	
	Use this routine to send a stall on the control transfer endpoint - usually used to indicate
	that the requested function is not available.

*/
void usb_stall_ep0();

/**
	\brief Query the current state of the USB connection.
	
	Returns the USB state, either powered, default, address or connect.  This is updated as the negotiation
	progresses.  It may be useful to query this if, after a suitable time-out, the connection has not been made.
*/ 
usb_state_type usb_get_state();

/**
	\brief Callback routine for a control transfer request that is placed on the class
		
	After receiving a setup packet, where the request is placed on the class, this routine is called.
	In usb_handle_class_request_callback, you can set up ready for the data stage of the control transfer.
	The direction of the data stage can be determined by examining test_bit(sdp.bRequest, DATA_STAGE_DIR) 
	although generally it appears to be obvious from the request.  The request is stored in sdp.bRequest.
	
	Typically, if it is a control read transfer (that is, it is a request by the host for data), then you will
	need to move the control_mode state variable to cm_CTRL_READ_DATA_STAGE_CLASS and send data using
	usb_send_data().  If you only intend to send one packet, you can immediately move the control_mode state
	variable to cm_CTRL_READ_AWAITING_STATUS to indicate you are waiting for the status to arrive.  You could
	wait for the usb_handle_class_ctrl_read callback and do it (move to cm_CTROL_READ_AWAITING_STATUS) but the
	PicPack USB stack can handle the control read event for you if you've already switched states.
	
	If it is a control write transfer (that is, it is a request by the host to send data to the device), then you
	will need to move the control_mode state variable to cm_CTRL_WRITE_DATA_STAGE_CLASS.  Then, the 
	usb_handle_class_ctrl_write will be fired when data is received by the device in the data stage.
	
	To allow this callback to trigger, ensure you #define USB_CALLBACK_ON_CLASS_CTRL in your config.h
*/	
void usb_handle_class_request_callback(setup_data_packet sdp);

/**
	\brief Callback routine for a class control read
	
	When a control transfer is taking place, this routine is called to indicate that a control read
	for the class has taken place.  Since everything in USB land is all about what has just happened, this
	callback will occur after data has been transferred to the host.  If you wish to send more data to the host,
	use usb_send_data(), or if your control read has sent all the data required, you will need to indicate that
	the state has changed by setting the control_mode variable to cm_CTRL_READ_AWAITING_STATUS.  This will
	indicate to the stack that it should now wait for the status packet before completing the control transfer.
	
	To allow this callback to trigger, ensure you #define USB_CALLBACK_ON_CLASS_CTRL in your config.h
*/
void usb_handle_class_ctrl_read_callback();

/**
	\brief Callback routine for a class control write
	
	When a control transfer is taking place, this routine is called to indicate that a control write
	for the class has taken place.  Since everything in USB land is all about what has just happened, this
	callback will occur after data has been received by the device.  If you expect more data from the host,
	it will arrive in due course since endpoint 0 will be primed for more data automatically. If you have
	received all the data from the host, you will need to set the control_mode state variable to
	cm_CTRL_WRITE_SENDING_STATUS and then actually send the status by calling usb_send_status_ack().
	Once the status has actually been sent, the control_mode state will automatically change to cm_IDLE to
	indicate the transfer has completed.

	To allow this callback to trigger, ensure you #define USB_CALLBACK_ON_CLASS_CTRL in your config.h
*/
void usb_handle_class_ctrl_write_callback(uns8 *data, uns16 count);

/**

	\brief Callback routine triggered when the descriptor is requested by the host
	
	Once negotiations start, descriptors are quested by the host.  The device must be able to respond
	to these requests.  Typically, this routine consists of a switch statement depending on the 
	descriptor_type parameter.  The descriptor_num is used to specify which of the descriptor_type descriptors
	are required, since they may be several (for example, string descriptors).

	Since descriptors are specific to a particular device (and project), this callback routine and 
	the associated descriptors are put in a file called usb_config_xxxx.c and placed in the project workspace. 
	This is because while the descriptors could have been provided as part of the PicPack library, you will
	almost always want to change them to suit your application, even if only for changing the vendor and device
	IDs and serial numbers.
	
	At present, descriptors are required to be in RAM.
	
	Since descriptor requests are an essential part of the USB protocol, this callback routine is mandatory.
*/
void usb_get_descriptor_callback(uns8 descriptor_type, uns8 descriptor_num,
                                 uns8 **rtn_descriptor_ptr, uns16 *rtn_descriptor_size);

/**
	\brief Callback routine triggered when data has been sent to the device
	
	If data is sent to the device and the endpoint is not endpoint 0 (the control transfer endpoint) then
	this routine is called.  Since the routine is passed the actual hardware buffer location, it is important
	to pull data out of the buffer as soon as possible in order to free up the buffer to receive more data.  The
	buffer is re-primed only once this routine completes since PicPack only supports single-buffered mode. In the
	future, we may look at supporting double buffering (ping-pong buffering) in order to be able to receive more
	data even while this routine is being called.
	
	In order for this callback to be triggered, you must #define USB_EP_DATA_CALLBACK in your config.h

	\param end_point The endpoint the data was sent do
	\param buffer_lcoation The memory location of the USB buffer where the data was received into
	\param byte_count The number of bytes received
	
*/
void usb_ep_data_out_callback(uns8 end_point, uns8 *buffer_location, uns16 byte_count);

/** 
	\brief Callback routine triggered when data has been sent to the host
	
	If you have called usb_send_data to transfer data to the host, this routine will be fired once this
	data has been transferred.  You may send more data by using usb_send_data().  Since the current PicPack
	USB library supports only single buffering, transfer speed is limited by how quickly you can refill the buffer
	again.  In the future, we may support double buffering (ping poing buffering) which will most likely improve
	transfer speeds (to be fair, transfer performance has not been a limiting factor in tests so far).
	
	In order for this callback to be triggered, you must #define USB_EP_DATA_CALLBACK in your config.h

	\param end_point The endpoint on which the data was transferred
	\param byte_count The number of bytes that were actually transferred

*/
void usb_ep_data_in_callback(uns8 end_point, uns16 byte_count);

/**
	\brief Callback routine triggered when a successful intial USB negotiation has completed
	
	Once descriptors have been received by the host and the host has selected a configuration to use,
	this routine is triggered.  Typically this means that negotiations have completed successfully and an
	appropriate driver has been loaded.
	
	In order for this callback to be triggered, you must #define USB_CALLBACK_ON_DEVICE_CONFIGURED in your
	config.h
*/	
void usb_device_configured_callback();

/**
	\brief Callback routine triggered each time a start of frame (SOF) has been received.
	
	Frames in USB occur each 1ms.  A SOF packet is sent to each device at the start of each frame. This
	is a really neat way of getting a 1ms timer without	any further work.
	
	\param frame The frame number.  Frames will wrap at 65535.
*/	
	
void usb_SOF_callback(uns16 frame);

/**
	\brief Turn on USB interrupts
	
	If you are using interrupt-driven code (generally the best way of doing things) you can turn on USB interrupts
	using turn_usb_ints_on().  Don't forget that you will also need to call turn_global_ints_on() as well. Typically
	this is called in your system setup routine.
*/	
void turn_usb_ints_on();

#ifdef USB_EP1_OUT_SIZE
	#define USB_EP1
#endif
#ifdef USB_EP1_IN_SIZE
	#ifndef USB_EP1
		#define USB_EP1
	#endif
#endif
#ifdef USB_EP2_OUT_SIZE
	#define USB_EP2
#endif
#ifdef USB_EP2_IN_SIZE
	#ifndef USB_EP2
		#define USB_EP2
	#endif
#endif
#ifdef USB_EP3_OUT_SIZE
	#define USB_EP3
#endif
#ifdef USB_EP3_IN_SIZE
	#ifndef USB_EP3
		#define USB_EP3
	#endif
#endif
#ifdef USB_EP4_OUT_SIZE
	#define USB_EP4
#endif
#ifdef USB_EP4_IN_SIZE
	#ifndef USB_EP4
		#define USB_EP4
	#endif
#endif

#ifdef USB_DEBUG_HIGH
	#ifndef USB_DEBUG
		#define USB_DEBUG
	#endif
#endif		


#ifndef USB_HIGHEST_EP
	#error "pic_usb: USB_HIGHEST_EP not defined in config.h"
#endif

#ifndef USB_SELF_POWERED
	#ifndef USB_BUS_POWERED
		#error "pic_usb: Must define either USB_SELF_POWERED or USB_BUS_POWERED in config.h"
	#endif
#endif

#ifndef USB_EP0_OUT_SIZE
	#error "pic_usb: USB_EP0_OUT_SIZE not defined in config.h"
#endif
#ifndef USB_EP0_IN_SIZE
	#error "pic_usb: USB_EP0_IN_SIZE not defined in config.h"
#endif
#ifndef USB_EP0_OUT_ADDR
	#error "pic_usb: USB_EP0_OUT_ADDR not defined in config.h"
#endif
#ifndef USB_EP0_IN_ADDR
	#error "pic_usb: USB_EP0_IN_ADDR not defined in config.h"
#endif

#endif