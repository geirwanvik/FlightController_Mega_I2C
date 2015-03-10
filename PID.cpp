/*
* PID.cpp
*
* Created: 14.11.2014 08:27:27
* Author: GeirW
*/


#include "PID.h"

p_t p;
motor_t motor;
float rollP, rollI = 0, rollD, rollPID;
float pitchP, pitchI = 0, pitchD, pitchPID;
float yawP, yawI = 0, yawD, yawPID;
float lastRollError = 0, lastPitchError = 0, lastYawError = 0;
void ComputePID(void)
{

	// ROLL
	int16_t error = (rc.roll - 1500);
	error -= imu.gyroADC[ROLL];
	rollP = error * p.rollPID.kP;
	rollI += error;
	rollI = p.rollPID.kI * constrain(rollI,-p.iMax,p.iMax);
	rollD = p.rollPID.kD * (error - lastRollError);
	lastRollError = error;
	rollPID = rollP + rollI + rollD;
	
	// PITCH
	error = (rc.pitch - 1500);
	error -= imu.gyroADC[PITCH];
	pitchP = error * p.pitchPID.kP;
	pitchI += error;
	pitchI = p.pitchPID.kI * constrain(pitchI,-p.iMax,p.iMax);
	pitchD = p.pitchPID.kD * (error - lastPitchError);
	lastPitchError = error;
	pitchPID = pitchP + pitchI + pitchD;

        // Debug
        debug.value1 = pitchP;
        debug.value2 = pitchI;
        debug.value3 = pitchD;
	
	// YAW
	error = (rc.yaw - 1500);
	error -= imu.gyroADC[YAW];
	yawP = error * p.yawPID.kP;
	yawI += error;
	yawI = p.yawPID.kI * constrain(yawI,-p.iMax,p.iMax);
	yawD = p.yawPID.kD * (error - lastYawError);
	lastYawError = error;
	yawPID = yawP + yawI + yawD;
	

	motor.back = rc.throttle - pitchPID - yawPID;
        if(motor.back < 1050)
        {
          motor.back = 1050;
        }
        else if (motor.back > 1850)
        {
          motor.back = 1850;
        }
	motor.right = rc.throttle + rollPID + yawPID;
        if(motor.right < 1050)
        {
          motor.right = 1050;
        }
        else if (motor.right > 1850)
        {
          motor.right = 1850;
        }
	motor.left = rc.throttle - rollPID + yawPID;
        if(motor.left < 1050)
        {
          motor.left = 1050;
        }
        else if (motor.left > 1850)
        {
          motor.left = 1850;
        }
	motor.front = rc.throttle + pitchPID -yawPID;
        if(motor.front < 1050)
        {
          motor.front = 1050;
        }
        else if (motor.front > 1850)
        {
          motor.front = 1850;
        }
	
	// Motor Mix - multiwii quad+ - burde stemme med gyro/imu aksene
	
	// back = throttle + pitchPID - yawPID
	
	// right = throttle - rollPID + yawPID
	
	// left = throttle + rollPID + yawPID
	
	// front = throttle - pitchPID - yawPID

        WriteMotors();
	
}

uint8_t PWM_PIN[4] = {3,5,6,2};      //for a quad+: rear,right,left,front

void InitMotors(void)
{
	for(uint8_t i=0;i<4;i++)
	{
		pinMode(PWM_PIN[i],OUTPUT); // mark all PWM pins as Output
	}
	
	//Specific PWM Timers & Registers for the MEGA's
	// init 16bit timer 3
	TCCR3A |= (1<<WGM31); // phase correct mode
	TCCR3A &= ~(1<<WGM30);
	TCCR3B |= (1<<WGM33);
	TCCR3B &= ~(1<<CS31); // no prescaler
	ICR3   |= 0x3FFF; // TOP to 16383;
	
	TCCR3A |= _BV(COM3C1); // connect pin 3 to timer 3 channel C
	TCCR3A |= _BV(COM3A1); // connect pin 5 to timer 3 channel A

	// init 16bit timer 4
	TCCR4A |= (1<<WGM41); // phase correct mode
	TCCR4A &= ~(1<<WGM40);
	TCCR4B |= (1<<WGM43);
	TCCR4B &= ~(1<<CS41); // no prescaler
	ICR4   |= 0x3FFF; // TOP to 16383;
	
	TCCR4A |= _BV(COM4A1); // connect pin 6 to timer 4 channel A
	TCCR3A |= _BV(COM3B1); // connect pin 2 to timer 3 channel B
	delay(300);
        
}
uint16_t motorIdle = 1000;
void WriteMotors(void)
{ 
	// Specific PWM Timers & Registers for the MEGA's
	// [1000:2000] => [8000:16000] for timer 3 & 4 for mega
        if (status.armed)
        {
       	  OCR3C = (motor.back<<3); //  pin 3
    	  OCR3A = (motor.right<<3); //  pin 5
    	  OCR4A = (motor.left<<3); //  pin 6
    	  OCR3B = (motor.front<<3); //  pin 2
        }
        else
        {
       	  OCR3C = (motorIdle<<3); //  pin 3
    	  OCR3A = (motorIdle<<3); //  pin 5
    	  OCR4A = (motorIdle<<3); //  pin 6
    	  OCR3B = (motorIdle<<3); //  pin 2
        }    
}

