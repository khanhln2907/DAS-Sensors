/*
 Name:		TestGPSBlox7.ino
 Created:	4/23/2021 3:02:36 PM
 Author:	Khanh Le

* Brief //
*/

#pragma once
#include "TaskBase.h"
#include "HardwareSerial.h"
#include "CircularBuffer.h"
#include <vector>



#define NUMEL(arr) (int) sizeof(arr)/sizeof(arr[0])

class TaskSampleUARTBase : public TaskBase {
public:
    TaskSampleUARTBase() = delete;
    TaskSampleUARTBase(uint16_t packetSize);

    void begin(HardwareSerial* serial, unsigned long baud);

protected:
    HardwareSerial* _port;
    
    void loop(void* data);
    
    virtual const uint8_t* const getHeaderBytes(uint8_t* nHeader) = 0;


    //virtual bool checkHeaderBytes(uint8_t* headerBuffer) = 0;

    CircularBuffer<uint8_t> parserFIFO;

    //const std::vector<uint8_t> headerByteVec;

    virtual bool parse() = 0;

    uint16_t _packetSize;

private:


};
