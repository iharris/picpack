/*
    Boostbloader.
    
    Based on Bloader by Sparkfun

	This version - Ian Harris imharris at gmail.com

	See changes.txt for changes

*/


#include <system.h>
#include "pic_utils.h"
#include "config.h"
#include "configbits.h"
#include "..\chipdefs.h"
#include "baud_rate_config.h"
#include "led_indicator.h"
#include "pic_flash.h"

// Pre-declare functions

void putc(uns8 nate);
uns8 getc(void);

// Global variables



uns8 incoming_buffer[16]; //Presume no record will be longer than 16 bytes
ADDR_TYPE memory_address;
uns8 record_length;

void flash_erase_all() {
    // Erase memory block -> 0 to almost the start of the boot loader
#ifdef erase_before_write
	// Erase first block
	flash_erase(0x0000);
    // The boot vector has been erased, so write it back!

	#ifdef _PIC16
    incoming_buffer[0] = 0x15; // - This sets the far jump
    incoming_buffer[1] = 0x8A;

    incoming_buffer[2] = 0x16; // - More far jump
    incoming_buffer[3] = 0x0A;

    incoming_buffer[4] = 0x2E;
    incoming_buffer[5] = BLOADER_START_LOW; 
    // IH: No longer need location 6,7 since we move the program's boot vector
	record_length = 8;
	#endif

	#ifdef _PIC18
 
    incoming_buffer[0] = JUMP_TO_BLOADER1; //0xef; // Goto bloader
    incoming_buffer[1] = JUMP_TO_BLOADER2; //0x04;

    incoming_buffer[2] = JUMP_TO_BLOADER3; //0xf0; 
    incoming_buffer[3] = JUMP_TO_BLOADER4; //0x3f;

    record_length = 4;
	#endif

    flash_write(0x0000, 8, incoming_buffer);
		
	// Now erase the rest
    for(memory_address = 0x0000+min_erase_chunk; 
        memory_address  < BLOADER_START - min_erase_chunk - 1;
        memory_address += min_erase_chunk)
			flash_erase(memory_address);


	// This is only necessary for pic 16 devices
	// Since they need to clear the pclath before jumping
	// this needs to be done since we have relocated the first 4
	// bytes of the program - normally a jump to main, startup
	// etc.  We can't rely on the compiler to put the pclath clearing
	// in it, since the compiler is assuming it's boot time, and hence
	// the pclath is already clear.
	//
	// pic 18 jumps directly, so it is not required
	//
	#ifdef _PIC16
	incoming_buffer[0] = 0x12; // - 120A  	BCF PCLATH,4
	incoming_buffer[1] = 0x0A;

	incoming_buffer[2] = 0x11; // - 118A  	BCF PCLATH,3
	incoming_buffer[3] = 0x8A;

	incoming_buffer[4] = 0x00; // - Nop to make up four instructions
	incoming_buffer[5] = 0x00;

	incoming_buffer[6] = 0x00; // - nop to make up four instructions
	incoming_buffer[7] = 0x00; 

    flash_write(MOVED_BVECTOR_START, 8, incoming_buffer);
	#endif
#endif // erase before write
}



// The PIC sends ASC(5)-enquiry out and waits for return ASC(6)-ack from computer.
// If ACK is seen, PIC goes into load mode, otherwise, returns to main 
// The PIC will transmit a 'T' and wait for a ':' to start listening

