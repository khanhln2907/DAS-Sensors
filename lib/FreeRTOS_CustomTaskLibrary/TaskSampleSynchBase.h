/*
 Created:	5/06/2021 3:02:36 PM
 Author:	Khanh Le

* Brief // 
*/

#pragma once
#include "TaskBase.h"
#include "HardwareSerial.h"
#include "CircularBuffer.h"

class TaskSampleSynchBase : public TaskBase {
public:
    TaskSampleSynchBase();

protected:

    virtual uint16_t getSampleRate() = 0;
    
    virtual void loop() = 0;
    
private:
    
};