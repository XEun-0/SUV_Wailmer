#include "TTCInterface.h"

TTCInterface::TTCInterface(void) {

}

TTCInterface::~TTCInterface(void) {

}

/*********************************************************
 * 
 * Name:  init
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::init(void) {
    // Initialization params
}

void TTCInterface::TTCTaskLauncher() {
    xTaskCreate(
        processInfoStatic,                          // Function to be called
        "processInfoStatic",                        // Name of the task
        512,                                        // Stack size
        this,                                       // Parameters passed to task
        2,                                          // Task priority (higher number = higher priority)
        NULL                                        // Task handle for reference
    );
}

void TTCInterface::processInfo() {

    while(true) {
        Serial.println("this is inside TTCInterface");
    }

}

void TTCInterface::processInfoStatic(void *pvParams) {
    TTCInterface *ttcInterface = static_cast<TTCInterface *>(pvParams);

    ttcInterface->processInfo();
}

/*********************************************************
 * 
 * Name:  mainLoop
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::mainLoop(void) {

    // Do whatever

    while(1) {
        
    }

    // Send error code serial msg if needed.
    vTaskDelete( NULL );
}