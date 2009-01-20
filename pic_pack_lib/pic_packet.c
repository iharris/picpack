/** \file pic_packet.c
    \brief Pic meshed packed network library

	Assumes either nrf2401a or nrf24l01 library

	Ian Harris 2008
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but	if it breaks, you get to keep both pieces" license.
    
   
*/

/* Todo list

- Make routing table configurable (not fixed at three)
- Config mode

*/



#include "pic_packet.h"


typedef union _rf_packet {
	rf_packet_det      d;
	char               a[PKT_PACKET_SIZE];
} rf_packet;
	


typedef struct _sending_item {
	rf_packet	packet;
    uns16     	tick_sent;
    uns8		sent_count;
    uns8		flag; // 0 don't resend / 1 resend / 0xff deleted
} sending_item;


typedef struct _seen_packet {
	uns16	pkt_id;
	uns16	source_addr;
} seen_packet;

static seen_packet    pkt_seen_list[PKT_SEEN_LIST_SIZE];
static sending_item   pkt_tx_queue[PKT_TX_QUEUE_SIZE];

uns8 pkt_seen_list_last = 0;
uns16 pkt_my_addr = 0x66;
uns16 pkt_my_next_pkt_id = 0;

/* Packet has been deleted */
#define PKT_FLAG_DELETED 0xff

uns8 pkt_seen(uns16 pkt_id, uns16 source_addr) {

uns8 count;

	for (count = 0; count < PKT_SEEN_LIST_SIZE; count++ ) {
		if ((pkt_seen_list[count].pkt_id == pkt_id) && 
		   (pkt_seen_list[count].source_addr == source_addr)) {
			return 1;
		} // if
	} // for
	return 0; // didn't find it
}			

void pkt_calc_check_byte(rf_packet *packet) {
	
	uns8 calc, count;

	calc = 0;
	for (count = 0; count < PKT_PACKET_SIZE -1; count++) { // -1 since we don't want to include the check digit
		calc ^= packet->a[count];
	}
	packet->d.check_byte = calc;
}

uns8 pkt_check_check_byte(rf_packet *packet) {
	
	uns8 calc, count;

	calc = 0;
	for (count = 0; count < PKT_PACKET_SIZE -1; count++) { // -1 since we don't want to include the check digit
		calc ^= packet->a[count];
	}

	if (calc == packet->d.check_byte) 
		return 1;
	 else 
	    return 0;
	    
}    

uns8 pkt_print_packet(rf_packet *my_packet) {
	
	serial_print_str("[Pkt: s:");
	serial_print_int_hex(my_packet->d.source_addr);
	serial_print_str(" i:");
	serial_print_int_hex(my_packet->d.pkt_id);
    serial_print_str(" d:");
    serial_print_int_hex(my_packet->d.dest_addr);
    serial_print_str(" r1:");
    serial_print_int_hex(my_packet->d.r1_addr);
    serial_print_str(" r2:");
    serial_print_int_hex(my_packet->d.r2_addr);
    serial_print_str(" r3:");
    serial_print_int_hex(my_packet->d.r3_addr);
    serial_print_str(" p:");
    for (uns8 count = 0; count < PKT_PAYLOAD_SIZE; count++) {
    	serial_print_int_hex(my_packet->d.payload[count]);
		serial_print_spc();
	}	
    serial_print_str("] ");
}    
	


uns8 pkt_queue_packet(rf_packet *packet, uns8 resend)
{
uns8 count, found;
	// find slot
	found = 0;
	for (count = 0; count < PKT_TX_QUEUE_SIZE; count++) {
		if (pkt_tx_queue[count].flag == PKT_FLAG_DELETED) {
			found = 1;
			break;
		}
	}
	if (found) {
	    #ifdef PKT_DEBUG_HIGH
			serial_print_str(" Qin ");
			serial_print_int(count);
		#endif	
		memcpy(/*dst*/  (void *)&pkt_tx_queue[count], // copy it in
			   /*src*/  (void *)packet,
			   /*len*/  PKT_PACKET_SIZE);
		pkt_tx_queue[count].sent_count = 0;
		//  resend = resend	
		pkt_tx_queue[count].flag = resend;
		return PKT_STATUS_QUEUED;
	} else {
		return PKT_STATUS_TX_QUEUE_FULL;
	}	
	
} 

