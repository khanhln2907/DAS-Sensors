/*
 * Motivated by Kolban: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/TaskBase.cpp
 * TaskBaseBase.cpp
 *
 * Modifier: Khanh Le      
 */

#include "TaskBase.h"
#include "sdkconfig.h"
#include "ArduinoLog.h"

static const char* LOG_TAG = "TaskBase";


/**
 * @brief Create an instance of the TaskBase class.
 *
 * @param [in] TaskBaseName The name of the TaskBase to create.
 * @param [in] stackSize The size of the stack.
 * @return N/A.
 */
TaskBase::TaskBase(std::string TaskBaseName, uint16_t stackSize, uint8_t priority) {
    _TaskBaseData = nullptr;
    _handle = nullptr;
    _coreId = tskNO_AFFINITY;
} // TaskBase

TaskBase::~TaskBase() {
} // ~TaskBase

/**
 * Static class member that actually runs the target TaskBase.
 *
 * The code here will run on the TaskBase thread.
 * @param [in] pTaskBaseInstance The TaskBase to run.
 */
void TaskBase::runTaskBase(void* pTaskBaseInstance) {
    TaskBase* pTaskBase = (TaskBase*)pTaskBaseInstance;
    ESP_LOGD(LOG_TAG, ">> runTaskBase: TaskBaseName=%s", pTaskBase->_TaskBaseName.c_str());
    pTaskBase->run(pTaskBase->_TaskBaseData);
    ESP_LOGD(LOG_TAG, "<< runTaskBase: TaskBaseName=%s", pTaskBase->_TaskBaseName.c_str());
    pTaskBase->stop();
} // runTaskBase

/**
 * @brief Start an instance of the TaskBase.
 *
 * @param [in] TaskBaseData Data to be passed into the TaskBase.
 * @return N/A.
 */
void TaskBase::start(std::string TaskBaseName, uint16_t stackSize, void* TaskBaseData, uint8_t priority, xTaskHandle taskHandle, BaseType_t coreId) {
    if (_handle != nullptr) {
        Log.error("TaskBase assert: Task handle was not defined \n");
    }
    _TaskBaseName = TaskBaseName;
    _stackSize = stackSize;
    _priority = priority;
    _coreId = coreId;
    _handle = taskHandle;

    _TaskBaseData = TaskBaseData;
    ::xTaskCreatePinnedToCore(&runTaskBase, _TaskBaseName.c_str(), _stackSize, this, _priority, &_handle, _coreId);
} // start

/**
 * @brief Suspend the TaskBase for the specified milliseconds.
 *
 * @param [in] ms The delay time in milliseconds.
 * @return N/A.
 */

/* static */ void TaskBase::taskSleepMllis(int ms) {
    ::vTaskDelay(ms / portTICK_PERIOD_MS);
} // delay


/**
 * @brief Stop the TaskBase.
 *
 * @return N/A.
 */
void TaskBase::stop() {
    if (_handle == nullptr) return;
    xTaskHandle temp = _handle;
    _handle = nullptr;
    ::vTaskDelete(temp);
} // stop
