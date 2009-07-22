/*
    Boostbloader.
    
    Based on Bloader by Sparkfun

	This version - Ian Harris imharris at gmail.com

	See changes.txt for changes

*/


#include <system.h>
#include "pic_utils.h"
#include "config.h"

#include "..\chipdefs.h"


#ifdef _PIC16
#ifdef _PIC16F88
	#pragma DATA _CONFIG, _CP_OFF & _CCP1_RB0 & _DEBUG_OFF & _WRT_PROTECT_OFF & _CPD_OFF & _LVP_OFF & _PWRTE_OFF & _WDT_OFF & _PWRTE_ON & _BODEN_OFF &_MCLR_ON & _INTRC_IO  
#endif

#ifdef _PIC16F876A
	#pragma DATA _CONFIG, _CP_OFF & _DEBUG_OFF & _WRT_OFF & _CPD_OFF & _LVP_OFF & _BODEN_OFF & _PWRTE_ON & _WDT_OFF & _HS_OSC
#endif

#endif

#ifdef _PIC18

#if defined(_PIC18F252)|| defined(_PIC18F452)
	#pragma DATA    _CONFIG1H,  _OSCS_OFF_1H & _HS_OSC_1H 
	#pragma DATA    _CONFIG2L, _BOR_OFF_2L & _BORV_20_2L & _PWRT_ON_2L
	#pragma DATA    _CONFIG2H, _WDT_OFF_2H & _WDTPS_128_2H
	#pragma DATA    _CONFIG3H, _CCP2MX_ON_3H
	#pragma DATA    _CONFIG4L, _STVR_ON_4L & _LVP_OFF_4L & _DEBUG_OFF_4L
	#pragma DATA    _CONFIG5L, _CP0_OFF_5L & _CP1_OFF_5L & _CP2_OFF_5L & _CP3_OFF_5L
	#pragma DATA    _CONFIG5H, _CPB_OFF_5H & _CPD_OFF_5H
	#pragma DATA    _CONFIG6L, _WRT0_OFF_6L & _WRT1_OFF_6L & _WRT2_OFF_6L & _WRT3_OFF_6L
	#pragma DATA    _CONFIG6H, _WRTC_OFF_6H & _WRTB_OFF_6H & _WRTD_OFF_6H
	#pragma DATA    _CONFIG7L, _EBTR0_OFF_7L & _EBTR1_OFF_7L & _EBTR2_OFF_7L & _EBTR3_OFF_7L
	#pragma DATA    _CONFIG7H, _EBTRB_OFF_7H
 #else
   #if defined(_PIC18F4550) 
 
	#pragma DATA	_CONFIG1L, _PLLDIV_5_1L & _CPUDIV_OSC4_PLL6_1L & _USBDIV_2_1L
	#pragma DATA    _CONFIG1H, _FOSC_HSPLL_HS_1H & _FCMEN_OFF_1H & _IESO_OFF_1H
	#pragma DATA    _CONFIG2L, _PWRT_ON_2L & _BOR_OFF_2L  & _VREGEN_OFF_2L
	#pragma DATA    _CONFIG2H, _WDT_OFF_2H & _WDTPS_128_2H
	#pragma DATA    _CONFIG3H, _CCP2MX_OFF_3H & _LPT1OSC_OFF_3H & _PBADEN_OFF_3H & _MCLRE_ON_3H
	#pragma DATA    _CONFIG4L, _STVREN_ON_4L & _LVP_OFF_4L & _DEBUG_OFF_4L & _XINST_OFF_4L
	#pragma DATA    _CONFIG5L, _CP0_OFF_5L & _CP1_OFF_5L & _CP2_OFF_5L & _CP3_OFF_5L
	#pragma DATA    _CONFIG5H, _CPB_OFF_5H & _CPD_OFF_5H
	#pragma DATA    _CONFIG6L, _WRT0_OFF_6L & _WRT1_OFF_6L & _WRT2_OFF_6L & _WRT3_OFF_6L
	#pragma DATA    _CONFIG6H, _WRTC_OFF_6H & _WRTB_OFF_6H & _WRTD_OFF_6H
	#pragma DATA    _CONFIG7L, _EBTR0_OFF_7L & _EBTR1_OFF_7L & _EBTR2_OFF_7L & _EBTR3_OFF_7L
	#pragma DATA    _CONFIG7H, _EBTRB_OFF_7H

   #else
   
   #if defined(_PIC18F67J50) 
	#pragma DATA	_CONFIG1L, _DEBUG_OFF_1L & _XINST_OFF_1L & _STVREN_OFF_1L & _PLLDIV_5_1L & _WDT_OFF_1L
	#pragma DATA    _CONFIG1H, _CP0_OFF_1H & _CPUDIV_OSC1__1H
	
	#pragma DATA    _CONFIG2L, _IESO_OFF_2L & _FCMEN_OFF_2L & _FOSC_HSPLL_2L
	#pragma DATA    _CONFIG2H, _WDTPS_32768_2H
	#pragma DATA    _CONFIG3L, _WAIT_OFF_3L & _BW_8_3L & _MODE_MM_3L & _EASHFT_OFF_3L
	#pragma DATA    _CONFIG3H, _MSSP_MSK7_3H & _PPMX_DEFAULT_3H & _ECCPMX_DEFAULT_3H & _CCP2MX_DEFAULT_3H
  

   #else
   
	 //#pragma DATA    _CONFIG1H, _OSC_HS_1H //_OSC_HSPLL_1H //
	   #pragma DATA    _CONFIG1H, _OSC_HS_1H & _IESO_OFF_1H
	   #pragma DATA    _CONFIG2L, _BOREN_SBORDIS_2L & _BORV_2_2L & _PWRT_ON_2L
	   #pragma DATA    _CONFIG2H, _WDT_OFF_2H & _WDTPS_128_2H
	   #pragma DATA    _CONFIG3H, _CCP2MX_PORTC_3H & _PBADEN_OFF_3H & _MCLRE_ON_3H
	   #pragma DATA    _CONFIG4L, _STVREN_ON_4L & _LVP_OFF_4L & _DEBUG_OFF_4L & _XINST_OFF_4L
	   #pragma DATA    _CONFIG5L, _CP0_OFF_5L & _CP1_OFF_5L & _CP2_OFF_5L & _CP3_OFF_5L
	   #pragma DATA    _CONFIG5H, _CPB_OFF_5H & _CPD_OFF_5H
	   #pragma DATA    _CONFIG6L, _WRT0_OFF_6L & _WRT1_OFF_6L & _WRT2_OFF_6L & _WRT3_OFF_6L
	   #pragma DATA    _CONFIG6H, _WRTC_OFF_6H & _WRTB_OFF_6H & _WRTD_OFF_6H
	   #pragma DATA    _CONFIG7L, _EBTR0_OFF_7L & _EBTR1_OFF_7L & _EBTR2_OFF_7L & _EBTR3_OFF_7L
	   #pragma DATA    _CONFIG7H, _EBTRB_OFF_7H
	#endif
 #endif
