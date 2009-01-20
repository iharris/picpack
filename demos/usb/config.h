#ifndef __config_h
#define __config_h
#include "platform.h"
#include "system.h"

// - - - - - - - - - - - - - - - - - - - - 
// pic_serial defines
// - - - - - - - - - - - - - - - - - - - - 

#define SERIAL_TX_BUFFER_SIZE 255
#define SERIAL_RX_BUFFER_SIZE 4
//#define SERIAL_DEBUG_ON

// - - - - - - - - - - - - - - -
// General platform definitions
// - - - - - - - - - - - - - - -
#define PLATFORM_TYPE TECH_TOYS_PIC18F4550
#define PLATFORM_CLOCK 16000000


//
#define JOY_PORT	PORTB
#define UP_PIN		7
#define DOWN_PIN	5
#define CENTER_PIN	2
#define LEFT_PIN	6
#define RIGHT_PIN	4


#endif