#ifdef _PIC16F88
    #warning "Compiling for PIC16F88"
    #define pic_mem_4k
    #define pic_16
    #define erase_before_write
    #define min_erase_chunk 32
    #define max_write_chunk	4
    #define BLOADER_START 0x0ebc
    #define BLOADER_START_HIGH 0x0e
    #define BLOADER_START_LOW  0xbc
    #warning "Did you set -rb 3772 in Settings | Options | Linker?"
    #define MOVED_BVECTOR_START 0x0e98
    #define MOVED_BVECTOR_HIGH  0x0e
    #define MOVED_BVECTOR_LOW   0x98
    #define JUMP_TO_BVECTOR 0x2e98
    #pragma DATA 0x000, 0x158A // BSF PCLATH, 3
    #pragma DATA 0x001, 0x160A // BSF PCLATH, 4
    #pragma DATA 0x002, 0x2ebc // Goto bloaderstart
    #define SERIAL_TRIS_SETUP() trisb = 0b00100100;  // trisb 5,2 = 1
#endif



#ifdef _PIC16F876A
    #warning "Compiling for PIC16F876A"
    #define pic_mem_8k
    #define pic_16
    #define max_write_chunk	4
    #define BLOADER_START 0x1f04
    #define BLOADER_START_HIGH 0x1f
    #define BLOADER_START_LOW  0x04
    #warning "Did you set -rb 7940 in Settings | Options | Linker?"
    #define MOVED_BVECTOR_START 0x1efc
    #define MOVED_BVECTOR_HIGH  0x1e
    #define MOVED_BVECTOR_LOW   0xfc
    #define JUMP_TO_BVECTOR 0x2efc
    #pragma DATA 0x000, 0x158A // BSF PCLATH, 3
    #pragma DATA 0x001, 0x160A // BSF PCLATH, 4
    #pragma DATA 0x002, 0x2f04 // Goto bloaderstart
    #define SERIAL_TRIS_SETUP() trisc = 0b11000000;  // trisc 7,6 = 1
#endif



#ifdef _PIC16F877A
    #warning "Compiling for PIC16F877A"
    #define pic_mem_8k
    #define pic_16
    #define max_write_chunk	4
    #define BLOADER_START 0x1f04
    #define BLOADER_START_HIGH 0x1f
    #define BLOADER_START_LOW  0x04
    #warning "Did you set -rb 7940 in Settings | Options | Linker?"
    #define MOVED_BVECTOR_START 0x1efc
    #define MOVED_BVECTOR_HIGH  0x1e
    #define MOVED_BVECTOR_LOW   0xfc
    #define JUMP_TO_BVECTOR 0x2efc
    #pragma DATA 0x000, 0x158A // BSF PCLATH, 3
    #pragma DATA 0x001, 0x160A // BSF PCLATH, 4
    #pragma DATA 0x002, 0x2f04 // Goto bloaderstart
    #define SERIAL_TRIS_SETUP() trisc = 0b11000000;  // trisc 7,6 = 1
#endif



#ifdef _PIC18F2620
    #warning "Compiling for PIC18F2620"
    #define pic_mem_64k
    #define pic_18
    #define erase_before_write
    #define min_erase_chunk 64
    #define max_write_chunk	64
    #define BLOADER_START 0xfdf4
    #define BLOADER_START_HIGH 0xfd
    #define BLOADER_START_LOW  0xf4
    #warning "Did you set -rb 65012 in Settings | Options | Linker?"
    #define MOVED_BVECTOR_START 0xfdb8
    #define MOVED_BVECTOR_HIGH  0xfd
    #define MOVED_BVECTOR_LOW   0xb8
    #define JUMP_TO_BVECTOR1 0xefdc
    #define JUMP_TO_BVECTOR2 0xf07e
    #pragma DATA 0x0000,  0xfa, 0xEF, 0x7e, 0xf0
    #define JUMP_TO_BLOADER1 0xef
    #define JUMP_TO_BLOADER2 0xfa
    #define JUMP_TO_BLOADER3 0xf0
    #define JUMP_TO_BLOADER4 0x7e
    #define SERIAL_TRIS_SETUP() trisc = 0b11000000;  // trisc 7,6 = 1
#endif



#ifdef _PIC18F4520
    #warning "Compiling for PIC18F4520"
    #define pic_mem_32k
    #define pic_18
    #define erase_before_write
    #define min_erase_chunk 64
    #define max_write_chunk	32
    #define BLOADER_START 0x7df4
    #define BLOADER_START_HIGH 0x7d
    #define BLOADER_START_LOW  0xf4
    #warning "Did you set -rb 32244 in Settings | Options | Linker?"
    #define MOVED_BVECTOR_START 0x7db8
    #define MOVED_BVECTOR_HIGH  0x7d
    #define MOVED_BVECTOR_LOW   0xb8
    #define JUMP_TO_BVECTOR1 0xefdc
    #define JUMP_TO_BVECTOR2 0xf03e
    #pragma DATA 0x0000,  0xfa, 0xEF, 0x3e, 0xf0
    #define JUMP_TO_BLOADER1 0xef
    #define JUMP_TO_BLOADER2 0xfa
    #define JUMP_TO_BLOADER3 0xf0
    #define JUMP_TO_BLOADER4 0x3e
    #define SERIAL_TRIS_SETUP() trisc = 0b11000000;  // trisc 7,6 = 1
#endif



