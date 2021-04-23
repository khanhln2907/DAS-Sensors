/*
 Name:		TestGPSBlox7.ino
 Created:	4/23/2021 3:02:36 PM
 Author:	Khanh Le

* Brief // 
*/

#pragma once
#include "TaskSampleUARTBase.h"

class TaskSampleGPS : public TaskSampleUARTBase {
public:
	TaskSampleGPS();

protected:
	void run(void* data);

private:


};