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
    I_UART_Base() = delete;
    I_UART_Base(uint16_t packetSize);

    void setup(HardwareSerial* serial, unsigned long baud);

protected:
    void sample();
    virtual const uint8_t* const getHeaderBytes(uint8_t* nHeader) = 0;
    CircularBuffer<uint8_t> parserFIFO;
    uint16_t _packetSize;


    virtual uint32_t getSampleRate() = 0;
    virtual bool parse() = 0;    
    virtual void write(uint8_t* txBuf, uint16_t len);

private:
    HardwareSerial* _port;    
};
