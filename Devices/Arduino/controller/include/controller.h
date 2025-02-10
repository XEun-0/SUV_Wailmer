#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "TTCInterface.h"
#include "Common/globals.h"
#include "Common/util.h"        // hexDump, etc

// weed out what's not needed
#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#include <Arduino_FreeRTOS.h>

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

    int l_ttcInterfaceInitialized;

    /*********************************************************
     * 
     * Name:  Controller
     * Notes: Constructor
     * 
     *********************************************************/
    Controller(void);

    /*********************************************************
     * 
     * Name:  ~Controller
     * Notes: Destructor
     * 
     *********************************************************/
    ~Controller(void);

    /*********************************************************
     * 
     * Name:  init
     * Notes: initialize certain things if needed
     * 
     *********************************************************/
    void init(void);

    /*********************************************************
     * 
     * Name:  getSendSOHSerial
     * Notes: initialize certain things if needed
     * 
     *********************************************************/
    void getSendSOHSerial(void);

    /*********************************************************
     * 
     * Name:  rxSerialCommands
     * Notes: initialize certain things if needed
     * 
     *********************************************************/
    void rxSerialCommands(void);

};

// Not in class

/*********************************************************
 * 
 * Name:  controllerTaskLauncher
 * Notes: Makes isntance of Controller if it doesn't exist
 *        and starts the setup then main loop.
 * 
 *********************************************************/
void controllerTaskLauncher( void *pvParams );

#endif // CONTROLLER_H