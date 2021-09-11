#include <Arduino.h>
#include "BasicType.h"
#include "ArduinoLog.h"
#include "HardwareSerial.h"
#include "CircularBuffer.h"
#include <qpcpp.hpp>
#include "MPU9250.h"
#include "TaskSampleGPS.h"

using namespace QP;

CircularBuffer<int> tmp(20);

MPU9250 imu;
bool isImuActivated = false;

TaskSampleGPS gps(50);

void setup(){
	delay(2000);
	Log.begin(LOG_LEVEL_NOTICE, &Serial);

	Log.notice("Fnished setup logging \n");

	imu.setup(&SPI1, 10);
	isImuActivated = (imu.begin() == MPU_CONFIG_STATUS::MPU_OK);
	if(isImuActivated){
		Serial.printf("Success \n");
	}else{
		Serial.printf("Failed: %d\n");
	}

	//gps.begin(&Serial1, 115200);

	pinMode(13, OUTPUT);
}

void loop(){
    digitalWriteFast(13, HIGH);
    delay(500);
    digitalWriteFast(13, LOW);
    delay(500);
    //Serial.printf("Hello from Sensors \n");
	if(isImuActivated){
		imu.sample();
		imu.printData();
	}
}


extern "C" int main(void)
{
#ifdef USING_MAKEFILE
	// To use Teensy 3.0 without Arduino, simply put your code here.
	// For example:
	setup();
	while (1) {
    loop();
	}

#else
	// Arduino's main() function just calls setup() and loop()....
	setup();
	while (1) {
		loop();
		yield();
	}
#endif
}