/**
 * @file main.c
 *
 * @author Omkar Jadhav (omjadhav@pdx.edu)
 * @copyright Portland State University, 2024
 *
 * @brief
 * This program implements a FREERTOS application for angle measurment using MPU6050 Gyro Sensor.
 *
 * The program creates 4 tasks - MenuTask, Data Task, PID Task, Exit Task. Initially, only Menu
 * Task runs while the other tasks remain suspended. The MENU Task expects user interaction on
 * UART Terminal to set mode(Run or Test), if RUN Mode - set the target angle.
 *
 *
 * <pre>
 * MODIFICATION HISTORY:
 * ---------------------
 * Ver   Who             Date            Changes
 * ----- ---- -------- -----------------------------------------------
 * 1.00	Omkar Jadhav	22-Jan-2024		Created Menu, Data, PID Task
 * 1.10	Omkar Jadhav	23-Jan-2024		Added Data, PID Task Suspend
 * 1.20	Omkar Jadhav	24-Jan-2024		Fixed Task Resume Bug and added Exit Task
 * 1.21	Omkar Jadhav	24-Jan-2024		Added Comments and Function Header
 * 1.3	Omkar Jadhav	25-Jan-2024		Added Sleep Mode
 *</pre>
 *
 ******************************************************************************/
/******************* File Includes **************************************/
//#include <string.h>
#include "nexys4io.h"
#include "xil_printf.h"
#include "xiic.h"
#include "MPU6050/mpu6050.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sleep.h"
//#include "semphr.h"

/****************** Macro Definitions ***********************************/
#define 	RUN  					0x1
#define 	TEST					0x2
#define		EXIT					0x3
#define 	UP						0x4
#define		DOWN					0x8
#define		LEFT					0x10
#define		RIGHT					0x20
#define 	TIMER_ID				1
#define 	DELAY_10_SECONDS		10000UL
#define 	DELAY_1_SECOND			1000UL
#define 	TIMER_CHECK_THRESHOLD	9
#define 	PI						3.14159f
#define     NUM_CALIB_VALUES        20

/****************** Global Declarations *******************************/
/****************** User Defined Struct Variable *********************************/
typedef struct
{
  float target_angle;
  float currAngle;
  int mode;
  int valid;
} taskParameters_t;

XIic i2c;
XIic_Config i2c_cfg;
MPU6050 mpu6050;
static float accAngleX, accAngleY, accAngleZ;
static float gyroAngleX = 0.0f, gyroAngleY = 0.0f, gyroAngleZ = 0.0f;
float integralVal = 0.0f;
float error = 0.0f;
float prev_error = 0.0f;
float angle_new = 0.0f;

/****************** Global Variables *********************************/
//xSemaphoreHandle xMutex=NULL;
/******************** Task Handlers *************************************/
static TaskHandle_t xMenuTask;
static TaskHandle_t xDataTask;
static TaskHandle_t xPIDTask;
static TaskHandle_t xExitRunTask;
static taskParameters_t taskParam;

/****************** Task Decalarations *********************************/
void
prvMenuTask(void *parameters);
void
prvDataTask(void *parameters);
void
prvPIDTask(void *parameters);
void
prvExitRunTask(void *parameters);

/****************** Function Prototypes *********************************/
int
platform_init();
int
i2c_init();
//int uart_init();
int
initIic(u16 deviceID, XIic *iic, XIic_Config *config);
//void getRollPitchYaw(float *rawAccValues, float *rawGyroValues, float *roll, float *pitch, float *yaw);

