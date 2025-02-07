#include "serialDataHandler.h"

/*********************************************************
 * 
 * Name:  SerialDataHandler Constructor
 * Notes: See serialDataHandler.h
 * 
 *********************************************************/
SerialDataHandler::SerialDataHandler() {}

/*********************************************************
 * 
 * Name:  InitializeSerialDataHandler
 * Notes: See serialDataHandler.h
 * 
 *********************************************************/
int SerialDataHandler::InitializeSerialDataHandler() {
    // SUV-20 Serial msg aggregation tasking story
}

/*********************************************************
 * 
 * Name:  Run
 * Notes: See serialDataHandler.h
 * 
 *********************************************************/
int SerialDataHandler::Run(TaskParams_t* params) { 

    if (params->xSerialEventGroup != NULL) {
        
        if ((xEventGroupGetBits(params->xSerialEventGroup) & (SENSOR_TASK_DONE | THRUSTER_TASK_DONE)) == (SENSOR_TASK_DONE | THRUSTER_TASK_DONE)) {
           
            #ifdef SERIAL_OUT
            Serial.println("[Serial Data Handler] has both semaphores.");
            #endif

            // Some task for 1 second
            vTaskDelay(50);
            #ifdef SERIAL_OUT
            Serial.println("[Serial Data Handler] inside run");
            Serial.print("sizeof(ThrusterInfo): ");
            Serial.println(sizeof(ThrusterInfo));
            Serial.print("sizeof(ThrusterSpeedsType): ");
            Serial.println(sizeof(ThrusterSpeedsType));
            Serial.print("sizeof(SensorInfo): ");
            Serial.println(sizeof(SensorInfo));
            Serial.println("[Serial Data Handler] params->statusMsg :");
            // for (int i = 0; i < OUT_BUFFER_SIZE; i++) {
            //     Serial.print(params->statusMsg->GetSensorOutInfo(i), HEX); // Print as hexadecimal
            //     Serial.print(",");
            // }
            hexDump(params->statusMsg->GetOutBufferPointer(), OUT_BUFFER_SIZE);
            //Serial.println(params->statusMsg->GetSensorOutInfo(32), HEX);
            Serial.println("\n\n");
            #endif
            
            xEventGroupClearBits(params->xSerialEventGroup, SENSOR_TASK_DONE | THRUSTER_TASK_DONE);
            
        } else {
            #ifdef SERIAL_OUT
            Serial.println("[Serial Data Handler] could not acquire semaphore.");
            #endif
        }

    } else {
        #ifdef SERIAL_OUT
        Serial.println("[Serial Data Handler] Semaphore does not exist.");
        #else
        // Serial.write(ERROR_CODE_NO_SEMAPHORE);
        #endif
    }

}

/*********************************************************
 * 
 * Name:  GetTaskMS
 * Notes: See serialDataHandler.h
 * 
 *********************************************************/
int SerialDataHandler::GetTaskMS() {
    return SERIAL_TASK_MS;
}

/*********************************************************
 * 
 * Name:  GetTaskHandle
 * Notes: See serialDataHandler.h
 * 
 *********************************************************/
TaskHandle_t* SerialDataHandler::GetTaskHandle() {
    return &SerialDataHandlerTaskHandle;
}

/*********************************************************
 * 
 * Name:  hexDump
 * Notes: See serialDataHandler.h
 * 
 *********************************************************/
void SerialDataHandler::hexDump(uint8_t *data, int length) {
    uint8_t *c = data;
    int s;

    for(s = 0; s < length; s++) {
        Serial.print(*c++, HEX);
        Serial.print(" ");
    }

    Serial.print("\n");
}