/*
 
 pic_serial.c
 ------------
 
 Interrupt driven fifo serial routines

 Ian Harris 2008
 imharris [at] gmail.com

 Released under the "do whatever you like with this
 but if it breaks, you get to keep both pieces" license.

*/

#include "config.h"
#include "pic_utils.h"
#include "pic_serial.h"
#include <string.h>
#include <stdlib.h>

/** Transmit fifo  */
uns8 tx_buffer[SERIAL_TX_BUFFER_SIZE];
/** Transmit fifo start point */
uns8 tx_start=0;
/** Transmit fifo end point */
uns8 tx_end=0;

/** Receive fifo */
uns8 rx_buffer[SERIAL_RX_BUFFER_SIZE];
/** Receive fifo start point */
uns8 rx_start = 0;
/** Receive fifo end point */
uns8 rx_end = 0;

#ifdef SERIAL_DEBUG
uns8 rx_soft_overflow = 0;
uns8 rx_hard_overflow = 0;
uns8 rx_framing_error = 0;
#endif


// Returns ASCII Decimal and Hex values
inline uns8 bin2Hex(uns8 x)
{
   if (x < 10) {
   	 return '0' + x;
   } else {
     return 'A' -10 +  x;
   }
}


void serial_setup(bit req_brgh, uns8 req_spbrg)
{
#ifdef _PIC16F88
    set_bit(trisb, 5);
    set_bit(trisb, 2);
	#define TRIS_SET
#endif
#ifdef _PIC16F876A
    set_bit(trisc,6);
    set_bit(trisc,7);
	#define TRIS_SET
#endif
#ifdef _PIC18F2620
    set_bit(trisc,6);
    set_bit(trisc,7);
	#define TRIS_SET
#endif
#ifdef _PIC18F4520
    set_bit(trisc,6);
    set_bit(trisc,7);
	#define TRIS_SET
#endif
#ifdef _PIC18F4550
    set_bit(trisc,6);
    set_bit(trisc,7);
	#define TRIS_SET
#endif
#ifdef _PIC18F87J50
    clear_bit(trisc,6);
    set_bit(trisc,7);
	#define TRIS_SET
#endif


#ifndef TRIS_SET
	#warning "You must set tris bits for serial use yourself, I don't know your pic"
	#warning "Please send your tris bits in so they can be included in the library"
#endif

    txsta.BRGH = req_brgh;	// set baud rate generator (high/low speed)

    spbrg = req_spbrg;	// set serial port baud rate generator value

    clear_bit(txsta, SYNC);	// turn off syncronous reception
    set_bit(rcsta, SPEN);	// enable serial port

	// Configure tx
	
	kill_interrupts();
   
    clear_bit(txsta, TX9);	// Turn off 9 bit receiption
    clear_bit(txsta, TX9D);	// Clear 9th bit data

    set_bit(txsta, TXEN);	// enable sending of serial data 

	// configure rx
	
	clear_bit(rcsta, RX9);	// disable 9 bit receiption
	clear_bit(rcsta, FERR);	// clear any framing errors

	_asm	{	// clear rcreg buffer
		MOVF	_rcreg,W
		MOVF	_rcreg,W
		MOVF	_rcreg,W
	}

	clear_bit(rcsta, CREN);	
    set_bit(rcsta, CREN);	// pulse low to clear any errors	

    set_bit(pie1, RCIE);	// enable receive interrupt
    
}




void serial_putc(uns8 c)
{
uns8 tx_next;
bit  my_store_gie;
#ifdef SERIAL_IDE_DEBUG
return;
#endif

    if ((tx_end == tx_start) &&  // Nothing in the fifo
        test_bit(pir1, TXIF)) {  // And txreg is empty
		txreg = c;               // then no need for fifo, just send straight out 
    } else { // else put it in the fifo	 
        tx_next = tx_end + 1;	// get next buffer position
		if (tx_next == SERIAL_TX_BUFFER_SIZE) {	// if we're at the end
			tx_next = 0;	// wrap to the beginning
        }
        #ifdef SERIAL_DISCARD_ON_TX_FULL_DURING_INT
			if ((!intcon.GIE) && (tx_next == tx_start)) {
				return;
			}
		#endif	
        while (tx_next == tx_start) { // wait for clearing
									 // Note, if buffer is full
									 // this will wait for ever
									 // if interrupts are disabled!
			#ifndef SERIAL_DISCARD_ON_TX_FULL_DURING_INT
				if (!intcon.GIE) {	// we're in an interrupt
					serial_handle_tx_isr();	// so handle it ourselves
				}
			#endif	
		}		
		my_store_gie = intcon.GIE;	// store interrupt state
		kill_interrupts();	// turn off global interrupts
        
        tx_buffer[tx_end] = c; // put it in
        tx_end = tx_next;  // move pointer along
		
		set_bit(pie1, TXIE); // turn on interrupt for transmitting
		intcon.GIE = my_store_gie;	// restore interrupt state
	} // -- else put it in the fifo
} 