/************************ Main Program ***********************************/
int main()
{
  /*
   * Steps to build firmware:
   * 1. Platform initialization - Contains uart, i2c initialization
   * 2. Test UART setup with the console, terminal
   * 3. Test Sensor integration by reading the device code
   * 4. Display Menu module - Contains test mode and the run mode. User should specify the angle as a setpoint
   * 5. Keep waiting for the user to start the run based on a character
   * 6. Run - Mode:
   * 		a. Get sensor readings
   * 		b. PID Control
   * 		c. Display the error on the leds
   * 		d. Check if the setpoint has been increased by monitoring the button value
   */

  /* Platform initialization */
  if (platform_init() != XST_SUCCESS)
  {
    xil_printf("Platform Initialization failed \n\r");
  }

  /* MPU 6050 Sensor Initialization and Configuration*/
  mpu6050_init(&i2c, &mpu6050);
  usleep(50000);
  mpu6050_gyroCfg(&i2c, &mpu6050);
  mpu6050_accCfg(&i2c, &mpu6050);

  /* Task Creation */
  // Memory is allocated and set to Ready state in the xTaskCreate API
  xTaskCreate(
      prvMenuTask, /* The function that implements the task. */
      (const char *) "Display Menu", /* Text name for the task, provided to assist debugging only. */
      configMINIMAL_STACK_SIZE, /* The stack allocated to the task. */
      NULL, /* The task parameter is not used, so set to NULL */
      2, /* Priority for the task */
      &xMenuTask
      );

  xTaskCreate(
      prvDataTask,
      (const char *) "Get inclination",
      configMINIMAL_STACK_SIZE,
      NULL,
      2,
      &xDataTask
      );

  xTaskCreate(
      prvPIDTask,
      (const char *) "PID",
      configMINIMAL_STACK_SIZE,
      NULL,
      2,
      &xPIDTask
      );

  xTaskCreate(
      prvExitRunTask,
      (const char *) "Exit TASK",
      configMINIMAL_STACK_SIZE,
      NULL,
      2,
      &xExitRunTask
      );

  /* Initially only run the Menu task. This helps to set the set point and be in run mode.
   * Gyro data task, PID Controller task, ExitRun Task can be resumed in the menu task.
   */
  vTaskSuspend(xDataTask);
  vTaskSuspend(xPIDTask);
  vTaskSuspend(xExitRunTask);

  // Start the scheduler and only run the menu task.
  // The user should be able to read and set the pid parameters in run mode
  vTaskStartScheduler();

  //xil_printf("Memory initialization failed to start the scheduler\n\r");

  // Infinite for loop in case the PC returns from the vTaskStartScheduler
  for (;;);

  return 0;
}


/**
 * Menu Display Task
 *
 * Reads the mode provided by the user over UART Terminal at 115200 baud rate.
 * The user can configure and set the following modes - RUN, TEST, SLEEP
 * 	- In RUN Mode, get the angle (in Degrees) from the user. Resumes the other tasks
 * 	  and suspend itself.
 * 	- In TEST Mode, the MPU6050 Sensor is calibrated at a particular angle to determine
 * 	  the error in the readings
 * 	- In SLEEP Mode, the MPU6050 Sensor is set to Sleep Mode via the PWR_MGMT_1 register.
 * 	  Refer to the datasheet to browse more information about the register. The sleep mode
 * 	  can be exited by setting any one of the switches.
 *
 * @note:  2-digit angle should be provided as 0xx
 **/
void prvMenuTask(void *parameters)
{
  char c, mode = 0;
  int angle = 0;

  // Infinite Loop
  for (;;)
  {
    xil_printf("Welcome to Inclinometer Menu\n\r\n\r");
    xil_printf("Please select any one option:\n\r");
    xil_printf("1. Run Mode - R\n\r");
    //xil_printf("2. Test Mode - T\n\r");
    xil_printf("2. Sleep - S\n\r");

    // Get the mode from the user
    mode = inbyte();

    while (mode == '\0')
      mode = inbyte();
    //xil_printf("Mode:%c\n\r", mode);

    switch (mode)
    {
      case 'r':
      case 'R':
        // Get the target angle from the user
        xil_printf("Enter the target angle (in degrees)\n\r");
        c = inbyte();
        angle = (int) (c - '0');
        angle = angle * 10 + (inbyte() - '0');
        angle = angle * 10 + (inbyte() - '0');
        mode = 'r';
        break;
      case 't':
      case 'T':
        xil_printf("Test Mode:\n\r");
        mode = 't';
        break;
      case 's':
      case 'S':
        mode = 's';
        break;

    };

    taskParam.target_angle = angle;
    xil_printf("Target ANGLE: %d\n\r", (int) taskParam.target_angle);

    taskParam.mode = (mode == 'r') ? RUN : (mode == 't') ? TEST : (mode == 's') ? EXIT : -1;
    taskParam.valid = false;

    xil_printf(
        "Mode: %s(%d)\n\r",
        (mode == 'r') ? "RUN" : (mode == 't') ? "TEST" : (mode == 's') ? "SLEEP" : "Undefined mode",
        mode);

    angle = 0;

    if (mode == 'r')
    {
      /* In RUN mode, resume the Gyro Data task, PID Controller Task,
       * Exit Run Task while suspending the Menu task
       */
      vTaskResume(xDataTask);
      vTaskResume(xPIDTask);
      vTaskResume(xExitRunTask);
      vTaskSuspend(xMenuTask);
    }
    else if (mode == 's')
    {
      int exit = 0;

      // Put the MPU6050 Sensor to sleep mode
      mpu6050_setSleepMode(&i2c, &mpu6050);

      // Continuously read the switches and exit if non-zero
      do
      {
        exit = NX4IO_getSwitches();
        xil_printf("Sleep Mode\n\r");

      } while (exit == 0);

      // MPU6050 Sensor wake up
      mpu6050_clearSleepMode(&i2c, &mpu6050);
    }
    else if(mode == 't')
    {
      mpu6050_calibration(&i2c, &mpu6050, NUM_CALIB_VALUES);
    }
  }
}

