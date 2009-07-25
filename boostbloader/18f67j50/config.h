#ifndef __CONFIG_H
#define __CONFIG_H
#include "platform.h"
#include "..\chipdefs.h"

#define PLATFORM_TYPE OLIMEX_BOARD
#define PLATFORM_CLOCK 20000000

#ifdef pic_mem_128k
	#define ADDR_TYPE uns32
#else
	#define ADDR_TYPE uns16
#endif

// Light led while bootloading
// #define BLOADER_USE_LED_INDICATOR

// - - - - - - - - - - - - - - - - - - - - 
// pic_flash defines
// - - - - - - - - - - - - - - - - - - - - 

#define FLASH_MAX_WRITE_CHUNK max_write_chunk
#define FLASH_MEM_ADDR_TYPE   ADDR_TYPE

#endif