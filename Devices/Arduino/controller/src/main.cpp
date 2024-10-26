#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#include <Arduino_FreeRTOS.h>
#include <Servo.h>
#include "SensorController.h"
#include "ThrusterController.h"

// Enums
enum ProgressState {
  INITIALIZED_SENSORS,
  GO,
  RUNNING,
  NOGO
};
ProgressState progressState;

SensorController *sensors;
ThrusterController *thrusters;

void ThrusterCommandsTask(void *pvParams) {
    while (1) {
        thrusters->Run();

        vTaskDelay( thrusters->GetTaskMS() / portTICK_PERIOD_MS);
    }
}

void AggregateSensorsTask(void *pvParams) {
    while (1) {
        
        sensors->Run();

        vTaskDelay( sensors->GetTaskMS() / portTICK_PERIOD_MS);
    }
}

// Setup, initialize
void setup() {
    Serial.begin(115200);
    //int controlsStatus =  InitializeControls();
        
    // byte readyMessage[] = {0xAA};
    // byte confirmationMessage[] = {0xBB};

    // Initialize state variables
    // thrusterCommState =   THRUSTER_INIT;
    // progressState =       INITIALIZED_SENSORS;

    sensors = new SensorController();
    thrusters = new ThrusterController();

    sensors->InitializeSensors();
    thrusters->InitializeThrusters();

    // Create Sensor Aggregation Task
    xTaskCreate(
        AggregateSensorsTask,    // Function to be called
        "AggregateSensorsTask", // Name of the task
        512,      // Stack size
        NULL,     // Parameters passed to task
        1,        // Task priority (higher number = higher priority)
        &(sensors->AggregateSensorsTaskHandle) // Task handle for reference
    );

    // Create Motor Commands Task
    xTaskCreate(
        ThrusterCommandsTask,    // Function to be called
        "MotorCommandsTask", // Name of the task
        256,      // Stack size
        NULL,     // Parameters passed to task
        2,        // Task priority
        &(thrusters->ThrusterCommandsTaskHandle) // Task handle for reference
    );
    
    // Start the scheduler
    vTaskStartScheduler();
}

// RTOS removes loop functionality
void loop() {}
