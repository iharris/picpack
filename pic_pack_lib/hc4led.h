#ifndef __hc4led_h
#define __hc4led_h include

#include "spi.h"
#include "pic_utils.h"

void hc4led_setup();
void hc4led_write_str(char *data);

#endif
