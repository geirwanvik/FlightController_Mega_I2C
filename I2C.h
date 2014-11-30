/* 
* I2C.h
*
* Created: 13.11.2014 21:35:04
* Author: GeirW
*/


#ifndef __I2C_H__
#define __I2C_H__
#include "Arduino.h"

#define PUBLIC
#define PRIVATE static

PUBLIC void InitI2C(void);
PUBLIC void ReadBytesI2C(uint8_t address, uint8_t reg, void *buf, size_t size);
PUBLIC void WriteByteI2C(uint8_t address, uint8_t reg, uint8_t val);

PRIVATE void i2c_rep_start(uint8_t address);
PRIVATE void i2c_write(uint8_t data);
PRIVATE void i2c_read_to_buf(uint8_t add, void *buf, size_t size);
PRIVATE void i2c_stop(void);
PRIVATE void waitTransmissionI2C(void);

#endif //__I2C_H__
