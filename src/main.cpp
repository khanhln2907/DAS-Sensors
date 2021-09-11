#include <Arduino.h>
#include "BasicType.h"
#include "ArduinoLog.h"
#include "HardwareSerial.h"
#include "CircularBuffer.h"

//#define USING_MAKEFILE
//#define __MK66FX1M0__

CircularBuffer<int> tmp(20);

void setup(){
    pinMode(13, OUTPUT);
}

void loop(){
    digitalWriteFast(13, HIGH);
    delay(500);
    digitalWriteFast(13, LOW);
    delay(500);
    Serial.printf("Hello from Sensors \n");
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