#endif
#endif

// Your set up - you need to put a PLATFORM_CLOCK def in your config.h

#if PLATFORM_CLOCK == 40000000
	#define SPBRG_9600 255
#endif

#if PLATFORM_CLOCK == 20000000
	#define SPBRG_9600 129
#endif

#if PLATFORM_CLOCK == 12000000
	#define SPBRG_9600 77
#endif	

#if PLATFORM_CLOCK == 8000000
	#define SPBRG_9600 51
#endif

#if PLATFORM_CLOCK == 4000000
	#define SPBRG_9600 25
#endif

#ifndef SPBRG_9600
	#warning "Don't know the SPBRG value for your clock speed."
	#warning "Did you set your PLATFORM_CLOCK etc in your config.h?"
#endif	

// Pre-declare functions

void putc(uns8 nate);
uns8 getc(void);
#ifdef _PIC18F67J50
void onboard_program_erase(uns8 high_program_address, uns16 program_address);
#else
void onboard_program_erase(uns16 program_address);
#endif
void onboard_program_write(void);

// Global variables

uns8 incoming_buffer[16]; //Presume no record will be longer than 16 bytes
#ifdef _PIC18F67J50
uns8 high_memory_address;
#endif
uns16 memory_address;
uns8 record_length;


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
#if PLATFORM_TYPE == SURE_PICDEM_2
	trisb = 0b00000000;
	latb =  0b00001110;	// Turn on a led on the dev board
