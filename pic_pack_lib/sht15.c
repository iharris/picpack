#include "sht15.h"
#include "pic_utils.h"
#include "pic_serial.h"

#define sht15_write_sda() 	clear_bit(tris_array[sht15_sda_port - PORTA], sht15_sda_pin);   // output
#define sht15_read_sda()  set_bit(tris_array[sht15_sda_port - PORTA], sht15_sda_pin);   // input


#define CHECK_TEMP 0b00000011
#define CHECK_HUMD 0b00000101
#define CHECK_STAT 0b00000111
#define WRITE_STAT 0b00000110


void sht15_setup(void)
{
	clear_bit(tris_array[sht15_sda_port - PORTA], sht15_sda_pin);   // output	trisa.1 = 0;
	clear_bit(tris_array[sht15_sck_port - PORTA], sht15_sck_pin);   // output
}

uns16 sht15_read_humidity(void) {

    uns16 response;

    sht15_start();
    sht15_send_byte(CHECK_HUMD);
    response = sht15_read_byte16();
    response = response >> 4;
	response = sht15_fix_humidity_l(response);
	return response;
}

uns16 sht15_read_temperature(void) {

uns16 response;
	
	sht15_start();
    sht15_send_byte(CHECK_TEMP);
    response = sht15_read_byte16(); //Listen for response from SHT15
    print_str("\nT=");
    print_int(response);
	response = sht15_fix_temperature_h(response);
    print_str("\nT=");
    print_int(response);

}	

//Init the sensor and read out the humidity and temperature data
void sht15_read(void)
{
    uns16 response;

    //Issue command start
    sht15_start();
    
    //Now send command code
    sht15_send_byte(CHECK_HUMD);
    response = sht15_read_byte16();
    print_str("\nH=");
    print_int(response);
    response = response >> 4;
	response = sht15_fix_humidity_l(response);
    print_str("\nH=");
    print_int(response);

    sht15_start();
    sht15_send_byte(CHECK_TEMP);
    response = sht15_read_byte16(); //Listen for response from SHT15
    print_str("\nT=");
    print_int(response);
	response = sht15_fix_temperature_h(response);
    print_str("\nT=");
    print_int(response);

}

void sht15_send_byte(uns8 sht15_command)
{
    uns8 i;


    sht15_write_sda();


    for(i = 0 ; i < 8 ; i++)
    {
        clear_pin(sht15_sck_port, sht15_sck_pin);
        change_pin(sht15_sda_port, sht15_sda_pin, sht15_command.7);
        sht15_command = sht15_command << 1;
        set_pin(sht15_sck_port, sht15_sck_pin);
    }

    //Wait for SHT15 to acknowledge.
    clear_pin(sht15_sck_port, sht15_sck_pin);
    sht15_read_sda();
    // <<!!>> Fix following
    while (test_pin(sht15_sda_port, sht15_sda_pin) == 1); //Wait for SHT to pull line low
    set_pin(sht15_sck_port, sht15_sck_pin);
    clear_pin(sht15_sck_port, sht15_sck_pin); //Falling edge of 9th clock
    
    while (test_pin(sht15_sda_port, sht15_sda_pin) == 0); //Wait for SHT to release line

    //Wait for SHT15 to pull SDA low to signal measurement completion. 
    //This can take up to 210ms for 14 bit measurements
    i = 0;
    while ((test_pin(sht15_sda_port, sht15_sda_pin) == 1)) //Wait for SHT to pull line low
    {
        i++;
        if (i == 255) break;
        
        delay_ms(10);
    }

    //Debug info
    i *= 10; //Convert to ms
    //print_str("\nRt=");
    //print_int(i);
    //print_str("ms"); //Debug to see how long response took

}

//Specific SHT start command
void sht15_start(void)
{
    sht15_write_sda();
    set_pin(sht15_sda_port, sht15_sda_pin); //SDA = 1;
    set_pin(sht15_sck_port, sht15_sck_pin); //SCK = 1;
    
    clear_pin(sht15_sda_port, sht15_sda_pin); //SDA = 0;
    clear_pin(sht15_sck_port, sht15_sck_pin); //SCK = 0;
    set_pin(sht15_sck_port, sht15_sck_pin); // SCK = 1;
    set_pin(sht15_sda_port, sht15_sda_pin); //SDA = 1;
    clear_pin(sht15_sck_port, sht15_sck_pin); //SCK = 0;
}

//Read 16 bits from the SHT sensor
uns16 sht15_read_byte16(void)
{
    uns8 j;
    uns16 in_byte;

	bit  my_store_gie = intcon.GIE;        
    kill_interrupts();

    clear_pin(sht15_sck_port, sht15_sck_pin);

    sht15_read_sda();

    for(j = 0 ; j < 16 ; j++)
    {
        clear_pin(sht15_sck_port, sht15_sck_pin);
        nop();
        nop();
        nop();
        nop();
		nop();
        set_pin(sht15_sck_port, sht15_sck_pin);
		nop();
        in_byte = in_byte << 1;
        in_byte.0 = test_pin(sht15_sda_port, sht15_sda_pin);
    }
	intcon.GIE = my_store_gie;
    return(in_byte);
}


uns16 sht15_fix_humidity_h(uns16 sensor_out)
{
	
long result; // 16Bit unsigned for the result

	if ( sensor_out <= 1712 ) {
		result = 143 * sensor_out - 8192; // result = a * sensor_out
		//result < 512 ? result = 512; // check for underflow
		//result = result – 8192; // result = result + b
	} else {
		result = 111 *sensor_out + 46288; // result = a * sensor_out
		//result = result + 46288; // result = result + b
		//result > 25600 ? result = 25600; // check for overflow (optional)
	}
	//8 MSB’s are 0-100%RH integers, 8 LSB’s are remainder
	result = result >> 12; // result = result / 256
	return result;
}

uns16 sht15_fix_humidity_l(uns8 sensor_out)
{

uns16 result; // 16Bit unsigned for the result

	if ( sensor_out <= 107 ) {
		result = 143 * sensor_out; // result = a * sensor_out
		if (result < 512) { result = 512; } // check for underflow
		result -= 512; 
	} else {
		result = 111 * sensor_out; // result = a * sensor_out
		result += 2893; // result = result + b
		if (result > 25600) { result = 25600; } // check for overflow (optional)
	}
	//8 MSB’s are 0-100%RH integers, 8 LSB’s are remainder
	result = result >> 8; // result = result / 256
	return result;
}

int16 sht15_fix_temperature_h(uns16 sensor_out)
{
	int16 result;
	
	result = -3963 + sensor_out;
    return result;
}