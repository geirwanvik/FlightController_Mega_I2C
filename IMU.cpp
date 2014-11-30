/*
* IMU.cpp
*
* Created: 11.11.2014 14:42:11
* Author: GeirW
*/

#include "IMU.h"

imu_t imu;
att_t att;

void InitIMU(void)
{
	InitSensors();
	int16_t gyroADC[3];
	
	//Calibration
	for (int i = 0; i< 600; i++)
	{
		GetGyroData(gyroADC);
		delay(1);
	}
}

void ComputeIMU(void)
{

	static int16_t gyroADCprevious[3] = {0,0,0};
	int16_t gyroADCp[3];
	int16_t gyroADCinter[3];
	
	GetMagData(imu.magADC);
	
	GetAccData(imu.accADC);
	GetEstimatedAttitude(&imu);
	GetGyroData(imu.gyroADC);

	for (byte axis = 0; axis < 3; axis++)
	{
		gyroADCp[axis] =  imu.gyroADC[axis];
	}
	
	delayMicroseconds(650); //empirical, interleaving delay between 2 consecutive reads
	GetGyroData(imu.gyroADC);

	for (byte axis = 0; axis < 3; axis++)
	{
		gyroADCinter[axis] =  imu.gyroADC[axis]+gyroADCp[axis];
		// empirical, we take a weighted value of the current and the previous values
		imu.gyroData[axis] = (gyroADCinter[axis]+gyroADCprevious[axis])/3;
		gyroADCprevious[axis] = gyroADCinter[axis]>>1;
		//imu.accADC[axis]=0;
	}
}


typedef struct fp_vector
{
	float X,Y,Z;
} t_fp_vector_def;

typedef union
{
	float A[3];
	t_fp_vector_def V;
} t_fp_vector;

typedef struct int32_t_vector
{
	int32_t X,Y,Z;
} t_int32_t_vector_def;

typedef union
{
	int32_t A[3];
	t_int32_t_vector_def V;
} t_int32_t_vector;

int16_t _atan2(int32_t y, int32_t x)
{
	float z = (float)y / x;
	int16_t a;
	if ( abs(y) < abs(x) ){
		a = 573 * z / (1.0f + 0.28f * z * z);
		if (x<0) {
			if (y<0) a -= 1800;
			else a += 1800;
		}
		} else {
		a = 900 - 573 * z / (z * z + 0.28f);
		if (y<0) a -= 1800;
	}
	return a;
}

float InvSqrt (float x)
{
	union{
		int32_t i;
		float   f;
	} conv;
	conv.f = x;
	conv.i = 0x5f3759df - (conv.i >> 1);
	return 0.5f * conv.f * (3.0f - x * conv.f * conv.f);
}

// Rotate Estimated vector(s) with small angle approximation, according to the gyro data
void rotateV(struct fp_vector *v,float* delta)
{
	fp_vector v_tmp = *v;
	v->Z -= delta[ROLL]  * v_tmp.X + delta[PITCH] * v_tmp.Y;
	v->X += delta[ROLL]  * v_tmp.Z - delta[YAW]   * v_tmp.Y;
	v->Y += delta[PITCH] * v_tmp.Z + delta[YAW]   * v_tmp.X;
}

static int32_t accLPF32[3]    = {0, 0, 1};
static float invG; // 1/|G|

static t_fp_vector EstG;
static t_int32_t_vector EstG32;
static t_int32_t_vector EstM32;
static t_fp_vector EstM;

void GetEstimatedAttitude(imu_t* imu_ptr)
{
	uint8_t axis;
	int32_t accMag = 0;
	float scale, deltaGyroAngle[3];
	uint8_t validAcc;
	static uint16_t previousT;
	uint16_t currentT = micros();

	scale = (currentT - previousT) * GYRO_SCALE; // GYRO_SCALE unit: radian/microsecond
	previousT = currentT;

	// Initialization
	for (axis = 0; axis < 3; axis++)
	{
		deltaGyroAngle[axis] = imu_ptr->gyroADC[axis]  * scale; // radian

		accLPF32[axis]    -= accLPF32[axis]>>ACC_LPF_FACTOR;
		accLPF32[axis]    += imu_ptr->accADC[axis];
		imu_ptr->accSmooth[axis]    = accLPF32[axis]>>ACC_LPF_FACTOR;

		accMag += (int32_t)imu_ptr->accSmooth[axis]*imu_ptr->accSmooth[axis] ;
	}

	rotateV(&EstG.V,deltaGyroAngle);
	rotateV(&EstM.V,deltaGyroAngle);
	
	accMag = accMag*100/((int32_t)ACC_1G*ACC_1G);
	validAcc = 72 < (uint16_t)accMag && (uint16_t)accMag < 133;
	// Apply complimentary filter (Gyro drift correction)
	// If accel magnitude >1.15G or <0.85G and ACC vector outside of the limit range => we neutralize the effect of accelerometers in the angle estimation.
	// To do that, we just skip filter, as EstV already rotated by Gyro
	for (axis = 0; axis < 3; axis++)
	{
		if ( validAcc )
		EstG.A[axis] = (EstG.A[axis] * GYR_CMPF_FACTOR + imu_ptr->accSmooth[axis]) * INV_GYR_CMPF_FACTOR;
		EstG32.A[axis] = EstG.A[axis]; //int32_t cross calculation is a little bit faster than float
		EstM.A[axis] = (EstM.A[axis] * GYR_CMPFM_FACTOR  + imu_ptr->magADC[axis]) * INV_GYR_CMPFM_FACTOR;
		EstM32.A[axis] = EstM.A[axis];
	}

	// Attitude of the estimated vector
	int32_t sqGX_sqGZ = sq(EstG32.V.X) + sq(EstG32.V.Z);
	invG = InvSqrt(sqGX_sqGZ + sq(EstG32.V.Y));
	att.angle[ROLL]  = _atan2(EstG32.V.X , EstG32.V.Z);
	att.angle[PITCH] = _atan2(EstG32.V.Y , InvSqrt(sqGX_sqGZ)*sqGX_sqGZ);
	
	att.heading = _atan2(
	EstM32.V.Z * EstG32.V.X - EstM32.V.X * EstG32.V.Z,
	(EstM.V.Y * sqGX_sqGZ  - (EstM32.V.X * EstG32.V.X + EstM32.V.Z * EstG32.V.Z) * EstG.V.Y)*invG );
	att.heading /= 10;
}