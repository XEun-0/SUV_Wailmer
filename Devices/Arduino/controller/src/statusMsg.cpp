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

    // Validation bit in the 33th byte
    // 0000 0000, first bit is Sensor, second bit is Thruster
    // 1100 0000
    uint8_t vByte = outBuffer[32];
    memcpy(&outBuffer[32], &(vByte |= 0b10000000), sizeof(vByte));
}

// Check validation bit
// outBuffer[32] & (1 << 8);

uint32_t StatusMsg::GetSensorOutInfo(uint8_t i) {
    return outBuffer[i];
}

uint8_t StatusMsg::GetValidationByte() {
    return outBuffer[32];
}

void StatusMsg::SetValidationByte(uint8_t setByte) {
    validationByte = setByte;
}

void StatusMsg::SetIsSensorPopulated(bool val) {
    isSensorPopulated = val;
}

void StatusMsg::SetIsThrusterPopulated(bool val) {
    isThrusterPopulated = val;
}

bool StatusMsg::GetIsSensorPopulated() {
    return isSensorPopulated;
}

bool StatusMsg::GetIsThrusterPopulated() {
    return isThrusterPopulated;
}

