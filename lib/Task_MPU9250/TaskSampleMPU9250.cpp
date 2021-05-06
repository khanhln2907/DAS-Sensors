#pragma once
#include "TaskSampleMPU9250.h"
#include "ArduinoLog.h"

TaskSampleMPU9250::TaskSampleMPU9250(){

};

uint16_t TaskSampleMPU9250::getSampleRate(){
    return 1000;
};

void TaskSampleMPU9250::loop(){
    // Hello do sth here
};