/**
 
 \file pic_rf_24l01.h
 
 \brief RF routines for the Nordic nRF24L01 chip

 RF routines for the Nordic nRF24L01 chip

 Ian Harris 2008
 imharris [at] gmail.com

 Released under the "do whatever you like with this
 but it breaks, you get to keep both pieces" license.

*/

#ifndef __pic_rf_24l01_h
#define __pic_rf_24l01_h

#include "config.h"
#include "pic_utils.h"

// Copy this into your config.h

/*  

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// pic_rf_24l01 include
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#define rf_ce_port PORTB
#define rf_ce_pin  2

#define rf_csn_port PORTB
#define rf_csn_pin  1

#define rf_irq_port PORTB
#define rf_irq_pin  0

#define rf_mosi_port PORTB
#define rf_mosi_pin 3	

#define rf_miso_port PORTB
#define rf_miso_pin 4

#define rf_irq_port PORTB
#define rf_irq_pin 5

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

// rf_config options bits

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

//60=bad
//0e=good

// read / write registers
#define RF_WR_REG_CONFIG_REG	0b00100000
#define RF_RD_REG_CONFIG_REG    0b00000000
#define RF_WR_REG_SETUP_RETR	0b00100100
#define RF_WR_REG_SETUP_AW 		0b00100011
#define RF_WR_REG_RF_SETUP 		0b00100110
#define RF_WR_REG_TX_ADDR 		0b00110000
#define RF_WR_REG_RX_ADDR_P0 	0b00101010
#define RF_WR_REG_EN_AA 		0b00100001
#define RF_WR_REG_RX_PW_P0 		0b00110001
#define RF_WR_REG_RF_CH         0b00100101
#define RF_WR_REG_STATUS        0b00100111

#define RF_RD_REG_RX_PW_P0		0b00010001
#define RF_RD_REG_STATUS        0b00000111
#define RF_RD_REG_FIFO_STATUS   0b00010111
#define RF_RD_REG_CD			0b00001001
#define RF_FLUSH_TX				0b11100001
#define RF_FLUSH_RX				0b11100010

// commands
#define RF_W_TX_PAYLOAD		0b10100000
#define RF_R_RX_PAYLOAD 	0b01100001

// do nothing - gets status
#define RF_NOP				0b11111111

// bits
#define CONFIG_MASK_RX_DR	6
#define CONFIG_MASK_TX_DS	5
#define CONFIG_MASK_MAX_RT	4
#define CONFIG_EN_CRC		3
#define CONFIG_CRCO			2
#define CONFIG_PWR_UP		1
#define CONFIG_PRIM_RX		0
// STATUS

#define STATUS_RX_DR	6
#define STATUS_TX_DS	5
#define STATUS_MAX_RT	4
// 3,2,1 = RX_P_NO (RX Pipe with payload 111 FIFO empty)
#define STATUS_TX_FULL	0

// FIFO_STATUS
#define FIFO_STATUS_TX_REUSE    6
#define FIFO_STATUS_TX_FULL		5
#define FIFO_STATUS_TX_EMPTY	4
#define FIFO_STATUS_RX_FULL		1
#define FIFO_STATUS_RX_EMPTY	0


/**
	\brief Setup ports and pins for communication with nRF24L01
	
	Set up ports and pins to correct input/output for communication
	with Nordic nRF24L01
*/
void pic_rf_setup();

/**
	\brief Initialise nRF24L01 chip with config
	
	Sends the configuration to the Nordic nRF24L01 chip ready
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
	\brief Transmit data from nRF24L01
	
	Changes to transmit mode, clocks data into
	the nrf24L01 and hits the shockburst button. Returns to
	receive mode when finished.
*/	
void pic_rf_transmit(uns8 *data, uns8 bytes_to_transmit);


/**
	\brief Receive data from nRF24L01
	
	Having been notified that there is data available, call
	this routine to clock the data in from the nRF24L01.
*/	
void pic_rf_receive_inline(uns8 *data, uns8 bytes_to_receive);


/**
	\brief Receive data from nRF24L01 (inline)
	
	Having been notified that there is data available, call
	this routine to clock the data in from the nRF24L01.
*/	

/**
	\brief Change channel on the nRF24L01
	
	Changes the current channel used by the nRF24L01.
*/	
void pic_rf_set_channel(uns8 channel);

/**
	\brief Receive data from nRF24L01
	
	Having been notified that there is data available, call
	this routine to clock the data in from the nRF24L01.
*/	
uns8 pic_rf_receive(uns8 *data, uns8 bytes_to_receive);

/**
	\brief Read nRF24L01 register
	
	Internal routine to read a particular nRF24L01 register. Clocks out
	data_len bytes from the chip. Internal routine.
	
	\param cmd  Read register command, eg RF_RD_REG_STATUS
	\param data Pointer to array of bytes where data will be put
	\param data_len Number of bytes to clock out 
*/
uns8 pic_rf_read_register(uns8 cmd, uns8 *data, uns8 data_len);

