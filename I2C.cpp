/*
* I2C.cpp
*
* Created: 13.11.2014 21:35:04
* Author: GeirW
*/


#include "I2C.h"

void InitI2C(void)
{
	TWSR = 0;                                    // no prescaler => prescaler = 1
	TWBR = ((F_CPU / 400000L) - 16) / 2;		 // 400khz I2C
	TWCR = 1<<TWEN;                              // enable twi module, no interrupt
}

void ReadBytesI2C(uint8_t add, uint8_t reg, void *buf, size_t size)
{
	i2c_rep_start(add<<1); // I2C write direction
	i2c_write(reg);        // register selection
	i2c_read_to_buf(add, buf, size);
}

void WriteByteI2C(uint8_t add, uint8_t reg, uint8_t val)
{
	i2c_rep_start(add<<1); // I2C write direction
	i2c_write(reg);        // register selection
	i2c_write(val);        // value to write in register
	i2c_stop();
}

void i2c_rep_start(uint8_t address)
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) ; // send REPEAT START condition
	waitTransmissionI2C();                       // wait until transmission completed
	TWDR = address;                              // send device address
	TWCR = (1<<TWINT) | (1<<TWEN);
	waitTransmissionI2C();                       // wail until transmission completed
}

void i2c_stop(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void i2c_write(uint8_t data)
{
	TWDR = data;                                 // send data to the previously addressed device
	TWCR = (1<<TWINT) | (1<<TWEN);
	waitTransmissionI2C();
}

uint8_t i2c_read(uint8_t ack)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (ack? (1<<TWEA) : 0);
	waitTransmissionI2C();
	uint8_t r = TWDR;
	if (!ack) i2c_stop();
	return r;
}

void waitTransmissionI2C(void)
{
	uint16_t count = 255;
	while (!(TWCR & (1<<TWINT)))
	{
		count--;
		if (count==0)
		{              //we are in a blocking state => we don't insist
			TWCR = 0;    //and we force a reset on TWINT register
			break;
		}
	}
}

void i2c_read_to_buf(uint8_t add, void *buf, size_t size)
{
	i2c_rep_start((add<<1) | 1);  // I2C read direction
	uint8_t *b = (uint8_t*)buf;
	while (size--) {
		/* acknowledge all but the final byte */
		*b++ = i2c_read(size > 0);
		/* TODO catch I2C errors here and abort */
	}
}

