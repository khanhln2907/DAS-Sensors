/*
 Name:		TestGPSBlox7.ino
 Created:	4/23/2021 3:02:36 PM
 Author:	Khanh Le

* Brief // 
*/

#pragma once
#include "SensorBase.h"
#include "I_UART_Base.h"
#include "BasicType.h"

#include <string>

namespace GPS_t {
    enum class Status_t : char {
        DataValid = 'A',
        DataInvalid = 'V',
    };

    enum class Quality_t : char {
        No_Fix = '0',
        Autonomous_GNSS_Fix = '1',
        Differential_GNSS_Fix = '2',
        Estimated_Dead_Reckoning_Fix = '6'
    };

    enum class NavMode_t : char {
        No_Fix = '1',
        Fix_2D = '2',
        Fix_3D = '3',
    };

    enum class PosMode_t : char {
        No_Fix = 'N',
        Estimated_Dead_Reckoning_Fix = 'E',
        Autonomous_GNSS_Fix = 'A',
        Differential_GNSS_Fix = 'D'
    };

    enum NMEAINFO : char {
        Status_DataValid = 'A',
        Status_DataInvalid = 'V',
        Quality_No_Fix = '0',
        Quality_Autonomous_GNSS_Fix = '1',
        Quality_Differential_GNSS_Fix = '2',
        Quality_Estimated_Dead_Reckoning_Fix = '6',
        NavMode_No_Fix = '1',
        NavMode_Fix_2D = '2',
        NavMode_Fix_3D = '3',
        PosMode_No_Fix = 'N',
        PosMode_Estimated_Dead_Reckoning_Fix = 'E',
        PosMode_Autonomous_GNSS_Fix = 'A',
        PosMode_Differential_GNSS_Fix = 'D'
    };

    struct RMCMessag_t {
        UTCTime utcTimeInfo;
        Date dateInfo;
        Status_t RMCStatus;
        Position_WGS84 posData;
        char NSIndicator;
        char EWIndicator;
        PosMode_t modeIndicator;
    };
}

class TaskSampleGPS : public SensorBase, public I_UART_Base {
public:
    TaskSampleGPS() = delete;
	TaskSampleGPS(uint16_t packetSize);

protected:
    virtual void parseNMEA_GPS_Message(std::string msg);
    
    uint8_t getNHeaderBytes();
    virtual const uint8_t* const getHeaderBytes(uint8_t* nHeader);
    virtual bool parse();

    virtual uint32_t getSampleRate();
    
    virtual void sample();
private:
    const uint8_t _HB[1] = { 0x24 };

    Sample<Position_WGS84> _wgs84Sample;
    UTCTime _timeUTC;
};