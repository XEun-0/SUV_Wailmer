#ifndef TTCINTERFACE_H
#define TTCINTERFACE_H

#include <Wire.h>
#include "MS5837.h"                     // MS5837
#include <Adafruit_BNO055.h>            // Adafruit_BNO055
#include <Arduino_FreeRTOS.h>           // RTOS Functions
#include <Servo.h>                      // Servo

#include "Common/taskGlobals.h"         // TTC_TASK_DELAY
#include "Common/appStatusCodeTypes.h"  // SensorInitStatusCodeType
#include "Interface/ttcSohResp.h"       // Structs and Enums

// Telemetry and Thruster Control Interface
class TTCInterface {

public:

    TTCInterface(void);
    ~TTCInterface(void);

    /*********************************************************
     * 
     * Name:  doThrusterCommands
     * Notes: apply thruster commands to thrusters
     * 
     *********************************************************/
    void doThrusterCommands(ThrusterCommandsType cmds);

    /*********************************************************
     * 
     * Name:  gatherSensorSOH
     * Notes: n/a
     * 
     *********************************************************/
    void gatherSensorSOH(TTCSohRespType *pInfo);

    /*********************************************************
     * 
     * Name:  gatherThrusterSOH
     * Notes: n/a
     * 
     *********************************************************/
    void gatherThrusterSOH(TTCSohRespType *pInfo);

    /*********************************************************
     * 
     * Name:  isInitialized
     * Notes: n/a
     * 
     *********************************************************/
    bool isInitialized(void);
    
    /*********************************************************
     * 
     * Name:  mainLoop
     * Notes: n/a
     * 
     *********************************************************/
    void mainLoop(void);

    /*********************************************************
     * 
     * Name:  TTCTaskLauncher
     * Notes: n/a
     * 
     *********************************************************/
    void TTCTaskLauncher(void);

private:
    const uint8_t bnoCalibrationData[22] = {
        0x02, 0x00, 0xCE, 0xFF, 0xE4, 0xFF, 0x65, 0x01, 
        0xF3, 0x01, 0xEA, 0xFE, 0xFF, 0xFF, 0xFE, 0xFF, 
        0x00, 0x00, 0xE8, 0x03, 0x21, 0x03
    };

    SemaphoreHandle_t   sohMutex;
    SemaphoreHandle_t   thruserBinarySemaphore;

    Adafruit_BNO055     bno;
    MS5837              baro;

    bool ttc_initialized;
    

    SensorInitStatusCodeType baroInitStatus;
    SensorInitStatusCodeType imuInitStatus;

    // State of health id
    uint8_t     soh_id;           // 1

    // Thruster control
    ThrusterState   currThrusterState;

    // Servo       left_thruster;
    // Servo       right_thruster;
    // Servo       front_right_thruster;
    // Servo       front_left_thruster;
    // Servo       back_left_thruster;
    // Servo       back_right_thruster;

    // Left side thrusters relative to camera
    Servo rear_left_thruster;
    Servo front_left_thruster;
    Servo left_thruster;
    int16_t     local_rear_left_thruster_speed;
    int16_t     local_front_left_thruster_speed;
    int16_t     local_left_thruster_speed;

    // Right side thrusters relative to camera
    Servo rear_right_thruster;
    Servo front_right_thruster;
    Servo right_thruster;
    int16_t     local_rear_right_thruster_speed;
    int16_t     local_front_right_thruster_speed;
    int16_t     local_right_thruster_speed;

    // int16_t     local_left_thruster_speed;
    // int16_t     local_right_thruster_speed;
    // int16_t     local_front_right_thruster_speed;
    // int16_t     local_front_left_thruster_speed;
    // int16_t     local_back_left_thruster_speed;
    // int16_t     local_back_right_thruster_speed;

    // Sensor information
    uint8_t   imuTemp;          // 1
    
    float     baroPressure;     // 4
    float     baroTemp;         // 4
    float     baroDepth;        // 4
    float     baroAltitude;     // 4

    float     imuOrientX;       // 4
    float     imuOrientY;       // 4
    float     imuOrientZ;       // 4

    // Functions

    /*********************************************************
     * 
     * Name:  init
     * Notes: initialize parameters
     * 
     *********************************************************/
    void init(void);
    
    /*********************************************************
     * 
     * Name:  initializeBarometer
     * Notes: n/a
     * 
     *********************************************************/
    SensorInitStatusCodeType initializeBarometer();
    
    void thrusterTest(Servo* t, int tpin);

    /*********************************************************
     * 
     * Name:  initializeIMU
     * Notes: n/a
     * 
     *********************************************************/
    SensorInitStatusCodeType initializeIMU();

    /*********************************************************
     * 
     * Name:  initializeThrusters
     * Notes: n/a
     * 
     *********************************************************/
    void initializeThrusters(void);

    /*********************************************************
     * 
     * Name:  printSensorInitStatusCodeType
     * Notes: n/a
     * 
     *********************************************************/
    void printSensorInitStatusCodeType(SensorInitStatusCodeType in);

    /*********************************************************
     * 
     * Name:  processInfo
     * Notes: process information, main loop
     * 
     *********************************************************/
    void processInfo();

    /*********************************************************
     * 
     * Name:  processInfoStatic
     * Notes: create static instance if it doesn't exist
     * 
     *********************************************************/
    static void processInfoStatic(void *pvParams);

    /*********************************************************
     * 
     * Name:  processThrusterState
     * Notes: n/a
     * 
     *********************************************************/
    void processThrusterState(void);

    /*********************************************************
     * 
     * Name:  setThrusterSpeed
     * Notes: n/a
     * 
     *********************************************************/
    void setThrusterSpeed(int16_t spds);

    /*********************************************************
     * 
     * Name:  setThrusterSpeed
     * Notes: n/a
     * 
     *********************************************************/
    void setThrusterSpeed(ThrusterSpeedsType spds);

    /*********************************************************
     * 
     * Name:  setLocalThrusterSpeed
     * Notes: n/a
     * 
     *********************************************************/
    void setLocalThrusterSpeed(int16_t spds);

    /*********************************************************
     * 
     * Name:  setLocalThrusterSpeed
     * Notes: n/a
     * 
     *********************************************************/
    void setLocalThrusterSpeed(ThrusterSpeedsType spds);
    
};

#endif // TTCINTERFACE_H