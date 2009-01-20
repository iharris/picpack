#ifndef __config_h
#define __config_h
#include "platform.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// pic_rf include
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// pic_packet defines
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#define PKT_TX_QUEUE_SIZE 5
#define PKT_SEEN_LIST_SIZE 5
#define PKT_SEND_MAX_TRIES 4
#define PKT_RESEND_TICK_DELAY 10000

#define PKT_PAYLOAD_SIZE 8

//#define PKT_USE_2401A
#define PKT_USE_24L01

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



// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 126
#define SERIAL_RX_BUFFER_SIZE 4
// enable this define if you want fine-grained control...
//#define SERIAL_DEBUG_ON
//#define SERIAL_IDE_DEBUG
// - - - - - - - - - - - - - - - - - - - - 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// lcd definitions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#define lcd_e_port   PORTB
#define lcd_e_pin   7

#define lcd_rs_port  PORTC
#define lcd_rs_pin  4

#define lcd_rw_port  PORTC
#define lcd_rw_pin  5

#define lcd_db7_port PORTC
#define lcd_db7_pin 3
#define lcd_db6_port PORTC
#define lcd_db6_pin 2
#define lcd_db5_port PORTC
#define lcd_db5_pin 1
#define lcd_db4_port PORTC
#define lcd_db4_pin 0

/* i2c for rtc */

#define i2c_scl_port PORTA
#define i2c_sda_port PORTA
#define i2c_scl_pin  2
#define i2c_sda_pin  1


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// pic_rf_24l01 include
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#define rf_irq_port PORTB
#define rf_irq_pin  0

#define rf_miso_port PORTB
#define rf_miso_pin  1

#define rf_mosi_port PORTB
#define rf_mosi_pin  2

#define rf_sck_port PORTB
#define rf_sck_pin  3

#define rf_csn_port PORTB
#define rf_csn_pin  4

#define rf_ce_port PORTB
#define rf_ce_pin  5


// Platform definitions
//
//

#define PLATFORM_CLOCK 20000000
#define PLATFORM_TYPE  OLIMEX_BOARD




#endif