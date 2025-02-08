#ifndef TTCINTERFACE_H
#define TTCINTERFACE_H

#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#include <Arduino_FreeRTOS.h>
#include <Servo.h>
#include "sensorController.h"
#include "thrusterController.h"
#include "serialDataHandler.h"
#include "statusMsg.h"

// Telemetry and Thruster Control Inteface
class TTCInterface {

public:

    TTCInterface(void);
    ~TTCInterface(void);

    void mainLoop(void);
    void TTCTaskLauncher(void);

private:

    void init(void);
    
    void processInfo();
    static void processInfoStatic(void *pvParams);
};

#endif // TTCINTERFACE_H