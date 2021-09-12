#include "BasicType.h"
#include "qpcpp.h"

using namespace QP;

class SensorBase : QP::QActive {
public:
    SensorBase();
    ~SensorBase();

    void startAO(uint8_t prio);

protected:
    virtual uint32_t getSampleRate() = 0;

    Q_STATE_DECL(initial);
    // Q_STATE_DECL(connected);
    // Q_STATE_DECL(disconnected);

private:
    virtual void sample() = 0;

};

