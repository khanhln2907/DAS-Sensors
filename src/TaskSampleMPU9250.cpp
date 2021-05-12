#pragma once
#include "TaskSampleMPU9250.h"
#include "ArduinoLog.h"

TaskSampleMPU9250::TaskSampleMPU9250(){
    ;
};

uint16_t TaskSampleMPU9250::getTaskRate(){
    return 1000;
};

void TaskSampleMPU9250::loop(void* data){
    // Hello do sth here

};

void TaskSampleMPU9250::begin(SPIClass* SPIx, uint8_t cs){
    _imuHandle.setup(SPIx, cs);
    MPU_CONFIG_STATUS status = _imuHandle.begin();
    _imuHandle.sample();

    Log.notice("Init: %d \n", status);

    
}