/**
 * MPU6050 Gyro Data Task
 *
 * Reads angle from the MPU6050 sensor over I2C.
 *
 * The task reads X and Y data angle with respect to a particular plane and
 **/
void prvDataTask(void *parameters)
{
  u8 currRawGyroVal[6], currRawAccVal[6];
  int axis = SET_AXIS_X;

  for (;;)
  {
    mpu6050_getGyroData(&i2c, &mpu6050, &currRawGyroVal[axis - SET_AXIS_X], axis, 6);
    mpu6050_getAccData(&i2c, &mpu6050, &currRawAccVal[axis - SET_AXIS_X], axis, 6);

    //usleep(50000);

    short GyroX = currRawGyroVal[0] << 8 | currRawGyroVal[1];
    short GyroY = currRawGyroVal[2] << 8 | currRawGyroVal[3];
    short GyroZ = currRawGyroVal[4] << 8 | currRawGyroVal[5];

    if ((int) (GyroX / 131.0f) != -1)
      gyroAngleX += (int) (GyroX / 131.0f);

    if ((int) (GyroY / 131.0f) != -1)
          gyroAngleY += (int) (GyroY / 131.0f);

    if ((int) (GyroX / 131.0f) != -1)
          gyroAngleZ += (int) (GyroZ / 131.0f);

    vTaskDelay(pdMS_TO_TICKS(50));

    mpu6050.roll = gyroAngleX;
    mpu6050.pitch = gyroAngleY;
    mpu6050.yaw = gyroAngleZ;
  }

  vTaskDelete(NULL);
}

/**
 * Exit Run  Task
 *
 * Read switches constantly to exit from the run mode. Run mode is exited when
 * non - zero. Resume the Menu task by suspending the pi task and data task and itself.
 **/
void prvExitRunTask(void *parameters)
{
  u16 c = 0x0000;

  for (;;)
  {
    // Constantly read the value from the switches to exit from the run mode
    c = NX4IO_getSwitches();

    if (c != 0)
    {
      taskParam.target_angle = 0.0f;
      taskParam.currAngle = 0.0f;
      mpu6050.roll = 0.0f;
      gyroAngleX = 0.0f;
      integralVal = 0.0f;
      error = 0.0f;
      prev_error = 0.0f;
      angle_new = 0.0f;

      //xil_printf("c: %d\n\r", c);
      vTaskSuspend(xDataTask);
      vTaskSuspend(xPIDTask);
      vTaskResume(xMenuTask);
      vTaskSuspend(NULL);
    }
  }

  vTaskDelete(NULL);
}

/**
 * PID Controller Task
 **/
void prvPIDTask(void *parameters)
{
  float kp = 1.2f;
  float ki = 0.2f;
  float kd = 0.06f;
  float time_step = 0.002f;
  u8 btns_val;

  for (;;)
  {
    //xSemaphoreTake(xMutex, portMAX_DELAY);
    float currAngle = (float) mpu6050.roll;
    float target_angle = (float) taskParam.target_angle;

    if (taskParam.mode == RUN)
    {
      prev_error = error;
      error = target_angle - (currAngle);

      integralVal += error * time_step;

      angle_new = currAngle + kp * error + kd * ((error - prev_error) / time_step) + ki * integralVal;

      btns_val = NX4IO_getBtns();

      switch (btns_val)
      {
        case UP:
          target_angle += 10;
          break;

        case DOWN:
          target_angle -= 10;
          break;

          /* Need to add the logic here
           * Decide if buttons read should be in a different task
           */
        case LEFT:
          break;

        case RIGHT:
          break;

      };

      xil_printf("%d.%02d ", (int) currAngle, (int) (currAngle - (int) currAngle) * 1000);
      xil_printf("%d.%02d ", (int) target_angle, (int) (target_angle - (int) target_angle) * 1000);
      xil_printf("%d.%02d ", (int) error, (error - (int) error) * 1000);
      xil_printf("%d.%02d\n\r", (int) angle_new, (int) (angle_new - (int) angle_new) * 1000);

      taskParam.target_angle = target_angle;
    }
  }

  vTaskDelete(NULL);
}

