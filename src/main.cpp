/*
 Name:		Main.ino
 Created:	4/23/2021 4:31:12 PM
 Author:	Khanh Le
*/

#include "FreeRTOS.h"
#include "ArduinoLog.h"
#include <SPI.h>
#include "TaskSampleGPS.h"

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
#endif

#define DEBUG_PORT Serial
#define DEBUG_BAUD 115200
#define DEBUG_LEVEL LOG_LEVEL_NOTICE

#define GPS_PORT Serial2
#define GPS_BAUD 9600

TaskSampleGPS GPS_h(80);
static TaskHandle_t taskSampleGPS_h;

void heap_caps_alloc_failed_hook(size_t requested_size, uint32_t caps, const char* function_name)
{
    Log.error("%s was called but failed to allocate %d bytes with 0x%X capabilities. \n", function_name, requested_size, caps);
}

// the setup function runs once when you press reset or power the board
void setup() {
    DEBUG_PORT.begin(DEBUG_BAUD);
    DEBUG_PORT.print("Setup() called.\n");    
    Log.begin(DEBUG_LEVEL, &DEBUG_PORT);
    Log.notice("Starting DAS ... \n");

    GPS_h.begin(&GPS_PORT, GPS_BAUD);
    GPS_h.start("TaskSampleGPS", 20000, NULL, 1, &taskSampleGPS_h, app_cpu);

    //xTaskCreatePinnedToCore(taskSampleGPS, "TaskStaticGPS", 5000, NULL, 2, &taskSampleGPS_h, app_cpu);

    //esp_err_t error = heap_caps_register_failed_alloc_callback(heap_caps_alloc_failed_hook);
    //void* ptr = heap_caps_malloc(allocation_size, MALLOC_CAP_DEFAULT);
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