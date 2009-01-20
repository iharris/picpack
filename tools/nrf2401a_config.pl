#/usr/bin/perl
# Calculate hex string of nrf2401a config
# Rather than do this all in code
#
#
# Change to your config, then pass the string to the pic_rf_quick_init function
#

use strict;

my $payload_width_ch2 = 0;
my $payload_width_ch1 = 168;
my $address_ch2_0 = 0xf0;
my $address_ch2_1 = 0xf0;
my $address_ch2_2 = 0xf0;
my $address_ch2_3 = 0xf0;
my $address_ch2_4 = 0xf0;

my $address_ch1_0 = 0b11100111; # addr starts here
my $address_ch1_1 = 0b11100111;
my $address_ch1_2 = 0b11100111;
my $address_ch1_3 = 0b11100111;
my $address_ch1_4 = 0b11100111;
my $address_width = 24; # (6 bits valid)
my $options_long_crc = 1;
my $options_enable_crc = 1;
my $options_enable_ch2 = 0;
my $options_enable_shockburst = 1;
my $options_enable_1_mbps = 1;
my $options_crystal = 0b011;	# 3 bits valid, 011=16Mhz
my $options_output_power = 0b11; # 2 bits valid
my $options_enable_receive = 1;
my $channel = 2;	# 7 bits valid

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
# Only change above this line
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

my $conf = "";

$conf .= hexval($payload_width_ch2);
$conf .= hexval($payload_width_ch1);

$conf .= hexval($address_ch2_0);
$conf .= hexval($address_ch2_1);
$conf .= hexval($address_ch2_2);
$conf .= hexval($address_ch2_3);
$conf .= hexval($address_ch2_4);

$conf .= hexval($address_ch1_0);
$conf .= hexval($address_ch1_1);
$conf .= hexval($address_ch1_2);
$conf .= hexval($address_ch1_3);
$conf .= hexval($address_ch1_4);


my $temp = $address_width << 2;

if ($options_long_crc == 1) {
	$temp |= 0b00000010;
}
if ($options_enable_crc == 1) {
	$temp |= 0b00000001;
}

$conf .= hexval($temp);

$temp = 0;

if ($options_enable_ch2 == 1) {
	$temp |= 0b10000000;
}
if ($options_enable_shockburst == 1) {
	$temp |= 0b01000000;
}
if ($options_enable_1_mbps == 1) {
	$temp |= 0b00100000;
}
$temp |= ($options_crystal & 0b00000111) << 2;
$temp |= ($options_output_power & 0b00000011);

$conf .= hexval($temp);
	
    	
$temp = $channel << 1;
#rf_current_channel = my_config->channel;

$temp |= ($options_enable_receive & 0b00000001); 
#rf_current_mode_receive = options.ENABLE_RECEIVE;
	
$conf .= hexval($temp);

print $conf . "\n";

sub hexval {
 return sprintf("\\x%.2x", @_[0]);
}