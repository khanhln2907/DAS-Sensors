#include "SensorBase.h"
#include "ArduinoLog.h"

SensorBase::SensorBase() : QActive(initial){
;
};


SensorBase::~SensorBase(){
;
};

Q_STATE_DEF(SensorBase, initial){
    Log.notice("Hello");
    return Q_RET_HANDLED;
}

void SensorBase::startAO(uint8_t prio){
    static QEvt const * l_missileQueueSto[2];    
    this->start(prio, l_missileQueueSto, Q_DIM(l_missileQueueSto), 
                nullptr, 0);
}