//349 38
uns8 pkt_process_rf_data(uns8 *data_in) {

uns8 status = 0;
uns8 count;
uns16 orig_pkt_id;
uns8 ack_payload[PKT_PAYLOAD_SIZE] = { 0xff, 0xff };
sending_item *pitem;
rf_packet packet;
	start_crit_sec();
	
	memcpy(/*dst*/  (void *)&packet, // copy it in
		   /*src*/  (void *)data_in,
		   /*len*/  PKT_PACKET_SIZE);
	end_crit_sec();

    #ifdef PKT_DEBUG
		serial_putc('r');
        pkt_print_packet(&packet);
    #endif
    if (!pkt_check_check_byte(&packet)) {
		#ifdef PKT_DEBUG_HIGH
			serial_print_str("CF! ");
		#endif
		return PKT_STATUS_CHECK_FAIL;
	}	

	// Am I the dest addr?
	if ((packet.d.dest_addr == pkt_my_addr) ||
	    (packet.d.dest_addr == PKT_BROADCAST_ADDR)) {
		status = PKT_STATUS_PKT_IS_FOR_ME;
		// It's for me. But is it an ack?
		if ((packet.d.payload[0] != 0xff) ||
			(packet.d.payload[1] != 0xff)) { // not an ack, so send one		
			ack_payload[2] = packet.d.pkt_id & 0xff;
			ack_payload[3] = packet.d.pkt_id >> 8;
			#ifndef PKT_DEBUG_NO_TRANSMIT
				uns8 send_status = pkt_send_payload(packet.d.source_addr, ack_payload, PKT_FLAG_NO_RESEND);
				#ifdef PKT_DEBUG 
					serial_print_str("ACKst=");
					serial_print_int(send_status);
				#endif
			#endif		
		} else { // We got an ack!
			status = PKT_STATUS_PKT_IS_ACK_FOR_ME;
			orig_pkt_id = packet.d.payload[3];
			orig_pkt_id <<= 8;
			orig_pkt_id += packet.d.payload[2];
			for (count = 0; count < PKT_TX_QUEUE_SIZE; count++) { // Search for a packet
				pitem = &pkt_tx_queue[count];
				if ((pitem->flag != PKT_FLAG_DELETED) &&
				(pitem->packet.d.pkt_id == orig_pkt_id)) {
					status = PKT_STATUS_PKT_IS_FACK_FOR_ME;	 // Found the ack
					#ifdef PKT_CALLBACK_ON_SEND_SUCCEEDED
						pkt_send_succeeded_callback(pitem->packet.d.dest_addr, pitem->packet.d.pkt_id);
					#endif	
					
					pitem->flag = PKT_FLAG_DELETED;
				}
			}			
		} // [we got an ack]

		
		// If not seen before, then process		
		if (!pkt_seen(packet.d.pkt_id, packet.d.source_addr)) {
		
			// process payload
			if ((status != PKT_STATUS_PKT_IS_FACK_FOR_ME) && 
			   (status != PKT_STATUS_PKT_IS_ACK_FOR_ME)) {
				pkt_payload_rx_callback(packet.d.source_addr, packet.d.pkt_id, packet.d.payload);
			}	
			// Add to seen list
			pkt_seen_list_last++;
			if (pkt_seen_list_last == PKT_SEEN_LIST_SIZE) {
				pkt_seen_list_last = 0;
			}
			pkt_seen_list[pkt_seen_list_last].pkt_id = packet.d.pkt_id;
			pkt_seen_list[pkt_seen_list_last].source_addr = packet.d.source_addr;
		} else {
			status = PKT_STATUS_PKT_FOR_ME_BUT_SEEN;
			#ifdef PKT_DEBUG_HIGH
				serial_print_str(" seen ");
			#endif	
		}	
	}		
	else
	// Is sender + pkt_id seen?
	if (pkt_seen(packet.d.pkt_id, packet.d.source_addr)) {
		// We've seen this packet before
		status = PKT_STATUS_SEEN_BEFORE;	
	} else 
	// Am I the sender?
	if (packet.d.source_addr == pkt_my_addr) {
		status = PKT_STATUS_I_AM_SENDER;
	} else
	// Is the r1_addr = 0xffff, meaning direct send?
	if (packet.d.r1_addr == PKT_DIRECT_SEND_ADDR) {
		status = PKT_STATUS_DIRECT_SEND;
	} else 	 
	// Am I one of the routers?
	if ((packet.d.r1_addr == pkt_my_addr) ||
	    (packet.d.r2_addr == pkt_my_addr) ||
	    (packet.d.r3_addr == pkt_my_addr)) {
		status = PKT_STATUS_PREVIOUS_ROUTED_VIA_ME;
	} else 
	// Is r3 full?
	if (packet.d.r3_addr != 0) {
		status = PKT_STATUS_ROUTING_FULL;
	} else {  // I need to re-broadcast this packet
		status = PKT_STATUS_NEED_TO_REBROADCAST;
		// Add me as a router
		if (packet.d.r1_addr == 0) {
			packet.d.r1_addr = pkt_my_addr;
		} else if (packet.d.r2_addr == 0) {
			packet.d.r2_addr = pkt_my_addr;
		} else { 
			packet.d.r3_addr = pkt_my_addr;
		}		
		// Update check byte now we've changed packet
		pkt_calc_check_byte(&packet);
		// queue packet no resend
		
		#ifndef PKT_DEBUG_NO_TRANSMIT
			pkt_queue_packet(&packet, PKT_FLAG_NO_RESEND);
		#endif
		// add to seen list?
	} // [I need to re-broadcast this packet]

	return status;
}

