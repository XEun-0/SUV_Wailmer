#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#include <Arduino_FreeRTOS.h>
#include <Servo.h>
#include "SensorController.h"
#include "ThrusterController.h"
#include "SerialDataHandler.h"

static SemaphoreHandle_t msgSemaphore;

SensorController    *sensors;
ThrusterController  *thrusters;
SerialDataHandler   *serialHandler;

void SerialMsgTask(void *pvParams) {
    while (1) {
        
        serialHandler->Run();

        vTaskDelay( thrusters->GetTaskMS() / portTICK_PERIOD_MS);
    }
}

void ThrusterCommandsTask(void *pvParams) {
    while (1) {
        
        thrusters->Run();

        vTaskDelay( thrusters->GetTaskMS() / portTICK_PERIOD_MS);
    }
}

void AggregateSensorsTask(void *pvParams) {
    while (1) {
        SemaphoreHandle_t msgSemaphore = (SemaphoreHandle_t)pvParams;
        sensors->Run(msgSemaphore);

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

    // mutex semaphore starts as given
    // Feature	                xSemaphoreCreateMutex	        xSemaphoreCreateBinary
    // Purpose	                Mutual exclusion	            Event signaling
    // Priority Inheritance	    Yes	                            No
    // Initial State	        Given	                        Taken
    // Best Use Case	        Protecting shared resources	    Task signaling or event handling
    msgSemaphore = xSemaphoreCreateMutex();

    // Create Serial data handling task. 
    xTaskCreate(
        SerialMsgTask,                              // Function to be called
        "SerialMsgTask",                            // Name of the task
        512,                                        // Stack size
        (void *)msgSemaphore,                       // Parameters passed to task
        2,                                          // Task priority (higher number = higher priority)
        &(serialHandler->SerialDataHandlerTaskHandle) // Task handle for reference
    );

    // Create Sensor Aggregation Task
    xTaskCreate(
        AggregateSensorsTask,                       // Function to be called
        "AggregateSensorsTask",                     // Name of the task
        512,                                        // Stack size
        (void *)msgSemaphore,                       // Parameters passed to task
        3,                                          // Task priority (higher number = higher priority)
        &(sensors->AggregateSensorsTaskHandle)      // Task handle for reference
    );

    // Create Motor Commands Task
    xTaskCreate(
        ThrusterCommandsTask,                       // Function to be called
        "MotorCommandsTask",                        // Name of the task
        512,                                        // Stack size
        (void *)msgSemaphore,                       // Parameters passed to task
        1,                                          // Task priority
        &(thrusters->ThrusterCommandsTaskHandle)    // Task handle for reference
    );
    
    // Start the scheduler
    vTaskStartScheduler();
}

// RTOS removes loop functionality
void loop() {}
