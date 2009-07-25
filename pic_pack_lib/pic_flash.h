#ifndef __pic_flash_h
#define __pic_flash_h

#include "pic_utils.h"
#include "config.h"

/* Place this in your config.h

// - - - - - - - - - - - - - - - - - - - - 
// pic_flash defines
// - - - - - - - - - - - - - - - - - - - - 

#define FLASH_MAX_WRITE_CHUNK 16
#define FLASH_MEM_ADDR_TYPE uns16

*/


void flash_erase(FLASH_MEM_ADDR_TYPE address);
void flash_write(FLASH_MEM_ADDR_TYPE address, uns8 count, uns8 *data);

#ifndef FLASH_MAX_WRITE_CHUNK
	#error "pic_flash: FLASH_MAX_WRITE_CHUNK not defined in config.h"
#endif	
#ifndef FLASH_MEM_ADDR_TYPE
	#error "pic_flash: FLASH_MEM_ADDR_TYPE not defined in config.h"
#endif	


#endif