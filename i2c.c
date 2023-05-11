/* 
 * File:   i2c.c
 * Author: yak19
 *
 * Created on May 5, 2023, 4:58 PM
 */
#include "i2c.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/device_config.h"

//....................................................................
// This function generates an I2C Start Condition
//....................................................................
void i2c_start(void)
{
    SDA_SetDigitalInput(); // ensure SDA & SCL are high
    SCL_SetHigh();
    SDA_SetDigitalOutput(); // SDA = output
    SDA_SetLow(); // pull SDA low
    __delay_us(2);
    SCL_SetLow(); // pull SCL low
}

//....................................................................
// This function generates an I2C Stop Condition
//....................................................................
void i2c_stop(void)
{
    SCL_SetLow(); // ensure SCL is low
    SDA_SetDigitalOutput(); // SDA = output
    SDA_SetLow(); // SDA low
    __delay_us(2);
    SCL_SetHigh(); // pull SCL high
    SDA_SetDigitalInput(); // allow SDA to be pulled high
    __delay_us(2);
    SCL_SetLow(); // ensure SCL is low
}

//....................................................................
// Outputs a bit to the I2C bus
//....................................................................
void bit_out(uint8_t data)
{
    SCL_SetLow(); // ensure SCL is low
    SDA_SetDigitalOutput(); // configure SDA as an output
    
    // output the MSB
    if(data>>7)
    {
        SDA_SetHigh();
    }
    else
    {
        SDA_SetLow();
    }
    
    __delay_us(2);
    SCL_SetHigh(); // pull SCL high to clock bit
    __delay_us(2);
    SCL_SetLow(); // pull SCL low for next bit
}

//....................................................................
// Inputs a bit from the I2C bus
//....................................................................
void bit_in(uint8_t *data)
{
    SCL_SetLow(); // ensure SCL is low
    __delay_us(2);
    SDA_SetDigitalInput(); // configure SDA as an input
    SCL_SetHigh(); // bring SCL high to begin transfer
    *data |= SDA_GetValue(); // input the received bit
    SCL_SetLow(); // bring SCL low again.
}

//....................................................................
// Writes a byte to the I2C bus
//....................................................................
uint8_t i2c_wr(uint8_t data)
{
    uint8_t i;                // loop counter
    uint8_t ack;              // ACK bit

    ack = 0;
    for (i = 0; i < 8; i++)         // loop through each bit
    {
        bit_out(data);              // output bit
        data = (uint8_t) (data << 1);           // shift left for next bit
    }

    bit_in(&ack);                   // input ACK bit
    return ack;
}

void i2c_wr_bytes(uint8_t* byte_arr, uint8_t size)
{
    for(uint8_t i = 0; i < size; i++)
    {
        i2c_wr(byte_arr[i]);
    }
}

//....................................................................
// Reads a byte from the I2C bus
//....................................................................
uint8_t i2c_rd(uint8_t ack)
{
    uint8_t i;                // loop counter
    uint8_t ret=0;            // return value

    for (i = 0; i < 8; i++)         // loop through each bit
    {
        ret = (uint8_t)(ret << 1);             // shift left for next bit
        bit_in(&ret);               // input bit
    }

    bit_out(ack);                   // output ACK/NAK bit
    return ret;
}

void i2c_rd_bytes(uint8_t* byte_arr, uint8_t size)
{
    for(uint8_t i=0; i < size - 1; i++)
    {
       byte_arr[i] = i2c_rd(ACK);
    }
    
    byte_arr[size - 1] = i2c_rd(NACK);
}