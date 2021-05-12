/*
 Name:		TestGPSBlox7.ino
 Created:	5/11/2021 9:16:36 PM
 Author:	Khanh Le

* Brief //
*/

#pragma once
#include "TaskBase.h"
#include "HardwareSerial.h"
#include "CircularBuffer.h"
#include <vector>

#define TX_FIFO_SIZE_BYTES 500
#define RX_FIFO_SIZE_BYTES 128

#define NUMEL(arr) (int) sizeof(arr)/sizeof(arr[0])

class TaskUARTLogging : public TaskBase {
public:
    TaskUARTLogging();
    //TaskUARTLogging(uint16_t txFIFOSize = TX_FIFO_SIZE_BYTES, uint16_t rxFIFOSize = RX_FIFO_SIZE_BYTES);

    void begin(HardwareSerial* serial, unsigned long baud);

protected:
    HardwareSerial* _port;
    
    void loop(void* data);
    
    CircularBuffer<uint8_t> _rxFIFO;
    CircularBuffer<uint8_t> _txFIFO;

    virtual uint16_t getTaskRate();

private:

};