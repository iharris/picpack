#ifndef __usb_cdc_class_h
#define __usb_cdc_class_h

void usb_cdc_putc(uns8 c);
uns8 usb_cdc_getc();
void usb_cdc_handle_tx();
uns8 usb_cdc_rx_avail();
uns8 usb_cdc_tx_empty();
void usb_cdc_print_str(char *str);
void usb_cdc_print_int(uns16 i);
void usb_cdc_setup();

#endif