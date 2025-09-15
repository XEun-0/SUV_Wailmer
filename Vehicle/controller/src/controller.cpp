#include "controller.h"
#include "Common/taskGlobals.h"        // TEN_SECONDS_DELAY
#include "pPrintf.h"

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
        // Serial.println("[Controller] this is inside main controller");
        // pPrintf("hello %d\n", 8);
        // testSomething();
        #endif

        #ifdef TIME_FUNC
        TickType_t startTick = xTaskGetTickCount();
        #endif

        getSendSOHSerial();

        #ifdef TIME_FUNC
        TickType_t currTick = xTaskGetTickCount();
        Serial.print("It took getSendSOHSerial: ");
        Serial.print(currTick - startTick);
        Serial.print(" ticks\n");
        #endif
        
        // getThrusterInfo

        // sendSOH

        // getCommandsFromVCP

        vTaskDelay(MAIN_CONTROLLER_TASK_DELAY);
    }

    // Send error code serial msg if needed.
    vTaskDelete( NULL );
}

/*********************************************************
 * 
 * Name:  getSendSOHSerial
 * Notes: See controller.h
 * 
 *********************************************************/
void Controller::getSendSOHSerial(void) {
    TTCSohRespType ttcSoh;
    memset(&ttcSoh, 0, sizeof(TTCSohRespType));

    // What is SOH - State of Health
    // Stats and information about the specific component 
    // in question. i.e. Sensors, Thrusters
    gTTCInterface.gatherSensorSOH(&ttcSoh);

    gTTCInterface.gatherThrusterSOH(&ttcSoh);
    
    #if !defined(SERIAL_OUT)
    Serial.write((uint8_t *)&ttcSoh, sizeof(TTCSohRespType));
    #else
    // char avr_buffer[100];

    // char floatX[10], floatY[10], floatZ[10];
    // dtostrf(ttcSoh.sensorInfo.imuOrientX, 6, 2, floatX); // Width 6, 2 decimal places
    // dtostrf(ttcSoh.sensorInfo.imuOrientY, 6, 2, floatY);
    // dtostrf(ttcSoh.sensorInfo.imuOrientZ, 6, 2, floatZ);

    // sprintf(avr_buffer, "X: %s, Y: %s, Z: %s", floatX, floatY, floatZ);
    // Serial.println(avr_buffer);
    #endif

    // process msg before sending out

    #if SERIAL_OUT
    printf("[ HexDump ] print: ")
    hexDump((uint8_t *)&ttcSoh, sizeof(TTCSohRespType));
    // checkStructSizes();
    #endif

    // Serial.println(ttcSoh.sensorInfo.imuOrientX);
}

/*********************************************************
 * 
 * Name:  rxSerialCommands
 * Notes: See controller.h
 * 
 *********************************************************/
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

