#include "checksum.h"

//    Optimized CRC-XMODEM calculation.
//    Polynomial: x^16 + x^12 + x^5 + 1 (0x1021)
//    Initial value: 0x0
//    This is the CRC used by the Xmodem-CRC protocol.

uint16_t crc = 0;

void UpdateCRC(uint8_t data)
{
	int i;
	crc = crc ^ ((uint16_t)data << 8);
	for (i=0; i<8; i++)
	{
		if (crc & 0x8000)
		crc = (crc << 1) ^ 0x1021;
		else
		crc <<= 1;
	}
}

void ClearCRC(void)
{
	crc = 0;
}

uint16_t GetCRC(void)
{
	return crc;
}