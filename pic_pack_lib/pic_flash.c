#include "pic_flash.h"

#ifdef _PIC16
	
void flash_erase(FLASH_MEM_ADDR_TYPE program_address)
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


void flash_write(FLASH_MEM_ADDR_TYPE address, uns8 count, uns8 *data)
{
    uns8 i;
    
// was -- #ifdef OSCCON
#ifdef FREE
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

void flash_erase(FLASH_MEM_ADDR_TYPE program_address)
{
	tblptru = 0;
	HIBYTE(tblptrh, program_address); //Set the address
    LOBYTE(tblptrl, program_address); //Set the address

    #ifdef EEPGD
		set_bit(eecon1, EEPGD); //Point to Flash Program data block
		clear_bit(eecon1, CFGS); // Access Flash program memory
    #endif
    set_bit(eecon1, WREN); //Enable EE Writes
    set_bit(eecon1, FREE); //Allow program memory changes

    //Specific Program Erase/EEPROM write steps
    eecon2 = 0x55;
    eecon2 = 0xAA;
    set_bit(eecon1, WR);

    clear_bit(eecon1, WREN);
} 



#ifdef _PIC18F67J50

void program_flash_two_bytes() {
	set_bit  (eecon1, WPROG);	// Program 2 bytes at a time
    set_bit  (eecon1, WREN);	// Enable EE Writes

    //Specific EEPROM write steps
    eecon2 = 0x55;
    eecon2 = 0xAA;
    set_bit(eecon1, WR);
    //Specific EEPROM write steps
 
	clear_bit(eecon1, WREN); //Disable EEPROM Writes

}

void flash_write(FLASH_MEM_ADDR_TYPE address, uns8 count, uns8 *data)

{
    uns8 i;
    uns8 block_count;
    
	tblptru = 0;	// change here for 128k	
	HIBYTE(tblptrh, address); //Set the address
    LOBYTE(tblptrl, address); //Set the address

	asm TBLRD*- 	// dummy decrement

    for(i = 0 ; i < count ; i += 2)
    {
		// seem to need to swap the bytes around
		tablat = data[i+1]; //Give it the data
		asm TBLWT*+
		tablat = data[i]; //Give it the data
		asm TBLWT*+
		program_flash_two_bytes();	// flash every two bytes
    }

}  


#else

#if FLASH_MAX_WRITE_CHUNK < 16 

void program_flash() {
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

void flash_write(FLASH_MEM_ADDR_TYPE address, uns8 count, uns8 *data)
{
    uns8 i;
    
	tblptru = 0;	// Assume no more than 64k
	HIBYTE(tblptrh, address); //Set the address
    LOBYTE(tblptrl, address); //Set the address
	asm TBLRD*- 	// dummy decrement
    for(i = 0 ; i < count ; i += 2)
    {
		// seem to need to swap the bytes around
		tablat = data[i+1]; //Give it the data
		asm TBLWT+*
		tablat = data[i]; //Give it the data
		asm TBLWT+*
		if (i == max_write_chunk - 2) {
			program_flash();
		}	

    }
	program_flash();

}  

#else

void flash_write(FLASH_MEM_ADDR_TYPE address, uns8 count, uns8 *data)

{
    uns8 i;
    uns8 block_count;
    if sizeof(FLASH_MEM_ADDR_TYPE) > 2 - - - 
	tblptru = 0;	// Assume no more than 64k
	HIBYTE(tblptrh, address); //Set the address
    LOBYTE(tblptrl, address); //Set the address

    for(i = 0 ; i < count ; i += 2)
    {
		// seem to need to swap the bytes around
	   tablat = data[i+1]; //Give it the data
		asm TBLWT*+
	   tablat = data[i]; //Give it the data
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