#ifdef PKT_USE_24L01
inline void pkt_send_packet(rf_packet *packet) {

//uns8 tx_buffer[PKT_PACKET_SIZE + 3]; // +3 for RF address (fixed)
uns8 count;

	#ifdef PKT_DEBUG
	    serial_print_str(" <S>");
	#endif   
	#ifdef PKT_DEBUG_HIGH
	    pkt_print_packet(packet);
	#endif   
	//tx_buffer[0] = 0b11100111; // address
	//tx_buffer[1] = 0b11100111; // address
	//tx_buffer[2] = 0b11100111; // address
    //for (count = 0; count < PKT_PACKET_SIZE; count++) {
	 // tx_buffer[count+3] = packet->a[count];
	//}  
	//pic_rf_transmit(tx_buffer, PKT_PACKET_SIZE + 3);
	
	pic_rf_transmit((uns8 *)packet, PKT_PACKET_SIZE);
}

#else
inline void pkt_send_packet(rf_packet *packet) {

uns8 tx_buffer[PKT_PACKET_SIZE + 3]; // +3 for RF address (fixed)
uns8 count;

	#ifdef PKT_DEBUG
	    serial_print_str(" <S>");
	#endif    
	#ifdef PKT_DEBUG_HIGH
	    pkt_print_packet(packet);
	#endif   
	tx_buffer[0] = 0b11100111; // address
	tx_buffer[1] = 0b11100111; // address
	tx_buffer[2] = 0b11100111; // address
    for (count = 0; count < PKT_PACKET_SIZE; count++) {
	  tx_buffer[count+3] = packet->a[count];
	}  
	pic_rf_transmit(tx_buffer, PKT_PACKET_SIZE + 3);
	//pic_rf_transmit((uns8 *)packet, PKT_PACKET_SIZE);
}


#endif

	

