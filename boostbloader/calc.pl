#!/usr/bin/perl
#
#
#
#
# Run this: 
# calc.pl > chipdefs.h
# 
#
# to regenerate chipdefs.h with new information.
#
# 5 May 2008
# Added 18f2520 thanks to Peter Lawson



             #chip, family, memory, bootloader_size, min_erase_chunk, max_write_chunk, tris setting 
print_config("PIC16F88",   "pic_16", 4096,  320, 32, 4,  "trisb = 0b00100100;  // trisb 5,2 = 1");
print_config("PIC16F876A", "pic_16", 8192,  249, 0,  4,  "trisc = 0b11000000;  // trisc 7,6 = 1");
print_config("PIC16F877A", "pic_16", 8192,  249, 0,  4,  "trisc = 0b11000000;  // trisc 7,6 = 1");
print_config("PIC18F2620", "pic_18", 65536, 520, 64, 64, "trisc = 0b11000000;  // trisc 7,6 = 1");
print_config("PIC18F4520", "pic_18", 32768, 520, 64, 32, "trisc = 0b11000000;  // trisc 7,6 = 1");
print_config("PIC18F4550", "pic_18", 32768, 520, 64, 32, "trisc = 0b11000000;  // trisc 7,6 = 1");
print_config("PIC18F2520", "pic_18", 32768, 520, 64, 32, "trisc = 0b11000000;  // trisc 7,6 = 1");
print_config("PIC18F452",  "pic_18", 32768, 534, 64, 8,  "trisc = 0b11000000;  // trisc 7,6 = 1");
print_config("PIC18F252",  "pic_18", 32768, 536, 64, 8,  "trisc = 0b11000000;  // trisc 7,6 = 1");

sub print_config {

my ($chip, $family, $memory, $bl_size, $min_erase_chunk, $max_write_chunk, $serial_tris_setup) = @_;
my ($mem_div);


if ($family == "pic_18") {
	$mem_div = 2;
} else {
	$mem_div = 1;
}	

# Allow 3 instructions grace, boostloader gets a bit confused otherwise
my ($bloader_start) = $memory - 3 - $bl_size;
if ($mem_div == 2) {
	$bloader_start = $bloader_start >> 1;
	$bloader_start = $bloader_start << 1; # Chop off this bit
}	

print "#ifdef _$chip\n";
print "    #warning \"Compiling for $chip\"\n";
print "    #define pic_mem_". $memory / 1024 . "k\n";
print "    #define $family\n";
if ($min_erase_chunk > 0) {
	print "    #define erase_before_write\n";
	print "    #define min_erase_chunk $min_erase_chunk\n";
}
printf "    #define max_write_chunk	$max_write_chunk\n";
printf "    #define BLOADER_START 0x%.4x\n", $bloader_start;
printf "    #define BLOADER_START_HIGH 0x%.2x\n", $bloader_start >> 8; 
printf "    #define BLOADER_START_LOW  0x%.2x\n", $bloader_start & 0xff;
printf "    #warning \"Did you set -rb ".$bloader_start." in Settings | Options | Linker?\"\n";
if ($family eq "pic_16") {
	if ($min_erase_chunk == 32){
		$moved_bvector_start = ($bloader_start & 0xffe0) - 8;
	} elsif ($min_erase_chunk == 0) {	
		$moved_bvector_start = $bloader_start - 8;
	} else {
		printf "!!- Well, don't know what to do with this erase chunk (%d) for moved_bvector_start\n", $min_erase_chunk;
	}
} else {
	
	#// for 18f2620, 18f4520 which has to erase on 64 byte (32 word) boundaries:
	#// MOVED_BVECTOR_START  = (BLOADER_START & 0xFFC0) - 8

	if ($min_erase_chunk == 64) {
		$moved_bvector_start = ($bloader_start & 0xffc0) - 8;
	} else {
		printf "!!- Well, don't know what to do with this erase chunk (%d) for moved_bvector_start\n", $min_erase_chunk;
	}
}

printf "    #define MOVED_BVECTOR_START 0x%.4x\n", $moved_bvector_start;
printf "    #define MOVED_BVECTOR_HIGH  0x%.2x\n", $moved_bvector_start >> 8;
printf "    #define MOVED_BVECTOR_LOW   0x%.2x\n", $moved_bvector_start & 0xff;

my ($jump_to_bvector, $jump_to_bvector1, $jump_to_bvector2);

if ($family eq "pic_16") {
	#// To Calculate: 0x2800 + (MOVED_BVECTOR_START & 0x7FF)
	$jump_to_bvector = 0x2800 + ($moved_bvector_start & 0x7ff);
	
	printf "    #define JUMP_TO_BVECTOR 0x%.4x\n", $jump_to_bvector;
	
} else {
	
	#// 18f series pics
	#// Require two instructions for jump
	
	#// To Calculate: 
	#// EF00 + ((MOVED_BVECTOR_START >> 1) & 0x00ff)
	#// F000 + ((MOVED_BVECTOR_START >> 1) & 0xff00) >> 8)
	#// Assuming no greater than 64k program space (32k words)

	$jump_to_bvector1 = 0xef00 + (($moved_bvector_start >> 1) & 0x00ff);
	$jump_to_bvector2 = 0xf000 + (($moved_bvector_start >> 1) >> 8);

	printf "    #define JUMP_TO_BVECTOR1 0x%.4x\n", $jump_to_bvector1;
	printf "    #define JUMP_TO_BVECTOR2 0x%.4x\n", $jump_to_bvector2;

}


if ($family eq "pic_16") {

	#//--#pragma cdata[0] = /*BSF*/0x1400 + /*PCLATH*/10 + (/*bit*/3 << 7)
	#//--#pragma cdata[1] = /*BSF*/0x1400 + /*PCLATH*/10 + (/*bit*/4 << 7)
	#//--#pragma cdata[2] = /*GOTO*/0x2800 + (BLOADER_START & 0x7FF)

	printf "    #pragma DATA 0x000, 0x158A // BSF PCLATH, 3\n";
	printf "    #pragma DATA 0x001, 0x160A // BSF PCLATH, 4\n";
	printf "    #pragma DATA 0x002, 0x%.4x // Goto bloaderstart\n", 0x2800 + ($bloader_start & 0x7ff);

} else { # pic_18

	#// To Calculate: 
	#// EF00 + ((BLOADER_START >> 1) & 0x00ff)
	#// F000 + ((BLOADER_START >> 1)) >> 8
	#// Goto 0x7ee0
	#// 7ee0 >> 1 = 3f70
	#// Seem to need to put these backwards
	

	printf "    #pragma DATA 0x0000,  0x%.2x, 0xEF, 0x%.2x, 0xf0\n", ($bloader_start >> 1) & 0xff, ($bloader_start >> 1) >> 8;
	printf "    #define JUMP_TO_BLOADER1 0xef\n";
	printf "    #define JUMP_TO_BLOADER2 0x%.2x\n", (($bloader_start >> 1) & 0x00ff);
	printf "    #define JUMP_TO_BLOADER3 0xf0\n";
	printf "    #define JUMP_TO_BLOADER4 0x%.2x\n", (($bloader_start >> 1) >> 8);
	
}


printf "    #define SERIAL_TRIS_SETUP() $serial_tris_setup\n";

print "#endif\n";
print "\n";
print "\n";
print "\n";

}