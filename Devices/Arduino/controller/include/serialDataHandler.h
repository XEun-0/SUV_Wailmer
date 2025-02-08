#ifndef SERIALDATAHANDLER_H
#define SERIALDATAHANDLER_H

#include "statusMsg.h"
#include "Arduino.h"
#include "Common/commonTypes.h"

#define SERIAL_TASK_MS  100

class SerialDataHandler {
public:
    SerialDataHandler();

    /*********************************************************
     * 
     * Name:  Run
     * Notes: n/a
     * 
     *********************************************************/
    int Run(TaskParams_t* params);

    /*********************************************************
     * 
     * Name:  InitializeSerialDataHandler
     * Notes: n/a
     * 
     *********************************************************/
    int InitializeSerialDataHandler();

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
    TaskHandle_t SerialDataHandlerTaskHandle;
    void hexDump(uint8_t *data, int length);
    // SerialDataHandlerState thrusterCommState;
    // uint8_t thrusterCommandsBuffer[THRUSTER_COMMAND_BUFFER_SIZE];

    // int InitializeControls();
    // void SlowMotorSpin();
};

#endif // SERIALDATAHANDLER_H
