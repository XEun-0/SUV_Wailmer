#ifndef STATUSMSG_H
#define STATUSMSG_H

#include <string.h>  // C
#include <stdio.h>

#include "Common/commonTypes.h"

#define SET_STATUS_BYTE         1
#define SENSOR_BUFFER_SIZE      32
#define OUT_BUFFER_SIZE         SENSOR_BUFFER_SIZE + SET_STATUS_BYTE

class StatusMsg {
public:
    StatusMsg();
    uint8_t GetValidationByte();
    void SetValidationByte(uint8_t setByte);

    void SetSensorOutInfo(SensorInfo sInfo);
    uint32_t GetSensorOutInfo(uint8_t i);

    void SetThrusterOutInfo(ThrusterInfo tInfo);
    void SetSensorValidation();
    void SetThrusterValidation();
    
private:
    uint8_t validationByte;
    uint8_t outBuffer[OUT_BUFFER_SIZE];
};

#endif //STATUSMSG_H