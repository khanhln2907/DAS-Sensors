/*
 Created:	5/06/2021 23:10:36 PM
 Author:	Khanh Le

* Brief // 
*/

#pragma once
#include "TaskBase.h"
#include "TaskSampleSynchBase.h"
#include "CommonType.h"
#include "IMUType.h"

class TaskSampleMPUBase : public TaskSampleSynchBase {
public:
    TaskSampleMPUBase();

protected:
    virtual uint16_t getTaskRate() = 0;
    virtual void loop(void* data) = 0;

    Sample<MPU_Type> _sample;
private:
 
};