#ifndef __config_h
#define __config_h

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 16
#define SERIAL_RX_BUFFER_SIZE 4
//#define SERIAL_DEBUG_ON
#define __DEBUG__
// - - - - - - - - - - - - - - -
// General platform definitions
#define PLATFORM_TYPE breadboard
#define PLATFORM_CLOCK 12000000

// Type = breadboard
// you're on your own!

// Type = SFE_TDN_V1
// Sparkfun transceiver development node v1

// Type = SURE_PIC_DEM_2
// Sure electronics pic dem 2 board

//#include "pic_platforms.h"

#endif