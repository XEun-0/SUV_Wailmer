#include "controller.h"
#include "Common/taskGlobals.h"        // TEN_SECONDS_DELAY

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
        Controller::instance->init(); // may not be needed
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
    l_ttcInterfaceInitialized = false;
}

/*********************************************************
 * 
 * Name:  mainLoop
 * Notes: See controller.h
 * 
 *********************************************************/
void Controller::mainLoop(void) {

    // Do whatever

    while(!gTTCInterface.isInitialized()) {
        vTaskDelay(TEN_SECONDS_DELAY);
    }

    l_ttcInterfaceInitialized = true;
    
    while(1) {
        #ifdef SERIAL_OUT
        Serial.println("[Controller] this is inside main controller");
        #endif

        getSendSOHSerial();
        // getSensorInfo
        
        // getThrusterInfo

        // sendSOH

        // getCommandsFromVCP

        vTaskDelay(pdMS_TO_TICKS(MAIN_CONTROLLER_TASK_DELAY));
    }

    // Send error code serial msg if needed.
    vTaskDelete( NULL );
}

void Controller::getSendSOHSerial(void) {
    TTCSohRespType ttcSoh;
    memset(&ttcSoh, 0, sizeof(TTCSohRespType));

    // Waht is SOH - State of Health
    // Stats and information about the specific component 
    // in question. i.e. Sensors, Thrusters
    gTTCInterface.gatherSensorSOH(&ttcSoh);

    gTTCInterface.gatherThrusterSOH(&ttcSoh);

    //Serial.println("Printing Values:");
    //Serial.println(ttcSoh.sensorInfo.imuOrientX);
    #if !defined(SERIAL_OUT)
    Serial.write((uint8_t *)&ttcSoh, sizeof(TTCSohRespType));
    #endif
    // process msg before sending out
    //hexDump((uint8_t *)&ttcSoh, sizeof(TTCSohRespType));
    #if SERIAL_OUT
    checkStructSizes();
    #endif

    // Serial.println(ttcSoh.sensorInfo.imuOrientX);
}

void Controller::rxSerialCommands(void) {
    // From Serial comms recieve commands and 
    // populate command struct for TTCInterface
    // tasking
    ThrusterCommandsType rxThrusterCmds;
    // Serial.read something and store in buffer

    gTTCInterface.doThrusterCommands(rxThrusterCmds);
}

// Not in class

/*********************************************************
 * 
 * Name:  controllerTaskLauncher
 * Notes: See controller.h
 * 
 *********************************************************/
void controllerTaskLauncher( void *pvParams ) {
    // Retrieve the singleton instance of the Controller class.
    // This ensures that only one instance of Controller exists 
    // and is used throughout the program.
    Controller *pController = Controller::getInstance();
    pController->mainLoop();
}

