#include "main.h"

CircularBuffer<int> tmp(20);
MPU9250 imu;
TaskSampleGPS gps(50);

bool isImuActivated = false;
void setup(){
	delay(2000);
	// QP init routine
	QF::init();
	static QEvt l_smlPoolSto[200];
	QF::poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));
	static QSubscrList psPool[200]; 
	QF::psInit(psPool, Q_DIM(psPool));

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
	
	//gps.startAO(1);
	pinMode(13, OUTPUT);

	digitalWriteFast(13, HIGH);
    delay(500);
    digitalWriteFast(13, LOW);
    delay(500);
}

void loop(){
    // digitalWriteFast(13, HIGH);
    // delay(500);
    // digitalWriteFast(13, LOW);
    // delay(500);
    // //Serial.printf("Hello from Sensors \n");
	// if(isImuActivated){
	// 	imu.sample();
	// 	imu.printData();
	// }

	QF::run();
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
