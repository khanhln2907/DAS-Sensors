/*
 Created:	5/06/2021 23:10:36 PM
 Author:	Khanh Le

* Brief // 
*/

#pragma once
#include "TaskSampleMPUBase.h"

class TaskSampleMPU9250 : public TaskSampleMPUBase {
public:
    TaskSampleMPU9250();

protected:

    virtual uint16_t getSampleRate();
    virtual void loop();
    
private:


};