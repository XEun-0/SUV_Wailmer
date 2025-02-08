#include "controller.h"

// CONFIGURATION: https://docs.platformio.org/page/boards/atmelavr/megaatmega2560.html
// PLATFORM: Atmel AVR (5.1.0) > Arduino Mega or Mega 2560 ATmega2560 (Mega 2560)
// HARDWARE: ATMEGA2560 16MHz, 8KB RAM, 248KB Flash

//====== Initialize Variables ============================

Controller *Controller::instance = nullptr;

//========================================================

/*********************************************************
 * 
 * Name:  Controller
 * Notes: See controller.h
 * 
 *********************************************************/
Controller::Controller(void) {

}

/*********************************************************
 * 
 * Name:  ~Controller
 * Notes: See controller.h
 * 
 *********************************************************/
Controller::~Controller(void) {

}

/*********************************************************
 * 
 * Name:  getInstance
 * Notes: See controller.h
 * 
 *********************************************************/
Controller *Controller::getInstance (void) {
    if (Controller::instance == nullptr) {
        Controller::instance = new Controller();
        //Controller::instance->init(); // may not be needed
    }

    return Controller::instance;
}

/*********************************************************
 * 
 * Name:  init
 * Notes: See controller.h
 * 
 *********************************************************/
void Controller::init(void) {
    // Initialization params
}

/*********************************************************
 * 
 * Name:  mainLoop
 * Notes: See controller.h
 * 
 *********************************************************/
void Controller::mainLoop(void) {

    // Do whatever

    while(1) {

    }

    // Send error code serial msg if needed.
    vTaskDelete( NULL );
}

// Not in class
/*********************************************************
 * 
 * Name:  controllerTaskLauncher
 * Notes: See controller.h
 * 
 *********************************************************/
void controllerTaskLauncher( void *pvParams ) {
    Controller *pController = Controller::getInstance();
    pController->mainLoop();
}
