#include "TaskSampleGPS.h"
#include "HardwareSerial.h"
#include "ArduinoLog.h"

static const char* LOG_TAG = "TaskSampleGPS ";

TaskSampleGPS::TaskSampleGPS() : TaskSampleUARTBase()
{
}

void TaskSampleGPS::run(void* data)
{
	while (1) {
		Log.notice("%d Sampling GPS\n", (unsigned long)millis());
		taskDelayMllis(100 / portTICK_PERIOD_MS);
	}
}