/**
 * Platform Initialization
 *
 * Initialize the Nexys IO module, I2C Module
 **/
int platform_init()
{
  uint32_t status;

  // ASSERT(XST_SUCCESS != uart_init());
  if (XST_SUCCESS != i2c_init())
  {
    xil_printf("I2C Initialization Failed");
    return XST_FAILURE;
  }
  // initialize the Nexys4 driver and (some of)the devices
  status = (uint32_t) NX4IO_initialize(XPAR_NEXYS4IO_0_S00_AXI_BASEADDR);

  if (status != XST_SUCCESS)
  {
    return XST_FAILURE;
  }

  NX4IO_setLEDs(0x13);

  return status;
}

/**
 * I2C Initialization
 *
 **/
int i2c_init(void)
{
  i2c_cfg.DeviceId = XPAR_AXI_IIC_0_DEVICE_ID; /**< Unique ID  of device */
  i2c_cfg.BaseAddress = XPAR_AXI_IIC_0_BASEADDR; /**< Device base address */
  i2c_cfg.Has10BitAddr = XPAR_AXI_IIC_0_TEN_BIT_ADR; /**< Does device have 10 bit address decoding */
  i2c_cfg.GpOutWidth = XPAR_AXI_IIC_0_GPO_WIDTH; /**< Number of bits in general purpose output */

  i2c.BaseAddress = XPAR_AXI_IIC_0_BASEADDR; /**< Device base address */
  i2c.Has10BitAddr = XPAR_AXI_IIC_0_TEN_BIT_ADR; /**< Does device have 10 bit address decoding */
  i2c.GpOutWidth = XPAR_AXI_IIC_0_GPO_WIDTH; /**< Number of bits in general purpose output */

  initIic(XPAR_AXI_IIC_0_DEVICE_ID, &i2c, &i2c_cfg);

  return XST_SUCCESS;
}

int initIic(u16 deviceID, XIic *iic, XIic_Config *config)
{
  config = XIic_LookupConfig(deviceID);

  return XIic_CfgInitialize(iic, config, config->BaseAddress);
}

/*
 void getRollPitchYaw(float *rawAccValues, float *rawGyroValues, float *roll, float *pitch, float *yaw)
 {
 float elapsedTime = 0.001f;

 accAngleX = (atan(rawAccValues[1] / sqrt(pow(rawAccValues[0], 2) + pow(rawAccValues[2], 2))) * 180 / PI) - 0.58; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
 accAngleY = (atan(-1 * rawAccValues[0] / sqrt(pow(rawAccValues[1], 2) + pow(rawAccValues[2], 2))) * 180 / PI) + 1.58; // AccErrorY ~(-1.58)
 gyroAngleX = gyroAngleX + rawGyroValues[0] * elapsedTime; // deg/s * s = deg
 gyroAngleY = gyroAngleY + rawGyroValues[1] * elapsedTime;
 *yaw =  *yaw + rawGyroValues[2] * elapsedTime;
 *roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
 *pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

 xil_printf("Roll: %f Pitch: %f Yaw: %f/n", *roll, *pitch, *yaw);
 }

 */

