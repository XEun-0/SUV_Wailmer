#include "statusMsg.h"


StatusMsg::StatusMsg() {
    validationByte = 1;
    memset(outBuffer, 0, OUT_BUFFER_SIZE);
}

void StatusMsg::SetSensorOutInfo(SensorInfo sInfo) {
    // bytes 0-27 = 28 bytes
    memcpy(&outBuffer[0],  &sInfo.baroPressure,  sizeof(sInfo.baroPressure));
    memcpy(&outBuffer[4],  &sInfo.baroTemp,      sizeof(sInfo.baroTemp));
    memcpy(&outBuffer[8],  &sInfo.baroDepth,     sizeof(sInfo.baroDepth));
    memcpy(&outBuffer[12], &sInfo.baroAltitude,  sizeof(sInfo.baroAltitude));

    memcpy(&outBuffer[16], &sInfo.imuOrientX,    sizeof(sInfo.imuOrientX));
    memcpy(&outBuffer[20], &sInfo.imuOrientY,    sizeof(sInfo.imuOrientY));
    memcpy(&outBuffer[24], &sInfo.imuOrientZ,    sizeof(sInfo.imuOrientZ));

    // bytes 28-29 = 1 byte
    memcpy(&outBuffer[28], &sInfo.imuTemp, sizeof(sInfo.imuTemp));
}

uint32_t StatusMsg::GetSensorOutInfo(uint8_t i) {
    return outBuffer[i];
}

uint8_t StatusMsg::GetValidationByte() {
    return validationByte;
}

void StatusMsg::SetValidationByte(uint8_t setByte) {
    validationByte = setByte;
}