#endif
#if PLATFORM_TYPE == OLIMEX_PIC_MT
	clear_bit(trisb, 4);
	#ifdef _PIC16
		clear_bit(portb, 4);
	#endif
	#ifdef _PIC18	
		clear_bit(latb, 4);	// Turn on a led on the dev board
	#endif
#endif
#if PLATFORM_TYPE == OLIMEX_BOARD
	trisa = 0b00000000;
	#ifdef _PIC16
		porta.0 = 1;	// Turn on a led on the dev board
	#endif
	#ifdef _PIC18
		lata.0 = 1;
	#endif	
#endif
#if PLATFORM_TYPE == SFE_TDN_V1
	trisb = 0b00000000;
	portb.1 = 1;
#endif

#if PLATFORM_TYPE == PIC_MT_BOARD
	trisb = 0b00000000;
	latb.4 = 0;
#endif
#if PLATFORM_TYPE == SFE_TDN_V1
	trisb = 0b00000000;
	portb.1 = 1;
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

    tmr1h = 0;
    tmr1l = 0; 

	set_bit( t1con, TMR1ON ); //enable timer 1

    while(!test_bit(pir1, RCIF)) // Check to see if the computer responded
        if (test_bit(pir1, TMR1IF)) break; // If timer1 rolls over, go to normal boot up
	clear_bit( t1con, TMR1ON ); // timer 1 off
    if (rcreg != 6) goto JUMP_VECTOR; // If the computer did not respond correctly with a ACK, we go into normal boot up mode

    while(!test_bit(pir1,RCIF)); // Wait for computer to transmit UART speed
    spbrg = rcreg; // Setup the hardware UART module
    
    //Short delay while computer re-adjusts the port speed under VB
    //for(i = 0 ; i < 255 ; i++)
    //    for(temp = 0 ; temp < 255 ; temp++);

    // Erase memory block -> 0 to almost the start of the boot loader
#ifdef erase_before_write
	// Erase first block
	onboard_program_erase(0x0000);
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

    memory_address = 0;
    onboard_program_write();
		
	// Now erase the rest
    for(memory_address = 0x0000+min_erase_chunk; 
        memory_address  < BLOADER_START - min_erase_chunk - 1;
        memory_address += min_erase_chunk)
			onboard_program_erase(memory_address);


#endif // erase before write

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

    memory_address = MOVED_BVECTOR_START;
    record_length = 8;
    onboard_program_write();
	#endif

    //=====================================================

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
        
        if(check_sum == 0) { // If we have a good transmission, put it in ink
			if ( memory_address == 0 ) {  // IH: If it's the boot vector
				#ifdef _PIC16
				// For pic_16, skip over the pclath instructions
				memory_address = MOVED_BVECTOR_START + 4;
				#endif
				#ifdef _PIC18
				// For pic_18, just relocate into the moved_bvector
				memory_address = MOVED_BVECTOR_START;
				#endif
            } 
                  
            onboard_program_write();
	    	
        }            

    }
JUMP_VECTOR:
#if PLATFORM_TYPE == SURE_PICDEM_2
	latb =  0b00001111;	// Turn off the led
