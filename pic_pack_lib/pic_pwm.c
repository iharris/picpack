// pic_pwm.c
// pwm routines for your pic

#include "pic_pwm.h"
#include "config.h"

uns8 pwm_count;
uns8 pwm_level[PWM_NUM_PINS];


void pwm_set_level(uns8 pwm_item, uns8 level) {
	
	pwm_level[pwm_item] = level;
}

uns8 pwm_get_level(uns8 pwm_item) {
	return pwm_level[pwm_item];
}	

void pwm_set_transition(uns8 pwm_item, uns8 to_level, uns16 steps) {
}	


void pwm_setup() {

	make_output(PWM_PORT_0, PWM_PIN_0);
	#if PWM_NUM_PINS > 1
		make_output(PWM_PORT_1, PWM_PIN_1);
	#endif
	#if PWM_NUM_PINS > 2
		make_output(PWM_PORT_2, PWM_PIN_2);
	#endif

}	

void pwm_handle() {
	uns8 count;
	
	pwm_count++;
	
	if (pwm_count == 0) {	// Turn them all on
		set_pin(PWM_PORT_0, PWM_PIN_0);
		#if PWM_NUM_PINS > 1
			set_pin(PWM_PORT_1, PWM_PIN_1);
		#endif
		#if PWM_NUM_PINS > 2
			set_pin(PWM_PORT_2, PWM_PIN_2);
		#endif
	}
	if (pwm_level[0] == pwm_count) {
		clear_pin(PWM_PORT_0, PWM_PIN_0);
	}	
	#if PWM_NUM_PINS > 1
		if (pwm_level[1] == pwm_count) {
			clear_pin(PWM_PORT_1, PWM_PIN_1);
		}
	#endif		
	#if PWM_NUM_PINS > 2
		if (pwm_level[2] == pwm_count) {
			clear_pin(PWM_PORT_2, PWM_PIN_2);
		}
	#endif		
}