#ifdef _PIC18F4550
    #warning "Compiling for PIC18F4550"
    #define pic_mem_32k
    #define pic_18
    #define erase_before_write
    #define min_erase_chunk 64
    #define max_write_chunk	32
    #define BLOADER_START 0x7df4
    #define BLOADER_START_HIGH 0x7d
    #define BLOADER_START_LOW  0xf4
    #warning "Did you set -rb 32244 in Settings | Options | Linker?"
    #define MOVED_BVECTOR_START 0x7db8
    #define MOVED_BVECTOR_HIGH  0x7d
    #define MOVED_BVECTOR_LOW   0xb8
    #define JUMP_TO_BVECTOR1 0xefdc
    #define JUMP_TO_BVECTOR2 0xf03e
    #pragma DATA 0x0000,  0xfa, 0xEF, 0x3e, 0xf0
    #define JUMP_TO_BLOADER1 0xef
    #define JUMP_TO_BLOADER2 0xfa
    #define JUMP_TO_BLOADER3 0xf0
    #define JUMP_TO_BLOADER4 0x3e
    #define SERIAL_TRIS_SETUP() trisc = 0b11000000;  // trisc 7,6 = 1
#endif



#ifdef _PIC18F2520
    #warning "Compiling for PIC18F2520"
    #define pic_mem_32k
    #define pic_18
    #define erase_before_write
    #define min_erase_chunk 64
    #define max_write_chunk	32
    #define BLOADER_START 0x7df4
    #define BLOADER_START_HIGH 0x7d
    #define BLOADER_START_LOW  0xf4
    #warning "Did you set -rb 32244 in Settings | Options | Linker?"
    #define MOVED_BVECTOR_START 0x7db8
    #define MOVED_BVECTOR_HIGH  0x7d
    #define MOVED_BVECTOR_LOW   0xb8
    #define JUMP_TO_BVECTOR1 0xefdc
    #define JUMP_TO_BVECTOR2 0xf03e
    #pragma DATA 0x0000,  0xfa, 0xEF, 0x3e, 0xf0
    #define JUMP_TO_BLOADER1 0xef
    #define JUMP_TO_BLOADER2 0xfa
    #define JUMP_TO_BLOADER3 0xf0
    #define JUMP_TO_BLOADER4 0x3e
    #define SERIAL_TRIS_SETUP() trisc = 0b11000000;  // trisc 7,6 = 1
#endif



#ifdef _PIC18F452
    #warning "Compiling for PIC18F452"
    #define pic_mem_32k
    #define pic_18
    #define erase_before_write
    #define min_erase_chunk 64
    #define max_write_chunk	8
    #define BLOADER_START 0x7de6
    #define BLOADER_START_HIGH 0x7d
    #define BLOADER_START_LOW  0xe6
    #warning "Did you set -rb 32230 in Settings | Options | Linker?"
    #define MOVED_BVECTOR_START 0x7db8
    #define MOVED_BVECTOR_HIGH  0x7d
    #define MOVED_BVECTOR_LOW   0xb8
    #define JUMP_TO_BVECTOR1 0xefdc
    #define JUMP_TO_BVECTOR2 0xf03e
    #pragma DATA 0x0000,  0xf3, 0xEF, 0x3e, 0xf0
    #define JUMP_TO_BLOADER1 0xef
    #define JUMP_TO_BLOADER2 0xf3
    #define JUMP_TO_BLOADER3 0xf0
    #define JUMP_TO_BLOADER4 0x3e
    #define SERIAL_TRIS_SETUP() trisc = 0b11000000;  // trisc 7,6 = 1
#endif



#ifdef _PIC18F252
    #warning "Compiling for PIC18F252"
    #define pic_mem_32k
    #define pic_18
    #define erase_before_write
    #define min_erase_chunk 64
    #define max_write_chunk	8
    #define BLOADER_START 0x7de4
    #define BLOADER_START_HIGH 0x7d
    #define BLOADER_START_LOW  0xe4
    #warning "Did you set -rb 32228 in Settings | Options | Linker?"
    #define MOVED_BVECTOR_START 0x7db8
    #define MOVED_BVECTOR_HIGH  0x7d
    #define MOVED_BVECTOR_LOW   0xb8
    #define JUMP_TO_BVECTOR1 0xefdc
    #define JUMP_TO_BVECTOR2 0xf03e
    #pragma DATA 0x0000,  0xf2, 0xEF, 0x3e, 0xf0
    #define JUMP_TO_BLOADER1 0xef
    #define JUMP_TO_BLOADER2 0xf2
    #define JUMP_TO_BLOADER3 0xf0
    #define JUMP_TO_BLOADER4 0x3e
    #define SERIAL_TRIS_SETUP() trisc = 0b11000000;  // trisc 7,6 = 1
#endif



#ifdef _PIC18F67J50
    #warning "Compiling for PIC18F67J50"
    #define pic_mem_125.375k
    #define pic_18
    #define erase_before_write
    #define min_erase_chunk 1024
    #define max_write_chunk	64
    #define BLOADER_START 0x1f364
    #define BLOADER_START_HIGH 0x1f3
    #define BLOADER_START_LOW  0x64
    #warning "Did you set -rb 127844 in Settings | Options | Linker?"
#warning " Well, don't know what to do with this erase chunk (1024) for moved_bvector_start"
    #define MOVED_BVECTOR_START 0x7db8
    #define MOVED_BVECTOR_HIGH  0x7d
    #define MOVED_BVECTOR_LOW   0xb8
    #define JUMP_TO_BVECTOR1 0xefdc
    #define JUMP_TO_BVECTOR2 0xf03e
    #pragma DATA 0x0000,  0xb2, 0xEF, 0xf9, 0xf0
    #define JUMP_TO_BLOADER1 0xef
    #define JUMP_TO_BLOADER2 0xb2
    #define JUMP_TO_BLOADER3 0xf0
    #define JUMP_TO_BLOADER4 0xf9
    #define SERIAL_TRIS_SETUP() trisc = 0b10000000;  // trisc 7 = 1, 6=0
#endif



