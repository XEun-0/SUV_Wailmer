#ifndef THRUSTERCONTROLLER_H
#define THRUSTERCONTROLLER_H

#include <Wire.h>
#include <Servo.h>
#include <Adafruit_BNO055.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "MS5837.h"
#include "statusMsg.h"
#include "Common/commonTypes.h"

// Motors
// Pins should 8-13
#define LEFT_THRUSTER_PIN           8         // 8
#define RIGHT_THRUSTER_PIN          13        // 13
#define FRONT_RIGHT_THRUSTER_PIN    12        // 12
#define FRONT_LEFT_THRUSTER_PIN     10        // 10
#define BACK_LEFT_THRUSTER_PIN      9         // 9
#define BACK_RIGHT_THRUSTER_PIN     11        // 11

#define MOTOR_STOP_COMMAND          1500
#define SLOW_SPEED                  1600

// Macros
#define THRUSTER_COMMAND_BUFFER_SIZE    32 //temp size may change
#define THRUSTER_INFO_SIZE              16
#define THRUSTER_READY_MS               7000
#define SERIAL_SEND_MS                  150
#define THRUSTER_TASK_MS                100

class ThrusterController {
public:
    ThrusterController();

    /*********************************************************
     * 
     * Name:  Run
     * Notes: n/a
     * 
     *********************************************************/
    int Run(TaskParams_t* params);
    
    /*********************************************************
     * 
     * Name:  InitializeThrusters
     * Notes: n/a
     * 
     *********************************************************/
    int InitializeThrusters();

    /*********************************************************
     * 
     * Name:  GetTaskMS
     * Notes: n/a
     * 
     *********************************************************/
    int GetTaskMS();

    /*********************************************************
     * 
     * Name:  GetTaskHandle
     * Notes: n/a
     * 
     *********************************************************/
    TaskHandle_t* GetTaskHandle();
    
private:
    Servo left_thruster;
    Servo right_thruster;
    Servo front_right_thruster;
    Servo front_left_thruster;
    Servo back_left_thruster;
    Servo back_right_thruster;

    ThrusterControllerState thrusterCommState;
    uint8_t thrusterCommandsBuffer[THRUSTER_COMMAND_BUFFER_SIZE];
    ThrusterInfo thruster_info;
    ThrusterSpeedsType thrusterSpeedsType;

    int InitializeControls();
    void SlowMotorSpin();
    void SetThrusterSpeed(ThrusterSpeedsType spds);
    void SetSingleThrusterSpeed(Servo *thruster, uint16_t speed);

    TaskHandle_t ThrusterCommandsTaskHandle;
};

#endif // THRUSTERCONTROLLER_H
