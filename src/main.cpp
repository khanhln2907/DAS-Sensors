/*
 Name:		Main.ino
 Created:	4/23/2021 4:31:12 PM
 Author:	Khanh Le
*/

#include "FreeRTOS.h"
//#include "streambuffer.h"
#include <ArduinoLog.h>
#include <SPI.h>
#include "TaskSampleGPS.h"
#include "TaskSampleMPU9250.h"
//#include "TaskUARTLogging.h"
//#include "MPU9250.h"
#include "PortConfig.h"
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
#endif

TaskSampleGPS GPS_h(80);
static TaskHandle_t taskSampleGPS_h;

TaskSampleMPU9250 IMU_t_h;

//TaskUARTLogging Logging_h;

//StreamBufferHandle_t xStreamBufferCreate( size_t xBufferSizeBytes,
 //                                          size_t xTriggerLevelBytes );

// the setup function runs once when you press reset or power the board
void setup() {
    DEBUG_PORT.begin(DEBUG_BAUD);
    DEBUG_PORT.print("Setup() called.\n");    
    Log.begin(DEBUG_LEVEL, &DEBUG_PORT);
    Log.notice("Starting DAS ... \n");

    // GPS_h.begin(&GPS_PORT, GPS_BAUD);
    // GPS_h.start("TaskSampleGPS", 20000, NULL, 1, &taskSampleGPS_h, app_cpu);

    //imu.setup(&Wire, MPU_I2C_ADDR);
    IMU_t_h.begin(&SPI, 2);
}

// the loop function runs over and over again until power down or reset
void loop() {
    
}

