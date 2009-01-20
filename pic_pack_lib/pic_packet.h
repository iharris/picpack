/*! \file pic_packet.h
    \brief Pic meshed packed network library

	Assumes either nrf2401a or nrf24l01 library

	Ian Harris 2008
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if you use it'd be nice to get an email" license. Oh, and
	if it breaks, you get to keep both pieces.
    
   
*/

// You'll need to define this routine in your own code:

/*
void pkt_payload_rx_callback(uns16 source_addr, uns16 pkt_id, uns8 *payload) {}
*/

// This will get called when a packet is received

// Put this in your config.h:

/*
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// pic_packet defines
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#define PKT_TX_QUEUE_SIZE 5
#define PKT_SEEN_LIST_SIZE 5
#define PKT_SEND_MAX_TRIES 10
#define PKT_RESEND_TICK_DELAY 500

#define PKT_PAYLOAD_SIZE 8

// Define if you want to get a callback on send failure
// #define PKT_CALLBACK_ON_SEND_FAILED
// if you define it, you'll need to include this routine in your code:
// void pkt_send_failed_callback(uns16 dest_addr, uns16 pkt_id) {
//	}

// Define if you want to get a callback on send success
// #define PKT_CALLBACK_ON_SEND_SUCCEEDED
// if you define it, you'll need to include this routine in your code:
// void pkt_send_succeeded_callback(uns16 dest_addr, uns16 pkt_id) {
//	}

// Define if you want to get a callback RF send
// #define PKT_CALLBACK_ON_SEND
// if you define it, you'll need to include this routine in your code:
// void pkt_sendcallback(uns16 dest_addr, uns16 pkt_id) {
//	}


// define one or other of these:
#define PKT_USE_2401A
//#define PKT_USE_24L01

// define if you want debug
//#define PKT_DEBUG

// define if you want high amounts of debug
//#define PKT_DEBUG_HIGH




*/



#ifndef __pic_packet_h
#define __pic_packet_h

#include "pic_utils.h"
#include "pic_serial.h"
#include "pic_tick.h"
#include "pic_serial.h"
#include "pic_tick.h"

#include "config.h"

#include <memory.h>

// Status results
/** Packet is already in seen list, ignoring */
#define PKT_STATUS_SEEN_BEFORE				1

/** I have sent this packet, so ignoring */
#define PKT_STATUS_I_AM_SENDER				2

/** Packet is for me */
#define PKT_STATUS_PKT_IS_FOR_ME			3

/** Packet is ACK for me, but didn't find in my transmit queue */
#define PKT_STATUS_PKT_IS_ACK_FOR_ME		4

/** Packet is ACK for me, found in and removed from transmit queue (successful send) */
#define PKT_STATUS_PKT_IS_FACK_FOR_ME		5

/** Packet is direct send, but not for me, ignoring */
#define PKT_STATUS_DIRECT_SEND				6

/** Packet has my address in the router list, ignoring */
#define PKT_STATUS_PREVIOUS_ROUTED_VIA_ME	7

/** Packet not retransmitted since its routing list is full */
#define PKT_STATUS_ROUTING_FULL				8

/** Packet is not for me, but protocol states I need to rebroadcast it */
#define PKT_STATUS_NEED_TO_REBROADCAST		9

/** Packet queued for transmition */
#define PKT_STATUS_QUEUED					10

/** Packet has not been queued for transmition since my transmit queue is full */
#define PKT_STATUS_TX_QUEUE_FULL			11

/** Packet is corrupt, ignoring */
#define PKT_STATUS_CHECK_FAIL				12

/** Packet is for me but seen previously */
#define PKT_STATUS_PKT_FOR_ME_BUT_SEEN		13

// flags

/** Packet should not be resent if it fails to reach destination */
#define PKT_FLAG_NO_RESEND 0

/** Packet should be resent if it fails to reach destination */
#define PKT_FLAG_RESEND 1

/** Packet should be broadcast to anyone on the network */
#define PKT_FLAG_BROADCAST 2


