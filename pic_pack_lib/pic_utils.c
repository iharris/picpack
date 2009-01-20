#include "pic_utils.h"


// Use shadowed ports to prevent read-before-write problems

// This is the one place the port_shadow variable actually gets declared

#if NUMBER_PORTS == 2
uns8 port_shadow[NUMBER_PORTS] = {0, 0};
#endif

#if NUMBER_PORTS == 3
uns8 port_shadow[NUMBER_PORTS] = {0, 0, 0};
#endif

#if NUMBER_PORTS == 4
uns8 port_shadow[NUMBER_PORTS] = {0, 0, 0, 0};
#endif

#if NUMBER_PORTS == 5
uns8 port_shadow[NUMBER_PORTS] = {0, 0, 0, 0, 0};
#endif


void set_pin_var(uns8 port, uns8 pin) {
	uns8 array_location = port - PORTA;
	set_bit(port_shadow[array_location], pin); 
	port_array[array_location] = port_shadow[array_location]; 
}	

void clear_pin_var(uns8 port, uns8 pin) {
	uns8 array_location = port - PORTA;
	clear_bit(port_shadow[array_location], pin);
	port_array[array_location] = port_shadow[array_location]; 
}
	
void toggle_pin_var(uns8 port, uns8 pin) {
	uns8 array_location = port - PORTA;
    port_shadow[array_location] ^= (1 << (pin));
    port_array[array_location] = port_shadow[array_location]; 
}


void change_pin_var(uns8 port, uns8 pin, bit value) {
	uns8 array_location = port - PORTA;
	if (value) {
		set_bit(port_shadow[array_location], pin); 
	} else {
		clear_bit(port_shadow[array_location], pin);
	}
    port_array[array_location] = port_shadow[array_location]; 
}

  
