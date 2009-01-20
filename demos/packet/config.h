#ifndef __config_h
#define __config_h
#include "platform.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// pic_rf include
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

/*
// For breadboard testing

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
*/

// For SFE_TDN_V1 board

#define rf_ce_port PORTA
#define rf_ce_pin  6

#define rf_dr1_port PORTA
#define rf_dr1_pin  3

#define rf_cs_port PORTA
#define rf_cs_pin 0

#define rf_data_port PORTA
#define rf_data_pin 2

#define rf_clk1_port PORTA
#define rf_clk1_pin 1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// pic_packet defines
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#define PKT_TX_QUEUE_SIZE		3
#define PKT_SEEN_LIST_SIZE		5
#define PKT_SEND_MAX_TRIES		4
#define PKT_RESEND_TICK_DELAY	1000

#define PKT_PAYLOAD_SIZE 8

#define PKT_USE_2401A
//#define PKT_USE_24L01

#define PKT_DEBUG
//#define PKT_DEBUG_HIGH

// Define if you want to get a callback on send failure
#define PKT_CALLBACK_ON_SEND_FAILED
// if you define it, you'll need to include this routine in your code:
// void pkt_send_failed_callback(uns16 dest_addr, uns16 pkt_id) {
//	}

// Define if you want to get a callback on send success
#define PKT_CALLBACK_ON_SEND_SUCCEEDED
// if you define it, you'll need to include this routine in your code:
// void pkt_send_succeeded_callback(uns16 dest_addr, uns16 pkt_id) {
//	}

// Define if you want to get a callback RF send
#define PKT_CALLBACK_ON_SEND
// if you define it, you'll need to include this routine in your code:
// void pkt_sendcallback(uns16 dest_addr, uns16 pkt_id) {
//	}



// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 50
#define SERIAL_RX_BUFFER_SIZE 4
// enable this define if you want fine-grained control...
//#define SERIAL_DEBUG_ON
//#define SERIAL_IDE_DEBUG
// - - - - - - - - - - - - - - - - - - - - 

// Platform definitions
//
//

#define PLATFORM_CLOCK 8000000
#define PLATFORM_TYPE  SFE_TDN_V1


#endif