//xil_printf("\n\r");
// Might need a mutex for this variable.
// The variable can be read constantly by the pid task
//xil_printf("Data: %d\n\r", currRawGyroVal[SET_AXIS_X - SET_AXIS_X]);
// taskParam.currAngle = currRawVal;
// Need mutex here to store the local memories into shared memory
//getRollPitchYaw(currRawGyroVal, currRawAccVal, &roll, &pitch, &yaw);
//accAngleX = (atan(currRawAccVal[1] / sqrt(pow(currRawAccVal[0], 2) + pow(currRawAccVal[2], 2))) * 180 / PI) - 0.58f; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
//accAngleY = (atan(-1 * currRawAccVal[0] / sqrt(pow(currRawAccVal[1], 2) + pow(currRawAccVal[2], 2))) * 180 / PI) + 1.58; // AccErrorY ~(-1.58)
//gyroAngleX =  (currRawGyroVal[0] + 0.56f) * 0.001f; // deg/s * s = deg
//gyroAngleY =  (currRawGyroVal[1] - 2.0f) * 0.001f;
//yaw =  (currRawGyroVal[2] + 0.79f) * 0.001f;
//roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
//pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
//short AccXLSB = currRawAccVal[1] << 8 | currRawAccVal[0];
//short AccYLSB = currRawAccVal[3] << 8 | currRawAccVal[2];
//short AccZLSB = currRawAccVal[5] << 8 | currRawAccVal[4];
//float  AccX=(float)(AccXLSB/4096) + 5;
//			float  AccY=(float)(AccYLSB/4096) + 5;
//			float  AccZ=(float)(AccZLSB/4096) + 5;
//			float  AngleRoll=atan(AccY/sqrt(AccX*AccX+AccZ*AccZ))*1/(3.142/180);
//			float  AnglePitch=0-atan(AccX/sqrt(AccY*AccY+AccZ*AccZ))*1/(3.142/180);
//mpu6050_getAccData(&i2c, &mpu6050, &currRawAccVal[axis - SET_AXIS_X], axis);
//mpu6050.rawGyroVal[axis - SET_AXIS_X] = (currRawGyroVal[axis - SET_AXIS_X] / 131.0f);
//mpu6050.rawAccVal[axis - SET_AXIS_X] = currRawAccVal[axis - SET_AXIS_X] / 16384.0f;
//xil_printf("Data: %d ", currRawGyroVal[axis - SET_AXIS_X]);
//xSemaphoreGive( xMutex );
//xil_printf("Roll: %d Pitch: %d Yaw : %d\n\r", (int)(0.96f * gyroAngleX + 0.04f * accAngleX), (int)(0.96f * gyroAngleY + 0.04f * accAngleY), (int)yaw);
//xil_printf("Roll: %d Pitch: %d\n\r", (int)gyroAngleX, (int)gyroAngleY);
//xil_printf("Roll: %x Pitch: %x\n\r", (short)(AccXLSB), (short)(AccYLSB));
//mpu6050.pitch = pitch;
//mpu6050.yaw = yaw;
//taskParam.valid = true;
//}
//axis = (axis + 1);
//if(axis > SET_AXIS_Z) axis = SET_AXIS_X;
//xil_printf("target_angle : %d\n\r", (int)target_angle);
//float valid = taskParam.valid;
//xSemaphoreGive(xMutex);
//xil_printf("Yes it definitely started FAILED!!!!!!!!!!!\n\r");
// mutex here to create local copies
/*
 for( axis = SET_AXIS_X; axis < SET_AXIS_Z; axis++)
 {
 currRawGyroVal[axis - SET_AXIS_X] = mpu6050.rawGyroVal[axis - SET_AXIS_X];
 currRawAccVal[axis - SET_AXIS_X]  = mpu6050.rawAccVal[axis - SET_AXIS_X];
 }
 */
// free mutex
//getRollPitchYaw(currRawGyroVal, currRawAccVal, &roll, &pitch, &yaw);
//NX4IO_setLEDs((int) (target_angle - angle_new));
//xSemaphoreTake(xMutex,portMAX_DELAY );
//taskParam.valid = false;
//xSemaphoreGive(xMutex);
//xil_printf("Valid_pid: %d \n\r", taskParam.valid);
/****************************************************************************/
/**
 * UART Initialization
 *
 *****************************************************************************
 int uart_init(void)
 {
 // Configure the uart Module
 uart.IsReady= true;
 uart.RegBaseAddress = XPAR_UARTLITE_1_BASEADDR;
 XUartLite_Config uart_cfg;
 uart_cfg.BaudRate = 115200;
 uart_cfg.DataBits= 8;
 uart_cfg.RegBaseAddr = XPAR_UARTLITE_1_BASEADDR;
 uart_cfg.UseParity = false;
 uart_cfg.ParityOdd = true;

 // Initialize the uart
 XUartLite_CfgInitialize(&uart, &uart_cfg, XPAR_UARTLITE_1_BASEADDR);

 return XUartLite_Initialize(&uart, XPAR_UARTLITE_1_DEVICE_ID);
 }
 */
