/*
 Name:		TestGPSBlox7.ino
 Created:	4/23/2021 3:02:36 PM
 Author:	Khanh Le

* Brief //
*/

#pragma once
#include "HardwareSerial.h"
#include "CircularBuffer.h"
#include <vector>



#define NUMEL(arr) (int) sizeof(arr)/sizeof(arr[0])

class I_UART_Base {
public:
    TaskSampleUARTBase() = delete;
    TaskSampleUARTBase(uint16_t packetSize);

    void begin(HardwareSerial* serial, unsigned long baud);

protected:
    HardwareSerial* _port;
   
    virtual uint16_t getSampleRate() = 0;
    
    virtual const uint8_t* const getHeaderBytes(uint8_t* nHeader) = 0;

    CircularBuffer<uint8_t> parserFIFO;

    virtual bool parse() = 0;

    
private:
    uint16_t _packetSize;


};