/**
	\brief Clock a byte into the nRF24L01
	
	Clock one byte into the nRF24L01. Internal routine.
	
	\param b The byte to send
	\return nRF24L01 status
*/	
uns8 pic_rf_send_byte(uns8 b);
/**
	\brief Clock a byte into the nRF24L01
	
	Clock one byte into the nRF24L01. Internal routine.
	
	\param b The byte to send
	\return nRF24L01 status
*/	
uns8 pic_rf_send_byte_int(uns8 b); // second version for interrupts

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
	\brief Send command to the nrf24l01
	
	Send a command and associated data to the nRF24L01
	
	\param cmd Command to send, eg, RF_WR_REG_SETUP_RETR
	\param data Pointer to an array of bytes to send as data for the command
	\param data_len Number of bytes in the array
	\return nRF24L01 status
*/	
uns8 pic_rf_send_command(uns8 cmd, uns8 *data, uns8 data_len);
/**
	\brief Send a single data byte command to the nrf24l01
	
	Send a command and 1 byte of data to the nRF24L01
	
	\param cmd Command to send, eg, RF_WR_REG_SETUP_RETR
	\param data One byte of data for the command
	\return nRF24L01 status
*/	
uns8 pic_rf_send_command_single(uns8 cmd, uns8 data);


/**
	\brief Read nRF24L01 register (inline)
	
	Internal routine to read a particular nRF24L01 register. Clocks out
	data_len bytes from the chip. Internal routine. Inline version.
	
	\param cmd  Read register command, eg RF_RD_REG_STATUS
	\param data Pointer to array of bytes where data will be put
	\param data_len Number of bytes to clock out
	\return nRF24L01 status
*/
inline uns8 pic_rf_read_register_inline(uns8 cmd, uns8 *data, uns8 data_len) {

uns8 byte_counter, status;

    clear_pin(rf_csn_port, rf_csn_pin);

	status = pic_rf_send_byte_int(cmd);
	 
	for(byte_counter = 0 ; byte_counter < data_len ; byte_counter++) {
		data[byte_counter] = pic_rf_send_byte_int(0); // dummy send to get byte back
	}
	
    set_pin(rf_csn_port, rf_csn_pin);
    
    return status;
}	

/**
	\brief Send command to the nrf24l01 (inline)
	
	Send a command and associated data to the nRF24L01. Internal routine.
	Inline version.
	
	\param cmd Command to send, eg, RF_WR_REG_SETUP_RETR
	\param data Pointer to an array of bytes to send as data for the command
	\param data_len Number of bytes in the array
	\return nRF24L01 status
*/	
inline uns8 pic_rf_send_command_inline(uns8 cmd, uns8 *data, uns8 data_len) {

uns8 byte_counter, status;

    clear_pin(rf_csn_port, rf_csn_pin);
	status = pic_rf_send_byte_int(cmd); 
	for(byte_counter = 0 ; byte_counter < data_len ; byte_counter++) {
		pic_rf_send_byte_int(data[byte_counter]);
	}							 
	
    set_pin(rf_csn_port, rf_csn_pin);
    return status;
}	


#define pic_rf_get_status() \
	    pic_rf_read_register(RF_NOP, 0, 0)

#define pic_rf_set_status(status) \
	pic_rf_send_command(RF_WR_REG_STATUS, status, 1)

#define pic_rf_receive_mode()  pic_rf_set_mode(RECEIVE_MODE)
#define pic_rf_transmit_mode() pic_rf_set_mode(TRANSMIT_MODE)



// #define catches

#ifndef rf_ce_port
	#error "pic_rf: You haven't defined rf_ce_port in your config.h!"
#endif
#ifndef rf_ce_pin
	#error "pic_rf: You haven't defined rf_ce_pin in your config.h!"
#endif

#ifndef rf_csn_port
	#error "pic_rf: You haven't defined rf_csn_port in your config.h!"
#endif
#ifndef rf_csn_pin
	#error "pic_rf: You haven't defined rf_csn_pin in your config.h!"
#endif

#ifndef rf_irq_port
	#error "pic_rf: You haven't defined rf_irq_port in your config.h!"
#endif
#ifndef rf_irq_pin
	#error "pic_rf: You haven't defined rf_irq_pin in your config.h!"
#endif

#ifndef rf_mosi_port
	#error "pic_rf: You haven't defined rf_mosi_port in your config.h!"
#endif
#ifndef rf_mosi_pin
	#error "pic_rf: You haven't defined rf_mosi_pin in your config.h!"
#endif

#ifndef rf_miso_port
	#error "pic_rf: You haven't defined rf_miso_port in your config.h!"
#endif
#ifndef rf_miso_pin
	#error "pic_rf: You haven't defined rf_miso_pin in your config.h!"
#endif

#ifndef rf_sck_port
	#error "pic_rf: You haven't defined rf_sck_port in your config.h!"
#endif
#ifndef rf_sck_pin
	#error "pic_rf: You haven't defined rf_sck_pin in your config.h!"
#endif

#endif
