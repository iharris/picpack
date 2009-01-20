/*! \file pic_rf_2401a.h
    \brief Pic Nordic nrf2401a routines

	Nordic nrf2401a routines

	Ian Harris 2008
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if it breaks, you get to keep both pieces" license
    
*/

#ifndef __pic_rf_2401a_h
#define __pic_rf_2401a_h

#include "config.h"
#include "pic_utils.h"

// Copy this into your config.h

/*  

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// pic_rf include
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#define rf_ce_port PORTB
#define rf_ce_pin  1

#define rf_dr1_port PORTB
#define rf_dr1_pin  0

#define rf_cs_port PORTB
#define rf_cs_pin 3

#define rf_data_port PORTB
#define rf_data_pin 7

#define rf_clk1_port PORTB
#define rf_clk1_pin 6

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

*/


/** RF configuration structure */
typedef struct _rf_config {
	uns8 payload_width_ch2;	/**< Payload width of channel 2 in bits */
	uns8 payload_width_ch1;	/**< Payload width of channel 1 in bits */
	uns8 address_ch2[5];	/**< Address of channel 2 */
	uns8 address_ch1[5];	/**< Address of channel 1 */
	uns8 address_width; /**< Address width in bits */
	uns8 crystal;	/**< Crystal frequency look up, 3 bits valid */
	uns8 output_power; 	/**< Output power, 2 bits valid */
	uns8 channel; 	/**< Channel to operate on, 7 bits valid */
	uns8 options;	/**< Options (see option bits) */
} rf_config;	



/** rf_config options - enable receive */
#define OP_ENABLE_RECEIVE		0
/** rf_config options - enable 16 bit CRC (otherwise 8 bit CRC if 0) */
#define OP_LONG_CRC				1
/** rf_config options - enable CRC (recommended!) */
#define OP_ENABLE_CRC			2
/** rf_config options - enable 1 MPS transmition (otherwise 250Kbps) */
#define OP_ENABLE_1_MBPS		5
/** rf_config options - enable shockburst transmition */
#define OP_ENABLE_SHOCKBURST	6
/** rf_config options - enable channel 2 reception */
#define OP_ENABLE_CH2			7


/** Mode selection - receive */
#define RECEIVE_MODE 		1
/** Mode selection - transmit */
#define TRANSMIT_MODE 		0

/** Maintain state of current mode (1 = receive mode) */
static bit rf_current_mode_receive = 0;
/** Maintain state of current channel */
static uns8 rf_current_channel = 2;

/**
	\brief Setup ports and pins for communication with nrf2401a
	
	Set up ports and pins to correct input/output for communication
	with Nordif nrf2401a
*/
void pic_rf_setup();
/**
	\brief Initialise nrf2401a chip with config
	
	Sends the configuration to the Nordic nrf2401a chip ready
	to begin communication.  This routine assumes you have already
	set my_config to the correct values.
*/
void pic_rf_init(rf_config *my_config);

/**
	\brief Initialise nrf2401a chip with quick config
	
	While the usual pic_rf_init() routine is excellent
	when you want to programatically change the 2401a config,
	if you're only doing this once (at the start) then it's likely
	you're burning a lot of instructions (154 words on a PIC16 device)
	just to send some bytes of config out to the 2401a. If you
	know your config in advance, then you can just send the byte-stream
	config using this routine.  Use the nrf2401a_config.pl script
	in the tools directory to generate this string.
*/	
void pic_rf_quick_init(char *my_config, uns8 my_channel, bit my_receive_on);

/**
	\brief Receive data from nrf2401a
	
	Having been notified that there is data available, call
	this routine to clock the data in from the nrf2401a.
*/	
void pic_rf_receive(uns8* data, uns8 bytes_to_receive);

/**
	\brief Transmit data from nrf2401a
	
	Changes to transmit mode, clocks data into
	the nRF2401A and hits the shockburst button. Returns to
	receive mode when finished.
*/	
void pic_rf_transmit(char* data, uns8 bytes_to_transmit);

/**
	\brief Change channel on the nrf2401a
	
	Reclocks the essential config to change the current channel
	used by the nrf2401a.
*/	
void pic_rf_set_channel(uns8 channel);

/** Change to receive mode */
#define pic_rf_receive_mode() pic_rf_set_mode(RECEIVE_MODE)

/** Change to transmit mode */
#define pic_rf_transmit_mode() pic_rf_set_mode(TRANSMIT_MODE)

/** Set chip select line (CS) to the given value */
#define pic_rf_chip_select(value) change_pin(rf_cs_port, rf_cs_pin, value);

