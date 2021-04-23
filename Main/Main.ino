/*
 Name:		Main.ino
 Created:	4/23/2021 4:31:12 PM
 Author:	khanh
*/

#if CONFIG_FREERTOS_UNICORE
    static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
#endif

#include "FreeRTOS.h"
#include "Sensors.h"
#include "ArduinoLog.h"

#define DEBUG_PORT Serial
#define DEBUG_BAUD 115200
#define DEBUG_LEVEL LOG_LEVEL_NOTICE

#define GPS_PORT Serial2
#define GPS_BAUD 9600

TaskSampleGPS GPS_h;
static TaskHandle_t taskSampleGPS_h;

// the setup function runs once when you press reset or power the board
void setup() {
    DEBUG_PORT.begin(DEBUG_BAUD);
    DEBUG_PORT.print("Setup() called.\n");    
    Log.begin(DEBUG_LEVEL, &DEBUG_PORT);
    Log.notice("Starting DAS ... \n");

    GPS_h.begin(&GPS_PORT, GPS_BAUD);
    GPS_h.start("TaskSampleGPS", 5000, NULL, 1, &taskSampleGPS_h, app_cpu);

    //xTaskCreatePinnedToCore(taskSampleGPS, "TaskStaticGPS", 5000, NULL, 2, &taskSampleGPS_h, app_cpu);

}

// the loop function runs over and over again until power down or reset
void loop() {
  
}

//void taskSampleGPS(void* parameter) {
    //    // Sample the GPS here
    //    while (1) {
    //        //portENTER_CRITICAL(&my_mutex);
    //        tmpVar++;
    //        //portEXIT_CRITICAL(&my_mutex);
    //        Log.notice("%d Task Static: %d\n", (unsigned long)millis(), tmpVar);
    //        vTaskDelay(130 / portTICK_PERIOD_MS);
    //    }
    //}