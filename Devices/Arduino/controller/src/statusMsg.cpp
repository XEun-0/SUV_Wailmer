#include "statusMsg.h"
#include "Arduino.h"

#define SENSOR_INFO_INDEX            0
#define THRUSTER_INFO_INDEX         32

StatusMsg::StatusMsg() {
    validationByte = 1;
    memset(outBuffer, 0, OUT_BUFFER_SIZE);
}

/*********************************************************
 * 
 * Name:  SetSensorOutInfo
 * Notes: n/a
 * 
 *********************************************************/
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

/*********************************************************
 * 
 * Name:  SetThrusterOutInfo
 * Notes: n/a
 * 
 *********************************************************/
void StatusMsg::SetThrusterOutInfo(ThrusterInfo *tInfo) {
    memcpy((void *)&outBuffer[THRUSTER_INFO_INDEX],  tInfo,  sizeof(ThrusterInfo));

    #ifdef SERIAL_OUT
    uint8_t *c = (uint8_t *)tInfo;
    int s;
        Serial.println("inside setThrusterOutInfo: ");
        for(s = 0; s < sizeof(ThrusterInfo); s++) {
            Serial.print(*c++, HEX);
            Serial.print(" ");
        }

        Serial.print("\n");
    #endif
}

/*********************************************************
 * 
 * Name:  GetOutBufferPointer
 * Notes: n/a
 * 
 *********************************************************/
uint8_t* StatusMsg::GetOutBufferPointer() {
    return outBuffer;
}

/*********************************************************
 * 
 * Name:  GetSensorOutInfo
 * Notes: n/a
 * 
 *********************************************************/
uint32_t StatusMsg::GetSensorOutInfo(uint8_t i) {
    return outBuffer[i];
}

/*********************************************************
 * 
 * Name:  GetValidationByte
 * Notes: n/a
 * 
 *********************************************************/
uint8_t StatusMsg::GetValidationByte() {
    return outBuffer[32];
}

/*********************************************************
 * 
 * Name:  SetValidationByte
 * Notes: n/a
 * 
 *********************************************************/
void StatusMsg::SetValidationByte(uint8_t setByte) {
    validationByte = setByte;
}

/*********************************************************
 * 
 * Name:  SetIsSensorPopulated
 * Notes: n/a
 * 
 *********************************************************/
void StatusMsg::SetIsSensorPopulated(bool val) {
    isSensorPopulated = val;
}

/*********************************************************
 * 
 * Name:  SetIsThrusterPopulated
 * Notes: n/a
 * 
 *********************************************************/
void StatusMsg::SetIsThrusterPopulated(bool val) {
    isThrusterPopulated = val;
}

/*********************************************************
 * 
 * Name:  GetIsSensorPopulated

 * Notes: n/a
 * 
 *********************************************************/
bool StatusMsg::GetIsSensorPopulated() {
    return isSensorPopulated;
}

/*********************************************************
 * 
 * Name:  GetIsThrusterPopulated

 * Notes: n/a
 * 
 *********************************************************/
bool StatusMsg::GetIsThrusterPopulated() {
    return isThrusterPopulated;
}