#endif
#if PLATFORM_TYPE == OLIMEX_PIC_MT
	#ifdef _PIC16
		set_bit(portb, 4);
	#endif
	#ifdef _PIC18	
		set_bit(latb, 4);
	#endif
#endif
#if PLATFORM_TYPE == OLIMEX_BOARD
	#ifdef _PIC16
		porta.0 = 0;	// Turn off the dev board led
	#endif
	#ifdef _PIC18
		lata.0 = 0;		// Turn off the dev board led
	#endif	
#endif
#if PLATFORM_TYPE == SFE_TDN_V1
	portb.1 = 0;
#endif

    clear_bit(txsta, TXEN); //Disable transmission
    clear_bit(txsta, CREN); //Disable receiver
    
 //    IH: Now - do a jump to where the vector is, and it's in the 
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



// Erase one block of program memory

#ifdef _PIC16

void onboard_program_erase(uns16 program_address)
{

    HIBYTE(eeadrh, program_address); // set the address
    LOBYTE(eeadr, program_address); // set the address

    set_bit(eecon1, EEPGD); // point to Program data block
    #ifdef FREE
    set_bit(eecon1, WREN); // enable EE Writes
    set_bit(eecon1, FREE); // allow program memory changes
    #endif
    // ppecific Program Erase/EEPROM write steps
    eecon2 = 0x55;
    eecon2 = 0xAA;
    set_bit(eecon1, WR);

    // processor stalls and resumes after second NOP
    nop();
    nop();

    clear_bit(eecon1, WREN);
} 

#endif

#ifdef PIC18

#ifdef _PIC18F67J50

void onboard_program_erase(uns8 program_address_high, uns16 program_address)
{
	tblptru = program_address_high;
	HIBYTE(tblptrh, program_address); //Set the address
    LOBYTE(tblptrl, program_address); //Set the address

    //set_bit(eecon1, EEPGD); //Point to Flash Program data block
    //clear_bit(eecon1, CFGS); // Access Flash program memory
    
    set_bit(eecon1, WREN); //Enable EE Writes
    set_bit(eecon1, FREE); //Allow program memory changes

    //Specific Program Erase/EEPROM write steps
    eecon2 = 0x55;
    eecon2 = 0xAA;
    set_bit(eecon1, WR);

    clear_bit(eecon1, WREN);
} 

#else

void onboard_program_erase(uns16 program_address)
{
	tblptru = 0;
	HIBYTE(tblptrh, program_address); //Set the address
    LOBYTE(tblptrl, program_address); //Set the address

    set_bit(eecon1, EEPGD); //Point to Flash Program data block
    clear_bit(eecon1, CFGS); // Access Flash program memory
    set_bit(eecon1, WREN); //Enable EE Writes
    set_bit(eecon1, FREE); //Allow program memory changes

    //Specific Program Erase/EEPROM write steps
    eecon2 = 0x55;
    eecon2 = 0xAA;
    set_bit(eecon1, WR);

    clear_bit(eecon1, WREN);
} 

#endif
#endif

#ifdef _PIC16
//Write e_data to the onboard eeprom at e_address
void onboard_program_write(void)
{
    uns8 i;
    
#ifdef OSCCON
    clear_bit(eecon1, FREE);
#endif
    set_bit(eecon1, EEPGD); //Point to Program data block
    set_bit(eecon1, WREN); //Enable EE Writes

    HIBYTE(eeadrh, memory_address); //Set the address
    LOBYTE(eeadr, memory_address); //Set the address
    //EEADR = EEADR & 0b.1111.1100; //Make sure address is 0 of four bytes

    for(i = 0 ; i < record_length ; i += 2)
    {
       eedath = incoming_buffer[i]; //Give it the data
       eedata = incoming_buffer[i+1]; //Give it the data

        //Specific EEPROM write steps
        eecon2 = 0x55;
        eecon2 = 0xAA;
        set_bit(eecon1, WR);
        //Specific EEPROM write steps
    
        //Processor stalls and resumes after second NOP
        nop();
        nop();
        
        eeadr++; //Go to next buffer spot
        if (eeadr == 0) eeadrh++;
    }
    
    clear_bit(eecon1, WREN); //Disable EEPROM Writes

}  
#endif

