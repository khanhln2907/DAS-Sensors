/*
 Name:		TestGPSBlox7.ino
 Created:	4/23/2021 3:02:36 PM
 Author:	Khanh Le

* Brief //
*/

#pragma once
#include "TaskBase.h"
#include "HardwareSerial.h"

class TaskSampleUARTBase : public TaskBase {
public:
    TaskSampleUARTBase();

    void begin(HardwareSerial* serial, unsigned long baud);

protected:
    HardwareSerial* _port;
    
private:
    

};