void serial_tx_isr()	
{
uns8 tx_next;

	if (tx_end == tx_start) { // anything in the fifo?
	   return; // nope
	}
	tx_next = tx_start + 1;	// get next position
	if (tx_next == SERIAL_TX_BUFFER_SIZE) {	// if we're at the end of the buffer
		tx_next = 0;	// wrap to the beginning
	}
	if (tx_end == tx_next) {	// if we've only got one character to send
		clear_bit(pie1, TXIE);	// then turn off interrupts
	}		
	txreg = tx_buffer[tx_start];	// transmit the character
	tx_start = tx_next;	// move start position of fifo

}	/* \ \  */


void serial_rx_isr() 
{
   uns8 rx_next;
	

	if (test_bit(rcsta, OERR)) {	// overrun error?
		clear_bit(rcsta, CREN);	// clear error
		_asm	{	
			MOVF	_rcreg,W	// clear any received characters
			MOVF	_rcreg,W
			MOVF	_rcreg,W
		}
		#ifdef SERIAL_DEBUG
		rx_hard_overflow++;	// increment error count if in debug mode
		#endif
		set_bit(rcsta, CREN);	// reset error indicator
	} else {
		if (test_bit(rcsta, FERR)) {	// framing error?
			#ifdef SERIAL_DEBUG
			rx_framing_error++;	// increment error count if in debug mode
			#endif
		}	
		rx_next = rx_end + 1;	// get next buffer position	
		if (rx_next == SERIAL_RX_BUFFER_SIZE) {	// if we're at the end
			rx_next = 0;	// then wrap to the beginning
		}
		if (rx_next != rx_start) { // if space in the fifo
			rx_buffer[rx_end] = rcreg; // put it in
			rx_end = rx_next;  // and move pointer along
		} else {	// else, there isn't space
			_asm MOVF	_rcreg,W // and just clear it, we've lost it
			#ifdef SERIAL_DEBUG
			rx_soft_overflow++;	// increment error count if in debug mode
			#endif
		}	// -- no space in the fifo
	}	// -- no overrun error
}	// -- serial_load_rx



uns8 serial_getc(void)
{
	uns8 rx_char, rx_next;
	
	while(rx_end == rx_start);	// wait until there is something received

	start_crit_sec();	// make sure nobody else can muck with the buffer
	
	rx_char = rx_buffer[rx_start];	// get character from the front of the buffer
	rx_start++;	// increment fifo start
	if (rx_start == SERIAL_RX_BUFFER_SIZE) {	// if we're at the end
		rx_start = 0;	// then wrap to the beginning
	}
    
    end_crit_sec();	// now they can muck with the buffer
    
    return (rx_char);	// return the result we first thought of

}	// -- getc    



void serial_print_str(char *str) {

uns8 count;

	for(count = 0 ; str[count] != 0; count++)
    {
        serial_putc(str[count]);
    }
}    



void serial_print_str(rom char *str) {

uns8 count;

	for(count = 0 ; str[count] != 0; count++)
    {
        serial_putc(str[count]);
    }
}    



void serial_print_int(uns16 i) {

char buffer[6];	// up to 5 characters plus \0
uns8 count = 5;
	buffer[5] = '\0';
	do {
		count--;
		buffer[count] = '0' + i % 10;
		i = i / 10;
	} while (i > 0);	
	while (buffer[count]) {
        serial_putc(buffer[count]);
        count++;
    }  
	//serial_print_str(&buffer[count]);	//	print it out 
//	for(count = 0 ; str[count] != 0; count++)
 //   {
 //   }

}	

void serial_print_int_hex(uns8 i) {

	serial_putc(bin2Hex(i >> 4));
	serial_putc(bin2Hex((i & 0x0f)));
	
}	

void serial_print_int_hex_16bit(uns16 i) {
	serial_print_int_hex(i >> 8);
	serial_print_int_hex(i & 0xff);
}	


void serial_print_spc() {
		serial_putc(' ');
}


void serial_print_nl() {
		serial_putc('\n');
}		



uns8 serial_rx_avail() { return rx_start != rx_end; }
uns8 serial_tx_empty() { return tx_start == tx_end; }

