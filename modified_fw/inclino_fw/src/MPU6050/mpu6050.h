/**
 * @file 	mpu6050.h
 * @author 	Omkar Jadhav (omjadhav@pdx.edu)
 *
 * @brief	The header file contains prototypes for MPU6050 sensor
 *
 * @rev	 1.0  - Created the header file and added prototypes for initialization
 * 			  - Added basic gryo configuration and X axis data capture function prototypes
 */
#ifndef __MPU6050_H__
#define __MPU6050_H__

/************************ File Includes **********************************/

#include <stdbool.h>
#include "xiic.h"

/************************ Macros **********************************/

#define 	ACCELOMETER_MODE 	0x1
#define 	GYRO_MODE		 	0x2
#define 	TEMP_MODE		 	0x3
#define		SET_AXIS_X			0x4
#define		SET_AXIS_Y			0x5
#define		SET_AXIS_Z			0x6
#define 	WHO_AM_I		    0x75
#define     CONFIG              0x1A
#define 	MPU6050_ID			0x68
#define 	PWR_MGMT_1			0X6B
#define 	GYRO_CONFIG			0x1B
#define 	ACC_CONFIG			0x1C
#define 	GYRO_XOUT_2			0x43
#define 	GYRO_XOUT_1			0x44
#define 	GYRO_YOUT_2			0x45
#define 	GYRO_YOUT_1			0x46
#define 	GYRO_ZOUT_2			0x47
#define 	GYRO_ZOUT_1			0x48
#define 	ACC_XOUT_2			0x3B
#define 	ACC_XOUT_1			0x3C
#define 	ACC_YOUT_2			0x3D
#define 	ACC_YOUT_1			0x3E
#define 	ACC_ZOUT_2			0x3F
#define 	ACC_ZOUT_1			0x40

/************************Struct Variable Declarations **********************************/
typedef struct
{
  //float rawAccVal[3];
  //float rawGyroVal[3];
  float roll;
  float pitch;
  float yaw;
  float xAxisOffset;
  float yAxisOffset;
  float zAxisOffset;

} MPU6050;

/*************************** Function Prototypes ***************************************/
void mpu6050_init(XIic* i2c, MPU6050* mpu6050);
void mpu6050_getGyroData(XIic* i2c, MPU6050* mpu6050, u8 *angle_actual, int axis, int numReg);
void mpu6050_getAccData(XIic* i2c, MPU6050* mpu6050, u8 *angle_actual, int axis, int numReg);
void mpu6050_gyroCfg(XIic* i2c, MPU6050* mpu6050);
void mpu6050_accCfg(XIic* i2c, MPU6050* mpu6050);
void mpu6050_setSleepMode(XIic* i2c, MPU6050* mpu6050);
void mpu6050_clearSleepMode(XIic* i2c, MPU6050* mpu6050);
void mpu6050_calibration(XIic* i2c, MPU6050* mpu6050, u8 numIterations);
//void mpu6050_readAllReg(XIic* i2c, MPU6050* mpu6050);

#endif
