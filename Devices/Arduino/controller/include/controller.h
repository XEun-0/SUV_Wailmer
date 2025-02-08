#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#include <Arduino_FreeRTOS.h>
#include <Servo.h>
#include "sensorController.h"
#include "thrusterController.h"
#include "serialDataHandler.h"
#include "statusMsg.h"

class Controller {

public:
    /*********************************************************
     * 
     * Name:  getInstance
     * Notes: get singleton instance of the controller
     * 
     *********************************************************/
    static Controller *getInstance (void);

    /*********************************************************
     * 
     * Name:  mainLoop
     * Notes: main loop
     * 
     *********************************************************/
    void mainLoop(void);

private:
    
    static Controller *instance;

    // Constructor
    Controller(void);
    // Destructor
    ~Controller(void);

    void init(void);

};

// Not in class
void controllerTaskLauncher( void *pvParams );

#endif // CONTROLLER_H