void pkt_process_tx_queue() {

uns8 count;
uns16 current_tick;
uns8 sent_count;
uns8 flag;
sending_item *item;
    
    current_tick = tick_get_count();
    
	for (count = 0; count < PKT_TX_QUEUE_SIZE; count++) { // search for a packet
		item = &pkt_tx_queue[count];	// grab item to save code
		flag = item->flag;
		if (flag != PKT_FLAG_DELETED) {	// got something to send
			sent_count = item->sent_count;
			if (sent_count > PKT_SEND_MAX_TRIES) { // Sent too many times
				item->flag = PKT_FLAG_DELETED; // Delete
				#ifdef PKT_CALLBACK_ON_SEND_FAILED
					pkt_send_failed_callback(item->packet.d.dest_addr, item->packet.d.pkt_id);
				#endif
				#ifdef PKT_DEBUG
					serial_print_str(" SF!\n ");
				#endif
			} else if ((sent_count == 0) // Never tried to send 
			   || (tick_calc_diff(item->tick_sent, current_tick)
			         > PKT_RESEND_TICK_DELAY*sent_count + count)) { // Or more than delay time since last try
						// Note slight randomness built in by adding array position (count)
				item->tick_sent = current_tick; // set tick count to current
				//delay_us(pkt_my_addr & 0x07 *30);
				#ifdef PKT_DEBUG_PAYLOAD_SENT
					item->packet.d.payload[7] = sent_count;
					pkt_calc_check_byte((rf_packet*)item);
				#endif
				#ifdef PKT_DEBUG_HIGH
					serial_print_str(" t=");
					serial_print_int(current_tick);
					serial_print_spc();
				#endif	
				pkt_send_packet((rf_packet*)item);	// send it
				#ifdef PKT_CALLBACK_ON_SEND
					pkt_send_callback(item->packet.d.dest_addr, item->packet.d.pkt_id);
				#endif
				
				if (flag != PKT_FLAG_RESEND) {  // no resend?
					item->flag = PKT_FLAG_DELETED;  // then delete
					#ifdef PKT_DEBUG_HIGH
						serial_print_str(" SNR ");
						pkt_print_packet((rf_packet*)item);
					#endif	
					
				} else { //	Yes resend
					item->sent_count++;
					#ifdef PKT_DEBUG_HIGH
						serial_print_str("SC ");
						serial_print_int(pkt_tx_queue[count].sent_count);
						serial_print_spc();
					#endif	
					
					// clear direct send, next one goes to everyone!
					if (item->packet.d.r1_addr == 0xffff) {
						item->packet.d.r1_addr = 0;
					}	
				} // [Yes resend]		
			} // [Okay to send it]	
		}  // [Got something to send]
	} // [Search for a packet]
} // 269 + 12
	
	
void pkt_init(uns16 my_addr, uns16 last_sent_id) {
	// go through tx queue and set all flags to DELETED	
uns8 count;
uns16 current_tick;

	pkt_my_addr = my_addr;	// store my address
	pkt_my_next_pkt_id = last_sent_id + 1;	// get next packet id
	for (count = 0; count < PKT_TX_QUEUE_SIZE; count++) {
		pkt_tx_queue[count].flag = PKT_FLAG_DELETED;  // deleted
	}	

	for (count = 0; count < PKT_SEEN_LIST_SIZE; count++ ) {
		pkt_seen_list[count].source_addr = 0xffff;	
	}	
}	

	
uns8 pkt_send_payload(uns16 dest_addr, uns8 *payload, uns8 resend) {

rf_packet my_packet;
uns8 count;

	#ifdef PKT_DEBUG_HIGH
	serial_print_str("\npkt send d:");
	serial_print_int(dest_addr);
	serial_print_str("s: ");
	serial_print_int(pkt_my_addr);
	serial_print_str("\n");
	#endif
	
	// build packet
	
	my_packet.d.source_addr = pkt_my_addr;
	my_packet.d.pkt_id = pkt_my_next_pkt_id++;
    my_packet.d.dest_addr = dest_addr;
    my_packet.d.r1_addr = 0;
    if ((dest_addr != PKT_BROADCAST_ADDR) &&
         ((payload[0] != 0xff) || (payload[1] != 0xff))) {	// if not broadcast send
		my_packet.d.r1_addr = PKT_DIRECT_SEND_ADDR; // for direct send
	}
    my_packet.d.r2_addr = 0;
    my_packet.d.r3_addr = 0;
    for (count = 0; count < PKT_PAYLOAD_SIZE; count++) {
		my_packet.d.payload[count] = payload[count];
	}	
    pkt_calc_check_byte(&my_packet);
    
	#ifdef PKT_DEBUG
		serial_print_str(" PS");
		pkt_print_packet(&my_packet);
	#endif
	
	// Put it in the queue for sending
    
    return pkt_queue_packet(&my_packet, resend);
}    
	



