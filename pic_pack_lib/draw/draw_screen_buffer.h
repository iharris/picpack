#ifndef __draw_screen_buffer_h
#define __draw_screen_buffer_h



extern uns8 draw_buffer0[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#if DRAW_PIXELS_HIGH > 1
extern uns8 draw_buffer1[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 2
extern uns8 draw_buffer2[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 3
extern uns8 draw_buffer3[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 4
extern uns8 draw_buffer4[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 5
extern uns8 draw_buffer5[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 6
extern uns8 draw_buffer6[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 7
extern uns8 draw_buffer7[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 8
extern uns8 draw_buffer8[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 9
extern uns8 draw_buffer9[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 10
extern uns8 draw_buffer10[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 11
extern uns8 draw_buffer11[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 12
extern uns8 draw_buffer12[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 13
extern uns8 draw_buffer13[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 14
extern uns8 draw_buffer14[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 15
extern uns8 draw_buffer15[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 16
extern uns8 draw_buffer16[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 17
extern uns8 draw_buffer17[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 18
extern uns8 draw_buffer18[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 19
extern uns8 draw_buffer19[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 20
extern uns8 draw_buffer20[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 21
extern uns8 draw_buffer21[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 22
extern uns8 draw_buffer22[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 23
extern uns8 draw_buffer23[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 24
extern uns8 draw_buffer24[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 25
extern uns8 draw_buffer25[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 26
extern uns8 draw_buffer26[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 27
extern uns8 draw_buffer27[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 28
extern uns8 draw_buffer28[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 29
extern uns8 draw_buffer29[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 30
extern uns8 draw_buffer30[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif
#if DRAW_PIXELS_HIGH > 31
extern uns8 draw_buffer31[DRAW_PIXELS_WIDE / DRAW_BITS_PER_PIXEL];
#endif

extern uns8 *draw_buffers[DRAW_PIXELS_HIGH] = {
	&draw_buffer0,
#if DRAW_PIXELS_HIGH > 1
	&draw_buffer1,
#endif
#if DRAW_PIXELS_HIGH > 2
	&draw_buffer2,
#endif
#if DRAW_PIXELS_HIGH > 3
	&draw_buffer3,
#endif
#if DRAW_PIXELS_HIGH > 4
	&draw_buffer4,
#endif
#if DRAW_PIXELS_HIGH > 5
	&draw_buffer5,
#endif
#if DRAW_PIXELS_HIGH > 6
	&draw_buffer6,
#endif
#if DRAW_PIXELS_HIGH > 7
	&draw_buffer7,
#endif
#if DRAW_PIXELS_HIGH > 8
	&draw_buffer8,
#endif
#if DRAW_PIXELS_HIGH > 9
	&draw_buffer9,
#endif
#if DRAW_PIXELS_HIGH > 10
	&draw_buffer10,
#endif
#if DRAW_PIXELS_HIGH > 11
	&draw_buffer11,
#endif
#if DRAW_PIXELS_HIGH > 12
	&draw_buffer12,
#endif
#if DRAW_PIXELS_HIGH > 13
	&draw_buffer13,
#endif
#if DRAW_PIXELS_HIGH > 14
	&draw_buffer14,
#endif
#if DRAW_PIXELS_HIGH > 15
	&draw_buffer15,
#endif
#if DRAW_PIXELS_HIGH > 16
	&draw_buffer16,
#endif
#if DRAW_PIXELS_HIGH > 17
	&draw_buffer17,
#endif
#if DRAW_PIXELS_HIGH > 18
	&draw_buffer18,
#endif
#if DRAW_PIXELS_HIGH > 19
	&draw_buffer19,
#endif
#if DRAW_PIXELS_HIGH > 20
	&draw_buffer20,
#endif
#if DRAW_PIXELS_HIGH > 21
	&draw_buffer21,
#endif
#if DRAW_PIXELS_HIGH > 22
	&draw_buffer22,
#endif
#if DRAW_PIXELS_HIGH > 23
	&draw_buffer23,
#endif
#if DRAW_PIXELS_HIGH > 24
	&draw_buffer24,
#endif
#if DRAW_PIXELS_HIGH > 25
	&draw_buffer25,
#endif
#if DRAW_PIXELS_HIGH > 26
	&draw_buffer26,
#endif
#if DRAW_PIXELS_HIGH > 27
	&draw_buffer27,
#endif
#if DRAW_PIXELS_HIGH > 28
	&draw_buffer28,
#endif
#if DRAW_PIXELS_HIGH > 29
	&draw_buffer29,
#endif
#if DRAW_PIXELS_HIGH > 30
	&draw_buffer30,
#endif
#if DRAW_PIXELS_HIGH > 31
	&draw_buffer31,
#endif

};
#endif