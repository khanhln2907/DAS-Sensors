#pragma once
#include "TaskSampleMPU9250.h"
#include "ArduinoLog.h"

TaskSampleMPU9250::TaskSampleMPU9250(){
    ;
};

uint16_t TaskSampleMPU9250::getTaskRate(){
    return 200;
};

void TaskSampleMPU9250::loop(void* data){
    // Hello do sth here
    //Log.notice("Hi\n");
    _imuHandle.sample();

    static uint32_t lastPrintTime = millis();
    if (millis() - lastPrintTime > 100) {
        _imuHandle.printData();
        lastPrintTime = millis();
    }
};

void TaskSampleMPU9250::begin(SPIClass* SPIx, uint8_t cs){
    _imuHandle.setup(SPIx, cs);
    MPU_CONFIG_STATUS status = _imuHandle.begin();

    Log.notice("Init: %d \n", status);

    
}