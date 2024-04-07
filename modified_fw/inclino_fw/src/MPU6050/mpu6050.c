/**
 * @file 	mpu6050.c
 * @author 	Omkar Jadhav (omjadhav@pdx.edu)
 *
 * @brief	Contains function definitions for MPU6050 sensor
 *
 * @rev	 1.0  - Created file and added definitions for initialization
 * 			  - Added basic gryo configuration (only Full Scale Range) and X axis data capture
 */
#include "mpu6050.h"
#include "xiic.h"

/**
 * MPU6050 Init
 *
 * Read MPU6050 Sensor ID and perform reset
 *
 * Perform an IIC read to Device ID register, reset the mpu6050 sensor and configure the digital
 * low pass filter to disable external signal sampling and acc, gyro bandwidth to 10 Hz for filtering
 */
void mpu6050_init(XIic* i2c, MPU6050* mpu6050)
{
  u8 oBuf[2]; //= {PWR_MGMT_1;
  u8 iBuf = 0x00;

  mpu6050->roll = 0.0f;
  mpu6050->pitch = 0.0f;
  mpu6050->yaw = 0.0f;
  mpu6050->xAxisOffset = 0.0f;
  mpu6050->yAxisOffset = 0.0f;
  mpu6050->zAxisOffset = 0.0f;

  // Read the device id to verify successful i2c transfers
  oBuf[0] = WHO_AM_I;
  oBuf[1] = 0x00;

  XIic_Send(i2c->BaseAddress, 0x68, oBuf, 1, XIIC_REPEATED_START);
  XIic_Recv(i2c->BaseAddress, 0x68, &iBuf, 1, XIIC_STOP);
  xil_printf("WHOAMI = 0x%x\n\r", iBuf);

  // Reset the sensor
  oBuf[0] = PWR_MGMT_1;
  oBuf[1] = 0x00;
  XIic_Send(i2c->BaseAddress, 0x68, oBuf, 2, XIIC_STOP);

  // Check the power mode configuration and clock source
  oBuf[0] = PWR_MGMT_1;
  oBuf[1] = 0x00;
  XIic_Send(i2c->BaseAddress, 0x68, oBuf, 1, XIIC_REPEATED_START);
  XIic_Recv(i2c->BaseAddress, 0x68, &iBuf, 1, XIIC_STOP);
  xil_printf("Power = 0x%x\n\r", iBuf);

  /* Configure the Digital Low Pass Filter (DLPF) for setting both
   * the gyroscopes and accelerometers
   * Set DPGF_CFG to 0x5 and EXT_SYNC_SET to 0x0
   * Following configurations occur:
   * 1. Disable sampling from AD0 pin.
   * 2. Set Accelerometer: Bandwidth: 10 Hz (Delay 13.8 ms)
   * 3. Set Gyroscope: Bandwidth: 10 Hz (Delay 13.4 ms)
   */
  oBuf[0] = CONFIG;
  oBuf[1] = 0x05;
  XIic_Send(i2c->BaseAddress, 0x68, (u8 *) &oBuf, 2, XIIC_STOP);
}

/**
 * @func: MPU6050 Gyro Config
 *
 * Set Gyro Sensitivity
 *
 * Set FS_SEL to 0x0 (Range: +/-250deg/s)
 */
void mpu6050_gyroCfg(XIic* i2c, MPU6050* mpu6050)
{
  u8 iBuf;
  u8 oBuf[2];

  // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
  oBuf[0] = GYRO_CONFIG;
  oBuf[1] = 0x00;
  XIic_Send(i2c->BaseAddress, 0x68, (u8 *) oBuf, 2, XIIC_STOP);

  // Check if the gyro is updated correctly
  oBuf[0] = GYRO_CONFIG;
  XIic_Send(i2c->BaseAddress, 0x68, oBuf, 1, XIIC_REPEATED_START);
  XIic_Recv(i2c->BaseAddress, 0x68, &iBuf, 1, XIIC_STOP);
  xil_printf("GyroConfig = 0x%x\n\r", iBuf);
}

/**
 * MPU6050 Acc Config
 *
 * Set Acc Sensitivity
 *
 * Set AFS_SEL to 0x1 (Range: +/-4g)
 */
void mpu6050_accCfg(XIic* i2c, MPU6050* mpu6050)
{
  u8 iBuf;
  u8 oBuf[2];

  oBuf[0] = ACC_CONFIG;
  oBuf[1] = 0x10;
  XIic_Send(i2c->BaseAddress, 0x68, oBuf, 2, XIIC_STOP);

  oBuf[0] = ACC_CONFIG;
  XIic_Send(i2c->BaseAddress, 0x68, oBuf, 1, XIIC_REPEATED_START);
  XIic_Recv(i2c->BaseAddress, 0x68, &iBuf, 1, XIIC_STOP);
  xil_printf("AccConfig = 0x%x\n\r", iBuf);
}

/**
 * MPU6050 Acc Data
 *
 * Get Most recent accelerometer measurements
 */
void mpu6050_getAccData(XIic* i2c, MPU6050* mpu6050, u8 *angle_actual, int axis, int numReg)
{
  u8 iBuf;
  u8 oBuf;

  // get the higher nibble data of a particular axis
  //XIic_SetAddress(i2c, XII_ADDR_TO_SEND_TYPE, (axis == SET_AXIS_X) ? GYRO_XOUT_2 :
  //										(axis == SET_AXIS_Y) ? GYRO_YOUT_2 : GYRO_ZOUT_2);
  oBuf = (axis == SET_AXIS_X) ? ACC_XOUT_2 : (axis == SET_AXIS_Y) ? ACC_YOUT_2 : ACC_ZOUT_2;
  XIic_Send(i2c->BaseAddress, 0x68, (u8 *) &oBuf, 1, XIIC_REPEATED_START);
  XIic_Recv(i2c->BaseAddress, 0x68, (u8 *) &iBuf, numReg, XIIC_STOP);
}

