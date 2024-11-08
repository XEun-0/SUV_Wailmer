#ifndef SERIALDATAHANDLER_H
#define SERIALDATAHANDLER_H

#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#include <Arduino_FreeRTOS.h>
#include <Servo.h>

#define SERIAL_TASK_MS 100;

class SerialDataHandler {
public:
    SerialDataHandler();

    int Run();
    int InitializeSerialDataHandler();
    int GetTaskMS();

    // // Thruster Controller State
    // enum SerialDataHandlerState {
    //     THRUSTER_INIT,
    //     THRUSTER_ARM,
    //     THRUSTER_TESTING,
    //     THRUSTER_GO,
    //     THRUSTER_NOGO
    // };
    
    TaskHandle_t SerialDataHandlerTaskHandle;
private:

    // SerialDataHandlerState thrusterCommState;
    // uint8_t thrusterCommandsBuffer[THRUSTER_COMMAND_BUFFER_SIZE];

    // int InitializeControls();
    // void SlowMotorSpin();
};

#endif // SERIALDATAHANDLER_H
