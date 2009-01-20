#include "i2c.h"
#include "pic_serial.h"
#define DELAY_AMOUNT 50

void i2c_ack_polling(uns8 device_address)
{
    while(test_pin(i2c_sda_port, i2c_sda_pin)!= 0)
    {
        i2c_start();
        i2c_send_byte(device_address);
    }
    i2c_stop();
}

void i2c_write_eeprom(uns8 device_address, uns8 mem_address, uns8 data)
{
    //i2c_ack_polling(device_address);
    i2c_start();
    i2c_send_byte(device_address);

    //send_byte(address.high8); //Uppder Address - Needed for >= 32k EEProms
    i2c_send_byte(mem_address);
    i2c_send_byte(data);
    i2c_stop();
}

uns8 i2c_read_eeprom(uns8 device_address, uns8 mem_address)
{
uns8 data;
    //i2c_ack_polling(device_address);

    i2c_start();
    i2c_send_byte(device_address);
    //send_byte(address.high8); // Upper Address - Needed for >= 32k EEProms
    i2c_send_byte(mem_address);
    i2c_stop();

    i2c_start();
    i2c_send_byte(device_address | 0b00000001); // Read bit must be set
    data = i2c_receive_byte(); // reuse local variable

    // write an nack
    clear_pin(i2c_scl_port, i2c_scl_pin);
    i2c_write_sda();
    delay_us(DELAY_AMOUNT);
    set_pin(i2c_sda_port, i2c_sda_pin);
    set_pin(i2c_scl_port, i2c_scl_pin);
    delay_us(DELAY_AMOUNT);

    i2c_stop();
    
    return(data);
}

uns16 i2c_read_eeprom_16bit(uns8 device_address, uns8 mem_address)
{
    uns16 data;
    
    //i2c_ack_polling(device_address);
    
    i2c_start();
    i2c_send_byte(device_address);
    //send_byte(address.high8); // Upper Address - Needed for >= 32k EEProms
    i2c_send_byte(mem_address);
    i2c_stop();

    i2c_start();
    i2c_send_byte(device_address | 0b00000001); // Read bit must be set
    data = i2c_receive_byte(); // reuse local variable
    i2c_write_sda();
	
    clear_pin(i2c_scl_port, i2c_scl_pin);
    delay_us(DELAY_AMOUNT);
    clear_pin(i2c_sda_port, i2c_sda_pin); //ack
    delay_us(DELAY_AMOUNT);
    set_pin(i2c_scl_port, i2c_scl_pin);
    delay_us(DELAY_AMOUNT);
    
    data = data << 8 | i2c_receive_byte(); // reuse local variable

    // write an nack
    clear_pin(i2c_scl_port, i2c_scl_pin);
    i2c_write_sda();
    delay_us(DELAY_AMOUNT);
    set_pin(i2c_sda_port, i2c_sda_pin);
    set_pin(i2c_scl_port, i2c_scl_pin);
    delay_us(DELAY_AMOUNT);

    i2c_stop();
    
    return(data);
}


void i2c_start(void)
{
    i2c_write_sda();
    
    delay_us(DELAY_AMOUNT);
    
    clear_pin(i2c_sda_port, i2c_sda_pin);
    delay_us(DELAY_AMOUNT);
}

void i2c_stop(void)
{
    clear_pin(i2c_scl_port, i2c_scl_pin);
    delay_us(DELAY_AMOUNT);

    i2c_write_sda();

    clear_pin(i2c_sda_port, i2c_sda_pin);
    delay_us(DELAY_AMOUNT);
    set_pin(i2c_scl_port, i2c_scl_pin);
    delay_us(DELAY_AMOUNT);
    set_pin(i2c_sda_port, i2c_sda_pin);
    delay_us(DELAY_AMOUNT);
}

uns8 i2c_receive_byte(void)
{
   uns8 count, in_byte;

    clear_pin(i2c_scl_port, i2c_scl_pin);

    i2c_read_sda();

    for(count = 0 ; count < 8 ; count++)
    {
        clear_pin(i2c_scl_port, i2c_scl_pin);
        delay_us(DELAY_AMOUNT);
        set_pin(i2c_scl_port, i2c_scl_pin);
        delay_us(DELAY_AMOUNT);

        in_byte = in_byte << 1;
        in_byte.0 = test_pin(i2c_sda_port, i2c_sda_pin);
    }

    return(in_byte);
}

void i2c_send_byte(uns8 data)
{
   uns8 count;

    i2c_write_sda();

    for( count = 0 ; count < 8 ; count++ )
    {
        clear_pin(i2c_scl_port, i2c_scl_pin);
        change_pin(i2c_sda_port, i2c_sda_pin, data.7);
        data = data << 1;
        delay_us(DELAY_AMOUNT);
        set_pin(i2c_scl_port, i2c_scl_pin);
        delay_us(DELAY_AMOUNT);
    }

    // read ack (and ignore)
    clear_pin(i2c_scl_port, i2c_scl_pin);
    i2c_read_sda();
    delay_us(DELAY_AMOUNT);
    set_pin(i2c_scl_port, i2c_scl_pin);
}

void i2c_setup() {
	make_output(i2c_scl_port, i2c_scl_pin);   // output
}
