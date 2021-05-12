#pragma once
/*
 * Motivated by Kolban: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/TaskBase.cpp
 * TaskBaseBase.h
 *
 * Modifier: Khanh Le
 */


#ifndef COMPONENTS_CPP_UTILS_TaskBase_H_
#define COMPONENTS_CPP_UTILS_TaskBase_H_
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string>
 /**
  * @brief Encapsulate a runnable TaskBase.
  *
  * This class is designed to be subclassed with the method:
  *
  * @code{.cpp}
  * void run(void *data) { ... }
  * @endcode
  *
  * For example:
  *
  * @code{.cpp}
  * class CurlTestTaskBase : public TaskBase {
  *    void run(void *data) {
  *       // Do something
  *    }
  * };
  * @endcode
  *
  * implemented.
  */
class TaskBase {
public:
	TaskBase(std::string TaskBaseName = "TaskBase", uint16_t stackSize = 10000, uint8_t priority = 5);
	virtual ~TaskBase();

	// Call these function to start up the task
	void start(std::string TaskBaseName, uint16_t stackSize, void* TaskBaseData, uint8_t priority, xTaskHandle taskHandle, BaseType_t coreId);

	void stop();
	/**
	 * @brief Body of the TaskBase to execute.
	 *
	 * This function must be implemented in the subclass that represents the actual TaskBase to run.
	 * When a TaskBase is started by calling start(), this is the code that is executed in the
	 * newly created TaskBase.
	 *
	 * @param [in] data The data passed in to the newly started TaskBase.
	 */
	static void taskSleepMllis(int ms);

protected:
	virtual uint16_t getTaskRate() = 0;
	virtual void loop(void* data) = 0; // Make run pure virtual

private:
	xTaskHandle _handle;
	void* _TaskBaseData;
	static void runTaskBase(void* data);
	std::string _TaskBaseName;
	uint16_t    _stackSize;
	uint8_t     _priority;
	BaseType_t  _coreId;

	void taskSleepInterval();

};

#endif /* COMPONENTS_CPP_UTILS_TaskBase_H_ */