/** Magic config packet address */
#define PKT_CONFIG_ADDR 0xfffd

/** Send to anyone that will listen */
#define PKT_BROADCAST_ADDR 0xfffe

/** Router address for direct send only (no routing) */
#define PKT_DIRECT_SEND_ADDR 0xffff

#ifdef PKT_USE_24L01
	#include "pic_rf_24l01.h"
#else
	#include "pic_rf_2401a.h"
#endif

#ifdef PKT_DEBUG_HIGH
	#ifndef PKT_DEBUG
		#define PKT_DEBUG
	#endif	
#endif	

/** Internal packet structure. You shouldn't need to worry about this generally */
typedef struct _rf_packet_det {  // 21 bytes
	 uns16 source_addr;
	 uns16 pkt_id;
	 uns16 dest_addr;
	 uns16 r1_addr;
	 uns16 r2_addr;
	 uns16 r3_addr;
	 uns8  payload[PKT_PAYLOAD_SIZE];
	 uns8  check_byte;
} rf_packet_det;


#define PKT_PACKET_SIZE sizeof(rf_packet_det)

#define RF_RX_BUFFER_SIZE PKT_PACKET_SIZE

/** 
	\brief Initialise packet delivery system
	
	Initialise the packet delivery system ready for use.
	This routine clears the transmit queue and seen queue. If you
	don't store the last_sent_pkt_id (eg, in EEPROM) then set this to
	0.
	
	\param my_addr The address of this system
	\param last_sent_pkt_id The last pkt_id used by this system.
	
*/	
void pkt_init(uns16 my_addr, uns16 last_sent_pkt_id);


/**
	\brief Process received RF data
	
	Call this routine when your RF device has received a 
	chunk of data from somewhere.  The packet delivery
	system will handle everything including acknowledgements
	and ignoring packets that it has already seen.

	\param pkt_in A pointer to the received data.  It is assumed
	that this will point to PKT_PACKET_SIZE bytes of data.
*/
uns8 pkt_process_rf_data(uns8* data_in);
/**
	\brief Handle queued items
	
	Call this routine regularly (as often as possible) in your
	main loop in order for the packet delivery system to send any
	queued packets when it is appropriate to do so.  It will also remove
	any packets from the tx queue that have been there too long.
*/	

void pkt_process_tx_queue();

/**
	\brief Send a payload via the packet delivery system
	
	Use this routine to send a payload of data to the destination
	address.  The payload must point to PKT_PAYLOAD_SIZE bytes of
	data (as defined in your config.h).  The packet will be constructed,
	setting destination address, sender address (set previously in pkt_init)
	payload, initial routing directions and the check byte calculated.  It
	will then be placed into the tx queue and will actually be sent next time
	pkt_process_tx_queue is called.
	
	\param dest_addr Send payload to this address.  Use address of PKT_BROADCAST_ADDR
	to broadcast to all listening local addresses 
	\param pay_load Pointer to PKT_PAYLOAD_SIZE array of bytes
	\param resend Set to PKT_FLAG_RESEND or PKT_FLAG_NO_RESEND
*/
uns8 pkt_send_payload(uns16 dest_addr, uns8 *payload, uns8 resend);

/**
	\brief Called when a packet has been received
	
	Once a packet has been received, if it has not been seen before and
	it is destined to this address (set using pkt_init), then this routine
	will be called.  You must have this routine defined in your own code.

	\param source_addr The address of the system that sent the packet
	\param pkt_id The ID of the packet (the source_addr and pkt_id are used to 
	uniquely identify the packet
	\param payload A pointer to PKT_PACKET_SIZE bytes received
*/	
void pkt_payload_rx_callback(uns16 source_addr, uns16 pkt_id, uns8 *payload);


void pkt_send_failed_callback(uns16 dest_addr, uns16 pkt_id);
void pkt_send_succeeded_callback(uns16 dest_addr, uns16 pkt_id);
void pkt_send_callback(uns16 dest_addr, uns16 pkt_id);


#endif