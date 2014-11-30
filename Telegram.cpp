/*
* Telegram.cpp
*
* Created: 09.11.2014 12:03:25
* Author: GeirW
*/
#include "Telegram.h"

void TelegramStateMachine(void)
{
	bool processComplete = ReceiveFrame();
	
	if(processComplete)
	{
		ProcessTelegram();
	}
}

void ProcessTelegram(void)
{
	byte payload[64];
	byte command;
	byte direction;
	byte length;
	
	GetFrameData(command,direction,payload,length);
	
	switch(command)
	{
		case IMU_CMD:
		SendSensorData();
		break;
		
		case ATT_CMD:
		SendAttData();
		break;
		
		case PID_CMD:
		if(direction == WriteDirection)
		{
			StorePidData(payload,length);
		}
		else
		{
			SendPidData();
		}
		break;
		
		case RC_CMD:
		SendRcData();
		break;
		
		case MOTOR_CMD:
		SendMotorData();
		break;
		
		case EEPROM_CMD:
		if (direction == WriteDirection)
		{
			StoreEepromData(payload,length);
		}
		else
		{
			SendEepromData();
		}
		break;
		
		case STATUS_CMD:
		if (direction == WriteDirection)
		{
			StoreStatus(payload,length);
		}
		else
		{
			SendStatus();
		}
		break;
		
		case LOOPT_CMD:
		SendLoopTime();
		break;
	}
}

void SendSensorData(void)
{
	byte length = sizeof(imu_t);
	byte payload[length];
	byte command = IMU_CMD;
	byte direction = WriteDirection;
	memcpy(payload,&imu,length);
	SendFrame(command,direction,payload,length);
}

void SendAttData(void)
{
	byte length = sizeof(att_t);
	byte payload[length];
	byte command = ATT_CMD;
	byte direction = WriteDirection;
	memcpy(payload,&att,length);
	SendFrame(command,direction,payload,length);
}

void SendPidData(void)
{
	byte length = sizeof(p_t);
	byte payload[length];
	byte command = PID_CMD;
	byte direction = WriteDirection;
	memcpy(payload,&p,sizeof(p_t));
	SendFrame(command,direction,payload,length);
}
void StorePidData(byte* payload, byte length)
{
	memcpy(&p,payload,length);
	WritePidData();
	
	byte outLength = 0;
	byte outPayload;
	byte command = PID_CMD;
	byte direction =  Ack;
	SendFrame(command,direction,&outPayload,outLength);
}

void SendRcData(void)
{
	byte length = sizeof(rc_t);
	byte payload[length];
	byte command = RC_CMD;
	byte direction = WriteDirection;
	memcpy(payload,&rc,length);
	SendFrame(command,direction,payload,length);
}

void SendMotorData(void)
{
	byte length = sizeof(motor_t);
	byte payload[length];
	byte command = MOTOR_CMD;
	byte direction = WriteDirection;
	memcpy(payload,&motor,length);
	SendFrame(command,direction,payload,length);
}

void StoreEepromData(byte* payload, byte& length)
{
	memcpy(&settings,payload,length);
	SaveMemory();
	
	byte outLength = 0;
	byte outPayload;
	byte command = EEPROM_CMD;
	byte direction =  Ack;
	SendFrame(command,direction,&outPayload,outLength);
}

void SendEepromData(void)
{
	byte length = sizeof(settings_t);
	byte payload[length];
	byte command = EEPROM_CMD;
	byte direction = WriteDirection;
	memcpy(payload,&settings,length);
	SendFrame(command,direction,payload,length);
}

void StoreStatus(byte* payload, byte length)
{
	memcpy(&status,payload,length);
	byte outLength = 0;
	byte outPayload;
	byte command = STATUS_CMD;
	byte direction = Ack;
	SendFrame(command,direction,&outPayload,outLength);
}

void SendStatus(void)
{
	byte length = sizeof(status_t);
	byte payload[length];
	byte command = STATUS_CMD;
	byte direction = WriteDirection;
	memcpy(payload,&status,length);
	SendFrame(command,direction,payload,length);
}

void SendLoopTime(void)
{
	byte length = sizeof(loopTime_t);
	byte payload[length];
	byte command = LOOPT_CMD;
	byte direction = WriteDirection;
	memcpy(payload,&loopTime,length);
	SendFrame(command,direction,payload,length);	
}