void main(void)
{

// 18f67j50

#ifdef _PIC18F67J50
	set_bit(osctune, PLLEN);
#endif

    uns8 i, temp;
    uns8 check_sum = 0;

#ifdef _PIC16F88
    osccon = 0b01110000;  // Setup internal oscillator for 8MHz
    while(osccon.2 == 0); // Wait for frequency to stabilize
#endif

#ifdef BLOADER_USE_LED_INDICATOR
	led_indicator_on();
#endif
	// This routine comes from chipdefs.h
	SERIAL_TRIS_SETUP();

#ifdef _PIC18F67J50    
    spbrg1 = SPBRG_9600;	// Automatically set for 9600 bps
    txsta1 = 0b00100100; // 8-bit asych mode, high speed uart enabled
    rcsta1 = 0b10010000; // Serial port enable, 8-bit asych continous receive mode
#else 
    spbrg = SPBRG_9600;	// Automatically set for 9600 bps
    txsta = 0b00100100; // 8-bit asych mode, high speed uart enabled
    rcsta = 0b10010000; // Serial port enable, 8-bit asych continous receive mode
#endif

    for(i = 0 ; i < 255 ; i++);

    putc(5); // PIC tells the world it can be loaded, by broadcasting an Enquiry

	// Clear timer to maximum value
	
    tmr1h = 0;
    tmr1l = 0; 

	set_bit( t1con, TMR1ON ); // enable timer 1

    while(!test_bit(pir1, RCIF)) // Check to see if the computer responded
        if (test_bit(pir1, TMR1IF)) break; // If timer1 rolls over, go to normal boot up
	
	clear_bit( t1con, TMR1ON ); // timer 1 off
    
    if (rcreg != 6) goto JUMP_VECTOR; // If the computer did not respond correctly with a ACK, we go into normal boot up mode

    // Should have provision here for 16 bit uart values, which if available, are more accurate

    while(!test_bit(pir1,RCIF)); // Wait for computer to transmit UART speed
    
    spbrg = rcreg; // Setup the hardware UART module
    
    
	flash_erase_all();	// Like it says (but also write back boot vector)
	

    // IH: used to send bloader location, now we need to send moved boot vector location
    //     since this is lower
    
    putc(MOVED_BVECTOR_HIGH); // This will tell the computer how to remap the boot vector
    putc(MOVED_BVECTOR_LOW); // This will avoid writing into restricted bloader code space

    while(1)
    {
        // Determine if the last received data was good or bad
        if (check_sum != 0) // If the check sum does not compute, tell computer to resend same line
            putc(7); // Ascii character BELL
        else            
            putc('T'); // Tell the computer that we are ready for the next line
        
        while(getc() != ':'); // Wait for the computer to initiate transfer

        record_length = getc(); // Get the length of this block

        if (record_length == 'S') goto JUMP_VECTOR; // Check to see if we are done - new boot vector

        memory_address = getc();
        memory_address <<= 8;
        memory_address += getc();
        check_sum = getc(); // Pick up the check sum for error dectection
        
        for(i = 0 ; i < record_length ; i++) // Read the program data
        {
            temp = getc();
            incoming_buffer[i] = temp;
        }
        
        for(i = 0 ; i < record_length ; i++) // Check sum calculations
            check_sum = check_sum + incoming_buffer[i];
        
        check_sum = check_sum + record_length;
        check_sum = check_sum + (memory_address >> 8);
        check_sum = check_sum + (memory_address & 0xff);

        #ifdef _PIC18
			memory_address <<= 1;
        #endif
        
        if ( check_sum == 0 ) { // If we have a good transmission, put it in ink
			if ( memory_address == 0 ) {  // If it's the boot vector
				#ifdef _PIC16
				// For pic_16, skip over the pclath instructions
				memory_address = MOVED_BVECTOR_START + 4;
				#endif
				#ifdef _PIC18
				// For pic_18, just relocate into the moved_bvector
				memory_address = MOVED_BVECTOR_START;
				#endif
            } 
                  
            flash_write(memory_address, record_length, incoming_buffer);
	    	
        }            

    }
JUMP_VECTOR:

	#ifdef BLOADER_USE_LED_INDICATOR
		led_indicator_off();
	#endif	
    
    clear_bit(txsta, TXEN); // Disable transmission
    clear_bit(txsta, CREN); // Disable receiver

    
 //	Now - do a jump to where the vector is, and it's in the 
 //        same memory page as we are, so no need to change PCLATH
 //		   for PIC16 devices
  
  #ifdef _PIC16
  asm data JUMP_TO_BVECTOR // goto MOVED_BVECTOR_START
  #endif
  
  #ifdef _PIC18
  asm data JUMP_TO_BVECTOR1  // goto MOVED_BVECTOR_START
  asm data JUMP_TO_BVECTOR2
  #endif  

}




//Sends out RS232 characters
//Speed is controller by UART setup registers
void putc(uns8 nate)
{
    while(!test_bit(pir1, TXIF));
    txreg = nate;
}

//Reads in RS232 characters
//Speed is controller by UART setup registers
uns8 getc(void)
{
    while(!test_bit(pir1, RCIF));
    return(rcreg);
}


