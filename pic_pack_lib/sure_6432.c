#include "sure_6432.h"
#include "pic_serial.h"

void sure_6432_setup_io() {
	
	
	make_output(sure_6432_en_port, sure_6432_en_pin);
	make_output(sure_6432_r1_port, sure_6432_r1_pin);
	make_output(sure_6432_r2_port, sure_6432_r2_pin);
	make_output(sure_6432_g1_port, sure_6432_g1_pin);
	make_output(sure_6432_g2_port, sure_6432_g2_pin);

	make_output(sure_6432_a_port, sure_6432_a_pin);
	make_output(sure_6432_b_port, sure_6432_b_pin);
	make_output(sure_6432_c_port, sure_6432_c_pin);
	make_output(sure_6432_d_port, sure_6432_d_pin);
	
	make_output(sure_6432_s_port, sure_6432_s_pin);
	make_output(sure_6432_l_port, sure_6432_l_pin);

	clear_pin(sure_6432_l_port, sure_6432_l_pin);
	clear_pin(sure_6432_s_port, sure_6432_s_pin);
	set_pin  (sure_6432_en_port, sure_6432_en_pin);  

}

void sure_6432_init() {
	
}

