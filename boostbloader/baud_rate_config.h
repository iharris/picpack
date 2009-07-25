// Your set up - you need to put a PLATFORM_CLOCK def in your config.h

#if PLATFORM_CLOCK == 40000000
	#define SPBRG_9600 255
#endif

#if PLATFORM_CLOCK == 20000000
	#define SPBRG_9600 129
#endif

#if PLATFORM_CLOCK == 12000000
	#define SPBRG_9600 77
#endif	

#if PLATFORM_CLOCK == 8000000
	#define SPBRG_9600 51
#endif

#if PLATFORM_CLOCK == 4000000
	#define SPBRG_9600 25
#endif

#ifndef SPBRG_9600
	#warning "Don't know the SPBRG value for your clock speed."
	#warning "Did you set your PLATFORM_CLOCK etc in your config.h?"
#endif	
