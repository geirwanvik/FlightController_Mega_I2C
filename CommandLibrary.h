/*
 * CommandLibrary.h
 *
 * Created: 08.11.2014 15:16:20
 *  Author: GeirW
 */ 


#ifndef COMMANDLIBRARY_H_
#define COMMANDLIBRARY_H_

#include "Arduino.h"

#define LEDPIN_TOGGLE              PINB  |= (1<<7); PINC  |= (1<<7);
#define LEDPIN_ON                  PORTB |= (1<<7); PORTC |= (1<<7);
#define LEDPIN_OFF                 PORTB &= ~(1<<7);PORTC &= ~(1<<7);

enum Commands { IMU_CMD = 'a', ATT_CMD = 'b', PID_CMD = 'c', RC_CMD = 'd', MOTOR_CMD = 'e', EEPROM_CMD = 'f', STATUS_CMD = 'g', LOOPT_CMD = 'h'  };
	
enum Directions { ReadDirection = 'r', WriteDirection = 'w', Ack = 'a' };

enum rcChannels { ROLL, PITCH, YAW, THROTTLE, AUX1, AUX2, AUX3, AUX4, NUM_RC_CHANS };

extern int16_t rcData[NUM_RC_CHANS];

typedef struct 
{
	int16_t  accSmooth[3]; // Filtered
	int16_t  gyroData[3]; // Avg from two readings
	int16_t  magADC[3]; // After offset and scale
	int16_t  gyroADC[3]; // After offset and scale 
	int16_t  accADC[3]; // After offset and scale
} imu_t;
extern imu_t imu;

typedef struct 
{
	int16_t angle[2];            // absolute angle inclination in multiple of 0.1 degree 180 deg = 1800. [0] = ROLL , [1] = PITCH
	int16_t heading;             // variometer in cm/s
} att_t;
extern att_t att;

struct pid_t
{
	float kP;
	float kI;
	float kD;
};

typedef struct  
{
	struct pid_t rollPID;
	struct pid_t pitchPID;
	struct pid_t yawPID;
	int16_t iMax;
} p_t;
extern p_t p;

typedef struct  
{
	uint16_t front;
	uint16_t back;
	uint16_t left;
	uint16_t right;
} motor_t;
extern motor_t motor;

typedef struct  
{
	uint16_t roll;
	uint16_t pitch;
	uint16_t yaw;
	uint16_t throttle;
	uint16_t aux1;
	uint16_t aux2;
	uint16_t aux3;
	uint16_t aux4;
} rc_t;
extern rc_t rc;

typedef struct  
{
	uint16_t min;
	uint16_t max;
	uint16_t mid;
} rc_cali_t;

typedef struct
{
		rc_cali_t rcCalibrations[NUM_RC_CHANS];
		int16_t accOffset[3];
		int16_t magOffset[3];  // {81,45,206}; // Calibrated values
} settings_t;
extern settings_t settings;

typedef struct  
{
		uint8_t accDoCalibrate :1;
		uint8_t rccDoCalibrate :1;
		uint8_t magDoCalibrate :1;
		uint8_t checksumOK :1;
		uint8_t armed :1;	
} status_t;
extern status_t status;

typedef struct  
{
	uint16_t time;
} loopTime_t;
extern loopTime_t loopTime;
#endif /* COMMANDLIBRARY_H_ */