#ifdef _PIC18

#if max_write_chunk < 16 

void write_flash() {
	set_bit  (eecon1, EEPGD); //Point to Program data block
    clear_bit(eecon1, CFGS); // Access Flash program memory
    set_bit  (eecon1, WREN); //Enable EE Writes

    //Specific EEPROM write steps
    eecon2 = 0x55;
    eecon2 = 0xAA;
    set_bit(eecon1, WR);
    //Specific EEPROM write steps
 
	// Not required for 18f2620 ??

	clear_bit(eecon1, WREN); //Disable EEPROM Writes
}

void onboard_program_write(void)
{
    uns8 i;
    
	tblptru = 0;	// Assume no more than 64k
	HIBYTE(tblptrh, memory_address); //Set the address
    LOBYTE(tblptrl, memory_address); //Set the address
	asm TBLRD*- 	// dummy decrement
    for(i = 0 ; i < record_length ; i += 2)
    {
		// seem to need to swap the bytes around
		tablat = incoming_buffer[i+1]; //Give it the data
		asm TBLWT+*
		tablat = incoming_buffer[i]; //Give it the data
		asm TBLWT+*
		if (i == max_write_chunk - 2) {
			write_flash();
		}	

    }
	write_flash();

}  

#else

#ifdef _PIC18F67J50
void onboard_program_write(void)
{
    uns8 i;
    uns8 block_count;
    
	tblptru = high_memory_address;	
	HIBYTE(tblptrh, memory_address); //Set the address
    LOBYTE(tblptrl, memory_address); //Set the address

    for(i = 0 ; i < record_length ; i += 2)
    {
		// seem to need to swap the bytes around
	   tablat = incoming_buffer[i+1]; //Give it the data
		asm TBLWT*+
	   tablat = incoming_buffer[i]; //Give it the data
		asm TBLWT*+
		

    }

	// dummy decrement
	// ensure we're still in the right 64 byte block
	asm TBLRD*-

    set_bit  (eecon1, WREN); //Enable EE Writes

    //Specific EEPROM write steps
    eecon2 = 0x55;
    eecon2 = 0xAA;
    set_bit(eecon1, WR);
    //Specific EEPROM write steps
 
	// Not required for 18f2620 ??
	clear_bit(eecon1, WREN); //Disable EEPROM Writes

}  
#else
void onboard_program_write(void)
{
    uns8 i;
    uns8 block_count;
    
	tblptru = 0;	// Assume no more than 64k
	HIBYTE(tblptrh, memory_address); //Set the address
    LOBYTE(tblptrl, memory_address); //Set the address

    for(i = 0 ; i < record_length ; i += 2)
    {
		// seem to need to swap the bytes around
	   tablat = incoming_buffer[i+1]; //Give it the data
		asm TBLWT*+
	   tablat = incoming_buffer[i]; //Give it the data
		asm TBLWT*+
		

    }

	// dummy decrement
	// ensure we're still in the right 64 byte block
	asm TBLRD*-

    set_bit  (eecon1, EEPGD); //Point to Program data block
    clear_bit(eecon1, CFGS); // Access Flash program memory
    set_bit  (eecon1, WREN); //Enable EE Writes

    //Specific EEPROM write steps
    eecon2 = 0x55;
    eecon2 = 0xAA;
    set_bit(eecon1, WR);
    //Specific EEPROM write steps
 
	// Not required for 18f2620 ??
	clear_bit(eecon1, WREN); //Disable EEPROM Writes

}  

#endif
#endif
#endif

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


