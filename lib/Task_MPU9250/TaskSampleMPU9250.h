/*
 Created:	5/06/2021 23:10:36 PM
 Author:	Khanh Le

* Brief // 
*/

#pragma once
#include "TaskSampleMPUBase.h"
#include "SPI.h"
#include "MPU9250.h"

class TaskSampleMPU9250 : public TaskSampleMPUBase {
public:
    TaskSampleMPU9250();
    void begin(SPIClass* SPIx, uint8_t cs);



protected:

    virtual uint16_t getTaskRate();
    virtual void loop(void* data);
    
private:
    MPU9250 _imuHandle;

};