/**
 * MPU6050 Gyro Data
 *
 * Get Most recent gyroscope measurements
 */
void mpu6050_getGyroData(XIic* i2c, MPU6050* mpu6050, u8 *angle_actual, int axis, int numReg)
{
  u8 iBuf;
  u8 oBuf;

  // get the higher nibble data of a particular axis
  // XIic_SetAddress(i2c, XII_ADDR_TO_SEND_TYPE, (axis == SET_AXIS_X) ? GYRO_XOUT_2 :
  //										(axis == SET_AXIS_Y) ? GYRO_YOUT_2 : GYRO_ZOUT_2);
  oBuf = (axis == SET_AXIS_X) ? GYRO_XOUT_2 : (axis == SET_AXIS_Y) ? GYRO_YOUT_2 : GYRO_ZOUT_2;

  XIic_Send(i2c->BaseAddress, 0x68, (u8 *) &oBuf, 1, XIIC_REPEATED_START);
  XIic_Recv(i2c->BaseAddress, 0x68, (u8 *) angle_actual, numReg, XIIC_STOP);

}

/**
 * MPU6050 Set Sleep Mode
 *
 * Place the sensor into sleep mode by stopping the continuous sample and measurement capture
 */
void mpu6050_setSleepMode(XIic* i2c, MPU6050* mpu6050)
{
  u8 oBuf[2] = { PWR_MGMT_1, 0x40 };

  XIic_Send(i2c->BaseAddress, 0x68, (u8 *) &oBuf, 2, XIIC_STOP);
}

/**
 * MPU6050 Clear Sleep Mode
 *
 * Wake up MPU6050 from sleep mode and resume measurement capture
 */
void mpu6050_clearSleepMode(XIic* i2c, MPU6050* mpu6050)
{

  u8 oBuf[2] = { PWR_MGMT_1, 0x00 };

  XIic_Send(i2c->BaseAddress, 0x68, (u8 *) &oBuf, 2, XIIC_STOP);
}

/**
 * MPU6050 Calibration
 *
 * Calculate mean error in idle position.
 */
void mpu6050_calibration(XIic* i2c, MPU6050* mpu6050, u8 numIterations)
{
  u8 axis, iteration;
  u8 currRawGyroVal[6];
  float gyroAngleX = 0.0f, gyroAngleY = 0.0f, gyroAngleZ = 0.0f;

  float sum;

  for (iteration = 0; iteration < numIterations; iteration++)
  {
    mpu6050_getGyroData(i2c, mpu6050, &currRawGyroVal, SET_AXIS_X, 6);

    short GyroX = currRawGyroVal[0] << 8 | currRawGyroVal[1];
    short GyroY = currRawGyroVal[2] << 8 | currRawGyroVal[3];
    short GyroZ = currRawGyroVal[4] << 8 | currRawGyroVal[5];

    gyroAngleX += (int) (GyroX / 131.0f);
    gyroAngleY += (int) (GyroY / 131.0f);
    gyroAngleZ += (int) (GyroZ / 131.0f);
  }

  mpu6050->xAxisOffset = gyroAngleX / numIterations;
  mpu6050->yAxisOffset = gyroAngleY / numIterations;
  mpu6050->zAxisOffset = gyroAngleZ / numIterations;
}
/*
 void mpu6050_readAllReg(XIic* i2c, MPU6050* mpu6050)
 {
 u8 oBuf = PWR_MGMT_1;
 u8 iBuf = 0x00;

 iBuf = 0x75;

 //XIic_Send(i2c->BaseAddress, 0x75, (u8 *)&iBuf,1,XIIC_REPEATED_START);
 XIic_Recv(i2c->BaseAddress, MPU6050_ID_ADDR,(u8 *)&oBuf,1,XIIC_STOP);
 xil_printf("WHOAMI = 0x%x\n\r",oBuf);

 iBuf = PWR_MGMT_1;
 oBuf = 0x01;
 XIic_Send(i2c->BaseAddress, PWR_MGMT_1,(u8 *)&oBuf,1,XIIC_REPEATED_START);
 XIic_Recv(i2c->BaseAddress, PWR_MGMT_1,(u8 *)&oBuf,1,XIIC_STOP);
 xil_printf("Power = 0x%x\n\r",oBuf);

 iBuf = GYRO_CONFIG;
 oBuf = 0x10;
 XIic_Send(i2c->BaseAddress, GYRO_CONFIG,(u8 *)&oBuf,1,XIIC_REPEATED_START);
 XIic_Recv(i2c->BaseAddress, GYRO_CONFIG,(u8 *)&oBuf,1,XIIC_STOP);
 xil_printf("GyroConfig = 0x%x\n\r",oBuf);

 iBuf = 0x6A;
 oBuf = 0x00;
 //XIic_Send(i2c->BaseAddress, 0x6A,(u8 *)&iBuf,1,XIIC_REPEATED_START);
 XIic_Recv(i2c->BaseAddress, 0x6A,(u8 *)&oBuf,1,XIIC_STOP);
 xil_printf("UserCntrl = 0x%x\n\r", (u8)oBuf);

 XIic_Recv(i2c->BaseAddress, GYRO_XOUT_1, (u8 *)&iBuf, 1, XIIC_STOP);
 xil_printf("Data: %d\n\r", iBuf);


 }
 */