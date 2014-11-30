/*
* IMU.h
*
* Created: 11.11.2014 14:42:11
* Author: GeirW
*/


#ifndef __IMU_H__
#define __IMU_H__
#include "Arduino.h"
#include "CommandLibrary.h"
#include "Sensors.h"

#define GYRO_SCALE (4 / 16.4 * PI / 180.0 / 1000000.0)   //MPU6050 16.4 LSB/(deg/s) and we ignore the last 2 bits
#define ACC_LPF_FACTOR 4 // that means a LPF of 16
#define ACC_1G 512
#define GYR_CMPF_FACTOR 600
#define GYR_CMPFM_FACTOR 250
#define INV_GYR_CMPF_FACTOR   (1.0f / (GYR_CMPF_FACTOR  + 1.0f))
#define INV_GYR_CMPFM_FACTOR  (1.0f / (GYR_CMPFM_FACTOR + 1.0f))

PUBLIC void ComputeIMU(void);
PUBLIC void InitIMU(void);
PRIVATE void GetEstimatedAttitude(imu_t* imu);

#endif //__IMU_H__
