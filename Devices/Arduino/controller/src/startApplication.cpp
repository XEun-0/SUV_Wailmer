#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#include <Arduino_FreeRTOS.h>
#include <Servo.h>

// Declare globals as extern in this file
// while declaring actually for other files
// that require gTTCInterface
#define DECLARE_GLOBALS
#include "Common/globals.h"     // gTTCInterface

#include "controller.h"         // controllerTaskLauncher

// SensorController    *sensors;
// ThrusterController  *thrusters;
// SerialDataHandler   *serialHandler;

// /*********************************************************
//  * 
//  * Name:  SerialMsgTask
//  * Notes: n/a
//  * 
//  *********************************************************/
// void SerialMsgTask(void *pvParams) {
//     while (1) {
//         TaskParams_t* params = (TaskParams_t*)pvParams;
//         serialHandler->Run(params);

//         vTaskDelay( serialHandler->GetTaskMS() / portTICK_PERIOD_MS);
//     }
// }

// /*********************************************************
//  * 
//  * Name:  ThrusterCommandsTask
//  * Notes: n/a
//  * 
//  *********************************************************/
// void ThrusterCommandsTask(void *pvParams) {
//     while (1) {
//         TaskParams_t* params = (TaskParams_t*)pvParams;
//         thrusters->Run(params);

//         vTaskDelay( thrusters->GetTaskMS() / portTICK_PERIOD_MS);
//     }
// }

// /*********************************************************
//  * 
//  * Name:  AggregateSensorsTask
//  * Notes: n/a
//  * 
//  *********************************************************/
// void AggregateSensorsTask(void *pvParams) {
//     while (1) {
//         TaskParams_t* params = (TaskParams_t*)pvParams;
//         //params->statusMsg->SetValidationByte(4);
//         sensors->Run(params);
//         // Check if sensors have run
//         vTaskDelay( sensors->GetTaskMS() / portTICK_PERIOD_MS);
//     }
// }

/*********************************************************
 * 
 * Name:  setup
 * Notes: Arduino way of starting application task
 * 
 *********************************************************/
void setup() {
    Serial.begin(SYS_BAUD_RATE);

    // sensors = new SensorController();
    // thrusters = new ThrusterController();
    // serialHandler = new SerialDataHandler();

    // make serialDataInterface class and thrusterInfo.h and sensorInfo.h
    // design changes include just removing too many tasks and 
    // incorporating them into 1 other tasking.


    // sensors->InitializeSensors();
    // thrusters->InitializeThrusters();
    // serialHandler->InitializeSerialDataHandler();

    // StatusMsg* statusMsg = new StatusMsg();
    
    // // Create TaskParams type with counting semaphore and statusMsg
    // TaskParams_t taskParameters = { 
    //                                 xSemaphoreCreateMutex(),    // xSerialMutex
    //                                 xEventGroupCreate(),        // xSerialEventGroup
    //                                 statusMsg
    //                               };
    
    // // Create Serial data handling task. 
    // xTaskCreate(
    //     SerialMsgTask,                              // Function to be called
    //     "SerialMsgTask",                            // Name of the task
    //     512,                                        // Stack size
    //     &taskParameters,                            // Parameters passed to task
    //     2,                                          // Task priority (higher number = higher priority)
    //     serialHandler->GetTaskHandle()              // Task handle for reference
    // );

    // // Create Sensor Aggregation Task
    // xTaskCreate(
    //     AggregateSensorsTask,                       // Function to be called
    //     "AggregateSensorsTask",                     // Name of the task
    //     512,                                        // Stack size
    //     &taskParameters,                            // Parameters passed to task
    //     3,                                          // Task priority (higher number = higher priority)
    //     &(sensors->AggregateSensorsTaskHandle)      // Task handle for reference
    // );

    // // Create Motor Commands Task
    // xTaskCreate(
    //     ThrusterCommandsTask,                       // Function to be called
    //     "MotorCommandsTask",                        // Name of the task
    //     512,                                        // Stack size
    //     &taskParameters,                            // Parameters passed to task
    //     1,                                          // Task priority (higher number = higher priority)
    //     thrusters->GetTaskHandle()                  // Task handle for reference
    // );

    // Create main controller task
    xTaskCreate(
        controllerTaskLauncher,             // Function to be called
        "controllerTaskLauncher",           // Name of the task
        512,                                // Stack size
        NULL,                               // Parameters passed to task
        1,                                  // Task priority (higher number = higher priority)
        NULL                                // Task handle for reference
    );

    // TTCInterface is global because main task
    // Controller needs to access it. 
    gTTCInterface.TTCTaskLauncher();

    // Start the scheduler
    vTaskStartScheduler();
}

/*********************************************************
 * 
 * Name:  loop
 * Notes: RTOS removes loop functionality
 * 
 *********************************************************/
void loop() {}