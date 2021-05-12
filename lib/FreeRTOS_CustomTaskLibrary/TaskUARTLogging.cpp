#include "TaskUARTLogging.h"

TaskUARTLogging::TaskUARTLogging(): TaskBase(), _txFIFO(TX_FIFO_SIZE_BYTES), _rxFIFO(RX_FIFO_SIZE_BYTES)
{

}

void TaskUARTLogging::begin(HardwareSerial* serial, unsigned long baud){

}

uint16_t TaskUARTLogging::getTaskRate(){
        return 1000;
}

void TaskUARTLogging::loop(void* data){
        ;
}
