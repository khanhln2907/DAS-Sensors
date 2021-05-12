/*
 Created:	5/06/2021 3:02:36 PM
 Author:	Khanh Le

* Brief // 
*/

#pragma once
#include "TaskBase.h"
#include "HardwareSerial.h"

class TaskSampleSynchBase : public TaskBase {
public:
    TaskSampleSynchBase();

protected:

    virtual uint16_t getTaskRate() = 0;
    
    virtual void loop(void* data) = 0;
    
private:
    
};