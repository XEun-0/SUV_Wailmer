#ifndef STATUSMSG_H
#define STATUSMSG_H

#include <string.h>  // C
#include <stdio.h>

#include "Common/commonTypes.h"

#define SET_STATUS_BYTE         1
#define SENSOR_BUFFER_SIZE      32
#define THRUSTER_BUFFER_SIZE    16
#define OUT_BUFFER_SIZE         SENSOR_BUFFER_SIZE + THRUSTER_BUFFER_SIZE

class StatusMsg {
public:
    // Constructor
    StatusMsg();
    
    /*********************************************************
     * 
     * Name:  GetValidationByte
     * Notes: n/a
     * 
     *********************************************************/
    uint8_t GetValidationByte();
    
    /*********************************************************
     * 
     * Name:  SetValidationByte
     * Notes: n/a
     * 
     *********************************************************/
    void SetValidationByte(uint8_t setByte);

    /*********************************************************
     * 
     * Name:  SetSensorOutInfo
     * Notes: n/a
     * 
     *********************************************************/
    void SetSensorOutInfo(SensorInfo sInfo);
    
    /*********************************************************
     * 
     * Name:  GetSensorOutInfo
     * Notes: n/a
     * 
     *********************************************************/
    uint32_t GetSensorOutInfo(uint8_t i);

    /*********************************************************
     * 
     * Name:  GetSensorOutInfo
     * Notes: n/a
     * 
     *********************************************************/
    void SetThrusterOutInfo(ThrusterInfo *tInfo);
    // uint32_t GetSensorOutInfo(uint8_t i);

    /*********************************************************
     * 
     * Name:  SetSensorValidation
     * Notes: n/a
     * 
     *********************************************************/
    void SetSensorValidation();
    
    /*********************************************************
     * 
     * Name:  SetThrusterValidation
     * Notes: n/a
     * 
     *********************************************************/
    void SetThrusterValidation();

    /*********************************************************
     * 
     * Name:  SetIsSensorPopulated
     * Notes: n/a
     * 
     *********************************************************/
    void SetIsSensorPopulated(bool val);
    
    /*********************************************************
     * 
     * Name:  SetIsThrusterPopulated
     * Notes: n/a
     * 
     *********************************************************/
    void SetIsThrusterPopulated(bool val);
    
    /*********************************************************
     * 
     * Name:  GetIsSensorPopulated
     * Notes: n/a
     * 
     *********************************************************/
    bool GetIsSensorPopulated();
    
    /*********************************************************
     * 
     * Name:  GetIsThrusterPopulated
     * Notes: n/a
     * 
     *********************************************************/
    bool GetIsThrusterPopulated();
    
    /*********************************************************
     * 
     * Name:  GetOutBufferPointer
     * Notes: n/a
     * 
     *********************************************************/
    uint8_t* GetOutBufferPointer();

private:
    
    uint8_t validationByte;
    uint8_t outBuffer[OUT_BUFFER_SIZE];

    bool isSensorPopulated = false;
    bool isThrusterPopulated = false;

};

#endif //STATUSMSG_H