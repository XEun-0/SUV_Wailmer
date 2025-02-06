#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#include <Arduino_FreeRTOS.h>
#include <Servo.h>
#include "sensorController.h"
#include "thrusterController.h"
#include "serialDataHandler.h"
#include "statusMsg.h"

SensorController    *sensors;
ThrusterController  *thrusters;
SerialDataHandler   *serialHandler;

void SerialMsgTask(void *pvParams) {
    while (1) {
        TaskParams_t* params = (TaskParams_t*)pvParams;
        serialHandler->Run(params);

        vTaskDelay( serialHandler->GetTaskMS() / portTICK_PERIOD_MS);
    }
}

void ThrusterCommandsTask(void *pvParams) {
    while (1) {
        TaskParams_t* params = (TaskParams_t*)pvParams;
        thrusters->Run(params);

        vTaskDelay( thrusters->GetTaskMS() / portTICK_PERIOD_MS);
    }
}

void AggregateSensorsTask(void *pvParams) {
    while (1) {
        TaskParams_t* params = (TaskParams_t*)pvParams;
        //params->statusMsg->SetValidationByte(4);
        sensors->Run(params);
        // Check if sensors have run
        vTaskDelay( sensors->GetTaskMS() / portTICK_PERIOD_MS);
    }
}

// Setup, initialize
void setup() {
    Serial.begin(115200);

    sensors = new SensorController();
    thrusters = new ThrusterController();
    serialHandler = new SerialDataHandler();

    sensors->InitializeSensors();
    thrusters->InitializeThrusters();
    serialHandler->InitializeSerialDataHandler();

    StatusMsg* statusMsg = new StatusMsg();
    
    // Create TaskParams type with counting semaphore and statusMsg
    TaskParams_t taskParameters = { 
                                    xSemaphoreCreateMutex(),    // xSerialMutex
                                    xEventGroupCreate(),        // xSerialEventGroup
                                    statusMsg
                                  };
    
    // Create Serial data handling task. 
    xTaskCreate(
        SerialMsgTask,                              // Function to be called
        "SerialMsgTask",                            // Name of the task
        512,                                        // Stack size
        &taskParameters,                            // Parameters passed to task
        2,                                          // Task priority (higher number = higher priority)
        serialHandler->GetTaskHandle() // Task handle for reference
    );

    // Create Sensor Aggregation Task
    xTaskCreate(
        AggregateSensorsTask,                       // Function to be called
        "AggregateSensorsTask",                     // Name of the task
        512,                                        // Stack size
        &taskParameters,                            // Parameters passed to task
        3,                                          // Task priority (higher number = higher priority)
        &(sensors->AggregateSensorsTaskHandle)      // Task handle for reference
    );

    // Create Motor Commands Task
    xTaskCreate(
        ThrusterCommandsTask,                       // Function to be called
        "MotorCommandsTask",                        // Name of the task
        512,                                        // Stack size
        &taskParameters,                            // Parameters passed to task
        1,                                          // Task priority (higher number = higher priority)
        &(thrusters->ThrusterCommandsTaskHandle)    // Task handle for reference
    );
    
    // Start the scheduler
    vTaskStartScheduler();
}

// RTOS removes loop functionality
void loop() {}
