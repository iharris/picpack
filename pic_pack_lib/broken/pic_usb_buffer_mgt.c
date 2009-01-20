/*! \file pic_usb_buffer_mgt.c
    \brief Pic USB buffer routines

	Declarations of the buffer data structures for USB transfers
	
	Ian Harris 2008
	imharris [at] gmail.com

	Released under the "do whatever you like with this
	but if it breaks, you get to keep both pieces" license.
    
   
*/

#include "pic_usb_buffer_mgt.h"

buffer_descriptor bd0out@0x400;
buffer_descriptor bd0in @0x404;
buffer_descriptor bd1out@0x408;
buffer_descriptor bd1in @0x40C;
buffer_descriptor bd2out@0x410;
buffer_descriptor bd2in @0x414;
buffer_descriptor bd3out@0x418;
buffer_descriptor bd3in @0x41c;
buffer_descriptor bd4out@0x420;
buffer_descriptor bd4in @0x424;
buffer_descriptor bd5out@0x428;
buffer_descriptor bd5in @0x42C;
buffer_descriptor bd6out@0x430;
buffer_descriptor bd6in @0x434;
buffer_descriptor bd7out@0x438;
buffer_descriptor bd7in @0x43C;

uns8 buffer_0_out[USB_EP0_OUT_SIZE]@ USB_EP0_OUT_ADDR;
uns8 buffer_0_in [USB_EP0_IN_SIZE] @ USB_EP0_IN_ADDR;

#ifdef USB_EP1_IN_SIZE
	uns8 buffer_1_in [USB_EP1_IN_SIZE] @USB_EP1_IN_ADDR;
#endif
#ifdef USB_EP1_OUT_SIZE
	uns8 buffer_1_out [USB_EP1_OUT_SIZE] @ USB_EP1_OUT_ADDR;
#endif
#ifdef USB_EP2_IN_SIZE
	uns8 buffer_2_in [USB_EP2_IN_SIZE] @ USB_EP2_IN_ADDR;
#endif
#ifdef USB_EP2_OUT_SIZE
	uns8 buffer_2_out [USB_EP2_OUT_SIZE] @ USB_EP2_OUT_ADDR;
#endif
#ifdef USB_EP3_IN_SIZE
	uns8 buffer_3_in [USB_EP3_IN_SIZE] @ USB_EP3_IN_ADDR;
#endif
#ifdef USB_EP3_OUT_SIZE
	uns8 buffer_3_out [USB_EP3_OUT_SIZE] @ USB_EP3_OUT_ADDR;
#endif

buffer_descriptor * ep_in_bd_location[USB_HIGHEST_EP + 1];
buffer_descriptor * ep_out_bd_location[USB_HIGHEST_EP + 1];

uns8 * ep_in_buffer_location[USB_HIGHEST_EP + 1] = {
	// we always have EP0
	
	&buffer_0_in,	// ep 0 in buffer location
	
	#if USB_HIGHEST_EP >= 1
		#ifdef USB_EP1_IN_SIZE
			&buffer_1_in,	// ep 1 in buffer location
		#else
			0,
		#endif
	#endif
		
	#if USB_HIGHEST_EP >= 2
		#ifdef USB_EP2_IN_SIZE
			&buffer_2_in,	// ep 2 in buffer location
		#else
			0,
		#endif
	#endif
	
	#if USB_HIGHEST_EP >= 3
		#ifdef USB_EP3_IN_SIZE
			&buffer_3_in,	// ep 3 in buffer location
		#else
			0,
		#endif
	#endif
		
	#if USB_HIGHEST_EP >= 4
		#ifdef USB_EP4_IN_SIZE
			&buffer_4_in,	// ep 4 in buffer location
		#else
			0,
		#endif
	#endif	
};	   

uns8 * ep_out_buffer_location[USB_HIGHEST_EP + 1] = {
	// we always have EP0
	
	&buffer_0_out,	// ep 0 in buffer location
	
	#if USB_HIGHEST_EP >= 1
		#ifdef USB_EP1_OUT_SIZE
			&buffer_1_out,	// ep 1 in buffer location
		#else
			0,
		#endif
	#endif
		
	#if USB_HIGHEST_EP >= 2
		#ifdef USB_EP2_OUT_SIZE
			&buffer_2_out,	// ep 2 in buffer location
		#else
			0,
		#endif
	#endif
	
	#if USB_HIGHEST_EP >= 3
		#ifdef USB_EP3_OUT_SIZE
			&buffer_3_out,	// ep 3 in buffer location
		#else
			0,
		#endif
	#endif
		
	#if USB_HIGHEST_EP >= 4
		#ifdef USB_EP4_OUT_SIZE
			&buffer_4_out,	// ep 4 in buffer location
		#else
			0,
		#endif
	#endif	
};	   


uns16 ep_in_buffer_size[USB_HIGHEST_EP + 1] = {
	USB_EP0_IN_SIZE,
	
	#if USB_HIGHEST_EP >= 1
		#ifdef USB_EP1_IN_SIZE
			USB_EP1_IN_SIZE,
		#else
			0,
		#endif
	#endif		

	#if USB_HIGHEST_EP >= 2
		#ifdef USB_EP2_IN_SIZE
			USB_EP2_IN_SIZE,
		#else
			0,
		#endif
	#endif

	#if USB_HIGHEST_EP >= 3
		#ifdef USB_EP3_IN_SIZE
			USB_EP3_IN_SIZE,
		#else
			0,
		#endif
	#endif
	
	#if USB_HIGHEST_EP >= 4
		#ifdef USB_EP4_IN_SIZE
			USB_EP4_IN_SIZE,
		#else
			0,
		#endif
	#endif		
};	



uns16 ep_out_buffer_size[USB_HIGHEST_EP + 1] = {
	USB_EP0_OUT_SIZE,
	
	#if USB_HIGHEST_EP >= 1
		#ifdef USB_EP1_OUT_SIZE
			USB_EP1_OUT_SIZE,
		#else
			0,
		#endif
	#endif		

	#if USB_HIGHEST_EP >= 2
		#ifdef USB_EP2_OUT_SIZE
			USB_EP2_OUT_SIZE,
		#else
			0,
		#endif
	#endif

	#if USB_HIGHEST_EP >= 3
		#ifdef USB_EP3_OUT_SIZE
			USB_EP3_OUT_SIZE,
		#else
			0,
		#endif
	#endif
	
	#if USB_HIGHEST_EP >= 4
		#ifdef USB_EP4_OUT_SIZE
			USB_EP4_OUT_SIZE,
		#else
			0,
		#endif
	#endif		
};	
