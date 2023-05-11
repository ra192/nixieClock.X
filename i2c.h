/* 
 * File:   i2c.h
 * Author: yak19
 *
 * Created on May 5, 2023, 4:57 PM
 */

#ifndef I2C_H
#define	I2C_H

#include <xc.h>


#ifdef	__cplusplus
extern "C" {
#endif

#define ACK 0x00
#define NACK 0x80

void i2c_start(void);

void i2c_stop(void);

uint8_t i2c_wr(uint8_t data);

void i2c_wr_bytes(uint8_t* byte_arr, uint8_t size);

uint8_t i2c_rd(uint8_t ack);

void i2c_rd_bytes(uint8_t* byte_arr, uint8_t size);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

