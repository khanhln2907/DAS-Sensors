#include <Arduino.h>
#include "BasicType.h"
#include "ArduinoLog.h"
#include "HardwareSerial.h"
#include "CircularBuffer.h"
#include <qpcpp.hpp>
#include "MPU9250.h"
#include "TaskSampleGPS.h"

using namespace QP;

namespace BoardConfig{
    void blink(uint8_t pin, uint16_t interval_ms){
        digitalWriteFast(pin, HIGH);
        delay(interval_ms);
        digitalWriteFast(pin, LOW);
        delay(interval_ms);
    }

    void toogle(uint8_t pin){
        static bool state = HIGH;
        digitalWriteFast(pin, !state);
        state = !state;
        Serial.printf("%lu, State: %d\n", (unsigned long) micros(), state);
    }
}

using namespace BoardConfig;


extern "C" Q_NORETURN Q_onAssert(char_t const * const module, int_t const location)
{
	Serial.printf("Assert \n");
	Serial.flush();
	abort();
}

#define TICKS_PER_SECOND 100
// systick handler............................................................
void sys_tick_handler() {
	//Log.notice("Systick!\n");
	QP::QF::TICK_X(0, (void*)0); // process time events for tick rate 0
}

IntervalTimer _systick;
constexpr auto _systickPeriod = 1.e6 / (double) (TICKS_PER_SECOND);
extern "C" void QP::QF::onStartup()
{
	Serial.printf("Starting QP Framework... \n");
	uint8_t systickPriority = 128; // QF_AWARE_ISR_CMSIS_PRI
	_systick.priority(systickPriority);
	_systick.begin(sys_tick_handler, _systickPeriod);

	// Serial.print(F("Enabling CPU cycle counter...\n"));
	// ARM_DEMCR |= ARM_DEMCR_TRCENA;
	// ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
}

extern "C" void QP::QV::onIdle()
{
	static int cnt = 0;
	cnt++;
	if(cnt % (TICKS_PER_SECOND * 10000U) == 0){
        Serial.printf("Idle \n");
        toogle(LED_BUILTIN);
    }
	QF_INT_ENABLE(); // simply re-enable interrupts
}