/** Set chip enable line (CE) to the given value */
#define pic_rf_chip_enable(value) change_pin(rf_ce_port, rf_ce_pin, value);

/**
	\brief Inline version of the pic_rf_init routine
	
	Depending upon your chip, if you're using the programmatic
	method of configuring the nrf2401a you may find that this routine
	helps you use less flash and/or stack space
*/	
#define pic_rf_init_inline(my_config) \
\
uns8 temp; \
uns8 options; \
	clear_bit(tris_array[rf_data_port - PORTA], rf_data_pin);  \
    clear_pin(rf_clk1_port, rf_clk1_pin);		 \
\
    pic_rf_chip_enable(0);\
    pic_rf_chip_select(1);	\
	\
	pic_rf_send_byte(my_config.payload_width_ch2);\
	pic_rf_send_byte(my_config.payload_width_ch1);\
	pic_rf_send_bytes(my_config.address_ch2, 5);\
	pic_rf_send_bytes(my_config.address_ch1, 5);\
	\
	\
	options = my_config.options;\
\
	temp = my_config.address_width << 2;\
	temp.1 = options.LONG_CRC; \
	temp.0 = options.ENABLE_CRC; \
\
	pic_rf_send_byte(temp);\
\
	temp = options & 0b11100000;  \
	temp |= (my_config.crystal & 0b00000111) << 2; \
	temp |= (my_config.output_power & 0b00000011); \
	\
	pic_rf_send_byte(temp);\
    	\
	temp = my_config.channel << 1;\
	rf_current_channel = my_config.channel;\
	\
	temp |= options.ENABLE_RECEIVE; \
	rf_current_mode_receive = options.ENABLE_RECEIVE;\
	\
	pic_rf_send_byte(temp);\
	\
    pic_rf_chip_select(0);	\
    pic_rf_chip_enable(1)	\
    \

/** 
	\brief Internal routine to send bytes to nrf2401a
	
	Internal routine to send bytes to the nrf2401a. Generally
	you shouldn't need to use this, see pic_rf_transmit instead
	\see pic_rf_transmit
*/	
void pic_rf_send_bytes(char *bytes, uns8 num_bytes);


/** 
	\brief Internal routine to send a byte to nrf2401a
	
	Internal routine to send a byte to the nrf2401a. Generally
	you shouldn't need to use this, see pic_rf_transmit instead
	\see pic_rf_transmit
*/	
void pic_rf_send_byte(uns8 b);

/**
	\brief Set rf mode to transmit or receive
	
	Pass RECEIVE_MODE or TRANSMIT_MODE to change current mode.
	Generally, you shouldn't need to call this routine.  The library
	assumes you want to receive until you transmit, in which case
	it switches automatically to transmit mode and back to receive
	afterwards.
*/
void pic_rf_set_mode(uns8 mode);

/**
	\brief Inline version of send_bytes
	
	Inline version of pc_rf_send_bytes.  This is an internal
	routine, and generally you shouldn't need to call it. Use
	pic_rf_transmit instead.
	
	\see pic_rf_transmit
*/
inline void pic_rf_send_bytes_inline(char *bytes, uns8 num_bytes) {

uns8 byte_counter;
	for(byte_counter = 0 ; byte_counter < num_bytes ; byte_counter++) {
		pic_rf_send_byte(bytes[byte_counter]);
	}
}		

// #define catches

#ifndef rf_ce_port
	#error "pic_rf_2401a: You haven't defined rf_ce_port in your config.h!"
#endif
#ifndef rf_ce_pin
	#error "pic_rf_2401a: You haven't defined rf_ce_pin in your config.h!"
#endif

#ifndef rf_dr1_port
	#error "pic_rf_2401a: You haven't defined rf_dr1_port in your config.h!"
#endif
#ifndef rf_dr1_pin
	#error "pic_rf_2401a: You haven't defined rf_dr1_pin in your config.h!"
#endif

#ifndef rf_cs_port
	#error "pic_rf_2401a: You haven't defined rf_cs_port in your config.h!"
#endif
#ifndef rf_cs_pin
	#error "pic_rf_2401a: You haven't defined rf_cs_pin in your config.h!"
#endif

#ifndef rf_data_port
	#error "pic_rf_2401a: You haven't defined rf_data_port in your config.h!"
#endif
#ifndef rf_data_pin
	#error "pic_rf_2401a: You haven't defined rf_data_pin in your config.h!"
#endif

#ifndef rf_clk1_port
	#error "pic_rf_2401a: You haven't defined rf_clk1_port in your config.h!"
#endif
#ifndef rf_clk1_pin
	#error "pic_rf_2401a: You haven't defined rf_clk1_pin in your config.h!"
#endif

#endif
