#ifndef __pic_pwm_h
#define __pic_pwm_h

// pic_pwm
// PWM routines for your pic

#include "config.h"


#include "pic_utils.h"


// For config.h
/*

// PWM config details
// - - - - - - - - - - - - - -
#define PWM_NUM_PINS 3

#define PWM_PORT_0 PORTB
#define PWM_PIN_0 1

#define PWM_PORT_1 PORTB
#define PWM_PIN_1 4

#define PWM_PORT_2 PORTB
#define PWM_PIN_2 6


// - - - - - - - - - - - - - 

*/

extern uns8 pwm_count;

extern uns8 pwm_level[PWM_NUM_PINS];

// Handle PWM interrupt
void pwm_handle();

// Set the level of a particular pin (8 bit value)
void pwm_set_level(uns8 pwm_item, uns8 level);

// Retrieve the level of a particular pin (8 bit value)
uns8 pwm_get_level(uns8 pwm_item);

// Setup PWM pins as outputs
void pwm_setup();

// Transition to another level over time
void pwm_set_transition(uns8 to_level, uns16 steps);

#endif