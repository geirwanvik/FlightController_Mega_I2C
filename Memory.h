/* 
* Memory.h
*
* Created: 16.11.2014 09:30:30
* Author: GeirW
*/


#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "Arduino.h"
#include "CommandLibrary.h"
#include <EEPROM.h>
#include "Checksum.h"
#define PUBLIC
#define PRIVATE static


PUBLIC void InitMemory(void);
PUBLIC void SaveMemory(void);
PUBLIC void ReadPidData(void);
PUBLIC void WritePidData(void);

PRIVATE void ReadMemory(byte address, byte* buffer, byte length);
PRIVATE void WriteMemory(byte address, byte* buffer, byte length);



#endif //__MEMORY_H__
