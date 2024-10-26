#ifndef THRUSTERCONTROLLER_H
#define THRUSTERCONTROLLER_H

#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#include <Arduino_FreeRTOS.h>
#include <Servo.h>

// Motors
// Pins should 8-13
#define LEFT_THRUSTER_PIN         11
#define RIGHT_THRUSTER_PIN        10
#define FRONT_RIGHT_THRUSTER_PIN  5
#define FRONT_LEFT_THRUSTER_PIN   6
#define BACK_LEFT_THRUSTER_PIN    9
#define BACK_RIGHT_THRUSTER_PIN   7

#define MOTOR_STOP_COMMAND        1500

// Macros
#define THRUSTER_COMMAND_BUFFER_SIZE    32 //temp size may change
#define THRUSTER_READY_MS               7000
#define SERIAL_SEND_MS                  150
#define THRUSTER_TASK_MS                100

class ThrusterController {
public:
    ThrusterController();

    int Run();
    int InitializeThrusters();
    int GetTaskMS();

    // Thruster Controller State
    enum ThrusterControllerState {
        THRUSTER_INIT,
        THRUSTER_ARM,
        THRUSTER_TESTING,
        THRUSTER_GO,
        THRUSTER_NOGO
    };
    
    TaskHandle_t ThrusterCommandsTaskHandle;
private:
    Servo left_thruster;
    Servo right_thruster;
    Servo front_right_thruster;
    Servo front_left_thruster;
    Servo back_left_thruster;
    Servo back_right_thruster;

    ThrusterControllerState thrusterCommState;
    uint8_t thrusterCommandsBuffer[THRUSTER_COMMAND_BUFFER_SIZE];

    int InitializeControls();
    void SlowMotorSpin();
};

#endif // THRUSTERCONTROLLER_H
