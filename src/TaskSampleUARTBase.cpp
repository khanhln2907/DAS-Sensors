#include "TaskSampleUARTBase.h"

TaskSampleUARTBase::TaskSampleUARTBase() : TaskBase()
{

}

void TaskSampleUARTBase::begin(HardwareSerial* serial, unsigned long baud)
{
	this->_port = serial;
	_port->begin(baud);
}

