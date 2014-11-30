/* 
* Telegram.h
*
* Created: 09.11.2014 12:03:26
* Author: GeirW
*/


#ifndef __TELEGRAM_H__
#define __TELEGRAM_H__

#define PRIVATE static
#define PUBLIC

#include "Protocol.h"
#include "CommandLibrary.h"
#include "Memory.h"

PUBLIC void TelegramStateMachine(void);

PRIVATE void ProcessTelegram(void);
PRIVATE void SendTelegram(void);
PRIVATE void SendSensorData(void);
PRIVATE void SendAttData(void);
PRIVATE void SendPidData(void);
PRIVATE void StorePidData(byte* payload, byte length);
PRIVATE void SendRcData(void);
PRIVATE void SendEepromData(void);
PRIVATE void StoreEepromData(byte* payload, byte& length);
PRIVATE void SendMotorData(void);
PRIVATE void StoreStatus(byte* payload, byte length);
PRIVATE void SendStatus(void);
PRIVATE void SendLoopTime(void);
#endif //__TELEGRAM_H__
