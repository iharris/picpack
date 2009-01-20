/*! \file pic_usb_buffer_mgt.h
    \brief Pic USB buffer routines

	Buffer data structures for USB transfers
	
	Ian Harris 2008
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if it breaks, you get to keep both pieces" license.
    
   
*/

#ifndef __pic_usb_buffer_mgt_h
#define __pic_ubs_buffer_mgt_h

#include "pic_usb.h"
#include "config.h"

extern buffer_descriptor bd0out;
extern buffer_descriptor bd0in;
extern buffer_descriptor bd1out;
extern buffer_descriptor bd1in;
extern buffer_descriptor bd2out;
extern buffer_descriptor bd2in;
extern buffer_descriptor bd3out;
extern buffer_descriptor bd3in;
extern buffer_descriptor bd4out;
extern buffer_descriptor bd4in;
extern buffer_descriptor bd5out;
extern buffer_descriptor bd5in;
extern buffer_descriptor bd6out;
extern buffer_descriptor bd6in;
extern buffer_descriptor bd7out;
extern buffer_descriptor bd7in;
 
extern uns8 buffer_0_out[USB_EP0_OUT_SIZE];
extern uns8 buffer_0_in [USB_EP0_IN_SIZE];

#ifdef USB_EP1_IN_SIZE
	extern uns8 buffer_1_in [USB_EP1_IN_SIZE];
#endif
#ifdef USB_EP1_OUT_SIZE
	extern uns8 buffer_1_out [USB_EP1_OUT_SIZE];
#endif
#ifdef USB_EP2_IN_SIZE
	extern uns8 buffer_2_in [USB_EP2_IN_SIZE];
#endif
#ifdef USB_EP2_OUT_SIZE
	extern uns8 buffer_2_out [USB_EP2_OUT_SIZE];
#endif
#ifdef USB_EP3_IN_SIZE
	extern uns8 buffer_3_in [USB_EP3_IN_SIZE];
#endif
#ifdef USB_EP3_OUT_SIZE
	extern uns8 buffer_3_out [USB_EP3_OUT_SIZE];
#endif

extern buffer_descriptor * ep_in_bd_location[USB_HIGHEST_EP + 1];
extern buffer_descriptor * ep_out_bd_location[USB_HIGHEST_EP + 1];

extern uns8 * ep_in_buffer_location[USB_HIGHEST_EP + 1];

extern uns8 * ep_out_buffer_location[USB_HIGHEST_EP + 1];

extern uns16 ep_in_buffer_size[USB_HIGHEST_EP + 1];

extern uns16 ep_out_buffer_size[USB_HIGHEST_EP + 1];

#endif

