/* 
* Memory.cpp
*
* Created: 16.11.2014 09:30:30
* Author: GeirW
*/


#include "Memory.h"

#define CRC_ADR (0)
#define DATA_ADR (4)
#define PID_ADR (200)

settings_t settings;
status_t status;

void InitMemory(void)
{
	byte crcArray[2];
	ReadMemory(CRC_ADR,crcArray,sizeof(crcArray));
	
	uint16_t savedCRC = (crcArray[0] << 8) | crcArray[1];
	
	byte eepromArray[sizeof(settings_t)];
	
	ReadMemory(DATA_ADR,eepromArray,sizeof(eepromArray));
	
	ClearCRC();
	
	for(byte i = 0; i< sizeof(settings_t); i++)
	{
		 UpdateCRC(eepromArray[i]);
	}
	
	if (savedCRC == GetCRC())
	{
		memcpy(&settings,eepromArray,sizeof(settings_t));
		
		status.checksumOK = true;
		
		ReadPidData();
		
		LEDPIN_OFF
		delay(1000);
		LEDPIN_ON
		delay(1000);
		LEDPIN_OFF
	} 
	else
	{
		status.checksumOK = false;
		for(byte i = 0; i<3;i++)
		{
			settings.accOffset[i] = 0;
			settings.magOffset[i] = 0;
		}
	}
}

void SaveMemory(void)
{
	byte eepromArray[sizeof(settings_t)];
	memcpy(eepromArray,&settings,sizeof(settings_t));
	
	ClearCRC();
	for(byte i = 0; i< sizeof(settings_t); i++)
	{
		UpdateCRC(eepromArray[i]);
	}
	
	uint16_t crc = GetCRC();
	byte crcArray[2];
	crcArray[0] = highByte(crc);
	crcArray[1] = lowByte(crc);
	
	WriteMemory(CRC_ADR,crcArray,sizeof(crcArray));
	
	WriteMemory(DATA_ADR,eepromArray,sizeof(eepromArray));
}

void ReadPidData(void)
{
	byte readArray[sizeof(p_t)];
	ReadMemory(PID_ADR,readArray,sizeof(readArray));
	memcpy(&p,readArray,sizeof(readArray));
}
void WritePidData(void)
{
	byte writeArray[sizeof(p_t)];
	memcpy(writeArray,&p,sizeof(p_t));
	WriteMemory(PID_ADR,writeArray,sizeof(writeArray));
}

void WriteMemory(byte address, byte* buffer, byte length)
{
	for(byte i = 0; i< length; i++)
	{
		EEPROM.write( (address + i) ,buffer[i]);
	}
	
}

void ReadMemory(byte address, byte* buffer, byte length)
{
	for(byte i = 0; i< length; i++)
	{
		buffer[i] = EEPROM.read( (address +i) );
	}	
}