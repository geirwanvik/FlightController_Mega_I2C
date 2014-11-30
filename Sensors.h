/* 
* Sensors.h
*
* Created: 13.11.2014 21:18:19
* Author: GeirW
*/


#ifndef __SENSORS_H__
#define __SENSORS_H__
#include "Arduino.h"
#include "I2C.h"
#include "CommandLibrary.h"
#include "Memory.h"

#define MPU6050_ADDRESS 0x68
#define MAG_ADDRESS 0x1E

#define PUBLIC
#define PRIVATE static

PUBLIC void InitSensors(void);
PUBLIC void GetGyroData(int16_t* gyroData);
PUBLIC void GetMagData(int16_t* mgaData);
PUBLIC void GetAccData(int16_t* accData);

PRIVATE void InitGyro(void);
PRIVATE void InitMag(void);
PRIVATE void InitAcc(void);
PRIVATE void ReadRawGyro(int16_t* data);
PRIVATE void ReadRawMag(int16_t* data);
PRIVATE void ReadRawMagMPU(int16_t* data);
PRIVATE void ReadRawAcc(int16_t* data);

#endif //__SENSORS_H__
