/*
* Protocol.h
*
* Created: 07.11.2014 10:09:11
* Author: GeirW
*/


#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#define PRIVATE static
#define PUBLIC

#include "Arduino.h"
#include "Checksum.h"

enum ProtocolStates { Start, Direction, Command, Length,Payload,CRC,End };
enum Identifier { StartByte = '#' };

struct ProtocolFrame { byte start; byte direction; byte command; byte length;
						byte payload[64]; uint16_t crc; };
		
PUBLIC bool ReceiveFrame(void);
PUBLIC void GetFrameData(byte& command, byte& direction, byte* payload, byte& length);
PUBLIC void SendFrame(byte& command, byte& direction, byte* payload, byte& length);

#endif //__PROTOCOL_H__
