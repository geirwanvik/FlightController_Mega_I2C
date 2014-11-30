/*
* Protocol.cpp
*
* Created: 07.11.2014 10:09:11
* Author: GeirW
*/

#include "Protocol.h"

static ProtocolFrame inFrame;

bool ReceiveFrame(void)
{
	bool frameComplete = false;
	static ProtocolStates state = Start;
	static byte bytesPayloadReceived = 0;
	static byte bytesCRCReceived = 0;
	
	while (Serial.available())
	{
		byte data = Serial.read();
		switch(state)
		{
			case Start:
			
			if(data == StartByte)
			{
				inFrame.start = data;
				ClearCRC();
				UpdateCRC(data);
				state = Direction; // next
			}
			break;
			
			case Direction:
			inFrame.direction = data;
			UpdateCRC(data);
			state = Command; // next
			break;
			
			case Command:
			inFrame.command = data;
			UpdateCRC(data);
			state = Length; // next
			break;
			
			case Length:
			if(data > 64) // length not valid
			{
				state = Start; // goto start
			}
			else
			{
				inFrame.length = data;
				UpdateCRC(data);
				state = Payload; // next
			}
			break;
			
			case Payload:
			if( (bytesPayloadReceived == inFrame.length) || inFrame.length == 0 )
			{
				bytesPayloadReceived = 0;
				state = CRC; // next, no break
			}
			else
			{
				inFrame.payload[bytesPayloadReceived++]=data;
				UpdateCRC(data);
				break; // stay
			}
			
			case CRC:
			if(bytesCRCReceived == 0)
			{
				inFrame.crc = data;
				bytesCRCReceived++;
				break; //stay
			}
			else if(bytesCRCReceived == 1)
			{
				inFrame.crc = inFrame.crc | (data << 8);
				bytesCRCReceived = 0;
				state = End; // next, no break
			}
			
			case End:
			state =Start; // goto start
			if(inFrame.crc == GetCRC())
			{
				return (frameComplete = true); // process command
			}
			break;
		}
	}
	
	return (frameComplete = false); // not finished & buffer empty
}

void GetFrameData(byte& command, byte& direction, byte* payload, byte& length)
{
	command = inFrame.command;
	direction = inFrame.direction;
	length = inFrame.length;
	memcpy(payload,&inFrame.payload,inFrame.length);
}

void SendFrame(byte& command, byte& direction, byte* payload, byte& length)
{
	ClearCRC();
	
	Serial.write(StartByte);
	UpdateCRC(StartByte);
	
	Serial.write(direction);
	UpdateCRC(direction);
	
	Serial.write(command);
	UpdateCRC(command);
	
	Serial.write(length);
	UpdateCRC(length);
	
	for(uint8_t i = 0; i< length; i++)
	{
		Serial.write(payload[i]);
		UpdateCRC(payload[i]);
	}
	
	uint16_t crc = GetCRC();
	
	Serial.write(crc & 0xFF);
	Serial.write(crc >> 8);
}
