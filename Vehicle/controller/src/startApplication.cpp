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

/*********************************************************
 * 
 * Name:  setup
 * Notes: Arduino way of starting application task
 * 
 *********************************************************/
void setup() {
    Serial.begin(SYS_BAUD_RATE);
    pPrintf("Starting SUV_WAILMER Software..\n");

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