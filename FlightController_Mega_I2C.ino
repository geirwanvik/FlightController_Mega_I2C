#include "IMU.h"
#include "Telegram.h"
#include "RC.h"
#include "PID.h"
#include "CommandLibrary.h"
#include <EEPROM/EEPROM.h>
#include "Memory.h"

loopTime_t loopTime;

void setup()
{
	// Setup statuses
	status.accDoCalibrate = 0; // Set from GUI, or set true to calibrate at bootup. will save result to epprom
	status.magDoCalibrate = 0; // Set from GUI, or sett true to calibrate at bootup, led flash for 30 sec, will save to eeprom.
	status.checksumOK = 0; // Will check eeprom data vs eeprom stored crc, if no match, calibrations & pid will be 0. Will pass after calibrations.
	status.armed = 0; // Not implemented yet
	
	pinMode(13,OUTPUT);
	
	Serial.begin(115200);
	
	InitMemory(); // Blink off, 1 sec on, off - Eeprom data OK
	InitIMU();
	InitRC();
	InitMotors();
}

#define LOOPTIME (5000) // We want constant 5ms loops, 200hz
#define RC_INTERVAL (50000) // Read RC at 50ms intervals. 20hz

uint32_t loopRC = 0;
uint32_t lastLoop = 0, currentLoop;

void loop()
{
	ComputeIMU();
	ComputePID();

	if((micros() - loopRC) > RC_INTERVAL)
	{
		ComputeRC();
		loopRC = micros();
	}
	else
	{
		TelegramStateMachine();
	}
	
	currentLoop = micros();
	loopTime.time = currentLoop - lastLoop;
	if( loopTime.time < LOOPTIME)
	{
		delayMicroseconds( LOOPTIME - loopTime.time );
	}
	lastLoop = micros();

}
