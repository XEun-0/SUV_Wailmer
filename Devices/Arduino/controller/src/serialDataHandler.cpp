#include "serialDataHandler.h"

SerialDataHandler::SerialDataHandler() {}

// SUV-20 Serial msg aggregation tasking story
int SerialDataHandler::InitializeSerialDataHandler() {

}

int SerialDataHandler::Run(TaskParams_t* params) { 
    
    if (params->snsrSemaphore != NULL && params->thrsSemaphore != NULL) {
        if (xSemaphoreTake(params->snsrSemaphore, pdMS_TO_TICKS(100)) == pdTRUE
            && xSemaphoreTake(params->thrsSemaphore, pdMS_TO_TICKS(100)) == pdTRUE) {
            Serial.println("[Serial Data Handler] has both semaphores.");
            // Some task for 1 second
            vTaskDelay(50);

            xSemaphoreGive(params->snsrSemaphore); 
            xSemaphoreGive(params->thrsSemaphore); 
        } else {
            Serial.println("[Serial Data Handler] could not acquire semaphore.");
        }
    } else {
        Serial.println("[Serial Data Handler] Semaphore does not exist.");
    }
}

int SerialDataHandler::GetTaskMS() {
    return SERIAL_TASK_MS;
}