#include "TTCInterface.h"

#define BNO055_SAMPLERATE_DELAY_MS (100)

TTCInterface::TTCInterface(void) {

    ttc_initialized = false;

    // Sensor var init
    imuTemp         = 0;

    baroPressure    = 0;
    baroTemp        = 0;
    baroDepth       = 0;
    baroAltitude    = 0;

    imuOrientX      = 0;
    imuOrientY      = 0;
    imuOrientZ      = 0;

    // Thruster var init
    currThrusterState = THRUSTER_INIT;

    local_left_thruster_speed         = 0;
    local_right_thruster_speed        = 0;
    local_front_right_thruster_speed  = 0;
    local_front_left_thruster_speed   = 0;
    local_back_left_thruster_speed    = 0;
    local_back_right_thruster_speed   = 0;

    sohMutex = xSemaphoreCreateMutex();
    thruserBinarySemaphore = xSemaphoreCreateBinary(); // might not need
    if (!sohMutex) {
        // Error code implementation
    }
}

TTCInterface::~TTCInterface(void) {

}

//========================================================
//=========== Public Functions ===========================
//========================================================

/*********************************************************
 * 
 * Name:  doThrusterCommands
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::doThrusterCommands(ThrusterCommandsType cmds) {
    if (sohMutex && xSemaphoreTake(sohMutex, TTC_SENSOR_MUTEX) == pdTRUE) {
        
        // left_thruster.writeMicroseconds(        spds);
        // right_thruster.writeMicroseconds(       spds);
        // front_right_thruster.writeMicroseconds( spds);
        // front_left_thruster.writeMicroseconds(  spds);
        // back_left_thruster.writeMicroseconds(   spds);
        // back_right_thruster.writeMicroseconds(  spds);
        
        xSemaphoreGive(sohMutex);
    } else {
        // send error code through serial
        #ifdef SERIAL_OUT
        Serial.println("[TTC] processInfo could not take semaphore");
        #endif
    }
}

/*********************************************************
 * 
 * Name:  gatherSensorSOH
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::gatherSensorSOH(TTCSohRespType *pInfo) {

    if (sohMutex && xSemaphoreTake(sohMutex, TTC_SENSOR_MUTEX) == pdTRUE) {
        #ifdef SERIAL_OUT
        //Serial.println("[TTC] gatherSensorSOH has semaphore");
        #endif

        pInfo->sensorInfo.baroPressure   = baroPressure;
        pInfo->sensorInfo.baroTemp       = baroTemp;
        pInfo->sensorInfo.baroDepth      = baroDepth;
        pInfo->sensorInfo.baroAltitude   = baroAltitude;

        pInfo->sensorInfo.imuOrientX     = imuOrientX;
        pInfo->sensorInfo.imuOrientY     = imuOrientY;
        pInfo->sensorInfo.imuOrientZ     = imuOrientZ;
        pInfo->sensorInfo.imuTemp        = imuTemp;

        xSemaphoreGive(sohMutex);
    } else {
        // send error code through serial
        #ifdef SERIAL_OUT
        //Serial.println("[TTC] gatherSensorSOH could not take semaphore");
        #endif
    }
}

/*********************************************************
 * 
 * Name:  gatherThrusterSOH
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::gatherThrusterSOH(TTCSohRespType *pInfo) {
    if (sohMutex && xSemaphoreTake(sohMutex, TTC_THRUSTER_MUTEX) == pdTRUE) {
        #ifdef SERIAL_OUT
        //Serial.println("[TTC] gatherThrusterSOH has semaphore");
        #endif

        pInfo->thrusterInfo.thrusterControllerState = (ThrusterState)currThrusterState;

        if (currThrusterState == THRUSTER_GO) {
            pInfo->thrusterInfo.thrusterSpeeds.left_thruster_speed = local_left_thruster_speed;
            pInfo->thrusterInfo.thrusterSpeeds.right_thruster_speed = local_right_thruster_speed;
            pInfo->thrusterInfo.thrusterSpeeds.front_right_thruster_speed = local_front_right_thruster_speed;

            pInfo->thrusterInfo.thrusterSpeeds.front_left_thruster_speed = local_front_left_thruster_speed;
            pInfo->thrusterInfo.thrusterSpeeds.back_left_thruster_speed = local_back_left_thruster_speed;
            pInfo->thrusterInfo.thrusterSpeeds.back_right_thruster_speed = local_back_right_thruster_speed;
        } else {
            memset(&pInfo->thrusterInfo.thrusterSpeeds, -1, sizeof(ThrusterSpeedsType));
        }

        xSemaphoreGive(sohMutex);
    } else {
        // send error code through serial
        #ifdef SERIAL_OUT
        Serial.println("[TTC] gatherThrusterSOH could not take semaphore");
        #endif
    }
}

/*********************************************************
 * 
 * Name:  isInitialized
 * Notes: See controller.h
 * 
 *********************************************************/
bool TTCInterface::isInitialized(void) {
    return ttc_initialized;
}

/*********************************************************
 * 
 * Name:  TTCTaskLauncher
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::TTCTaskLauncher() {
    xTaskCreate(
        processInfoStatic,              // Function to be called
        "processInfoStatic",            // Name of the task
        512,                            // Stack size
        this,                           // Parameters passed to task
        2,                              // Task priority (higher number = higher priority)
        NULL                            // Task handle for reference
    );
}

//========================================================
//=========== Private Functions ==========================
//========================================================

/*********************************************************
 * 
 * Name:  init
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::init(void) {
    ttc_initialized = true;

    baroInitStatus =    initializeBarometer();
    imuInitStatus =     initializeIMU();

    // memset(&thruster_info, 0, THRUSTER_INFO_SIZE);

    // Serial.println(sizeof(ThrusterIndividualTestProgress)); // verified 2 bytes
    // Serial.println(sizeof(SensorInfoType));     // 36
    // Serial.println(sizeof(ThrusterInfoType));   // 16
    // Serial.println(sizeof(TTCSohRespType));     // 52

    initializeThrusters();
}

/*********************************************************
 * 
 * Name:  initializeBarometer
 * Notes: See controller.h
 * 
 *********************************************************/
SensorInitStatusCodeType TTCInterface::initializeBarometer() {
    // Can use this if dealing with only 1 sensor
    Wire.begin();

    // Wire.beginTransmission(0x76);

    SensorInitStatusCodeType tempBaroInitStatus = BARO_GOOD;
    
    TickType_t startTickCount = xTaskGetTickCount();
    
    while (!baro.init()) {
        tempBaroInitStatus = ERROR_NO_BARO;
        TickType_t currTickCount = xTaskGetTickCount();
        #ifdef SERIAL_OUT
        Serial.println(currTickCount - startTickCount);
        #endif
        if (currTickCount - startTickCount > 10) {
            // Timeout after 10 seconds roughly
            // at a rate of 1 retry per 2 seconds
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(2000)); // 2 seconds
    }

    baro.setModel(MS5837::MS5837_30BA);
    //(air ~ 1.23, freshwater ~997, seawater ~1029)
    baro.setFluidDensity(997);      
    
    return tempBaroInitStatus;
}                                  

/*********************************************************
 * 
 * Name:  initializeIMU
 * Notes: See controller.h
 * 
 *********************************************************/
SensorInitStatusCodeType TTCInterface::initializeIMU() {
    // Can use this if dealing with only 1 sensor
    Wire.beginTransmission(0x28);
    
    SensorInitStatusCodeType tempImuInitStatus = IMU_GOOD;
    
    TickType_t startTickCount = xTaskGetTickCount();

    while (!bno.begin()) {
        tempImuInitStatus = ERROR_NO_BARO;
        TickType_t currTickCount = xTaskGetTickCount();
        #ifdef SERIAL_OUT
        Serial.println(currTickCount - startTickCount);
        #endif
        if (currTickCount - startTickCount > 10) {
            // Timeout after 10 seconds roughly
            // at a rate of 1 retry per second
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second
    }
    
    bno.setMode(OPERATION_MODE_CONFIG);

    vTaskDelay(pdMS_TO_TICKS(1000));

    // Load calibration data
    bno.setSensorOffsets(bnoCalibrationData);

    // Switch back to normal operation mode
    bno.setMode(OPERATION_MODE_NDOF);

    vTaskDelay(pdMS_TO_TICKS(1000));

    bno.setExtCrystalUse(true);

    return tempImuInitStatus;
}

/*********************************************************
 * 
 * Name:  initializeThrusters
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::initializeThrusters(void) {
    
    // Assigned thruster hardware pins to servo objects
    left_thruster.attach(           LEFT_THRUSTER_PIN);
    right_thruster.attach(          RIGHT_THRUSTER_PIN);
    front_right_thruster.attach(    FRONT_RIGHT_THRUSTER_PIN);
    front_left_thruster.attach(     FRONT_LEFT_THRUSTER_PIN);
    back_left_thruster.attach(      BACK_LEFT_THRUSTER_PIN);
    back_right_thruster.attach(     BACK_RIGHT_THRUSTER_PIN);

    // Send stop command to thrusters
    left_thruster.writeMicroseconds(        MOTOR_STOP_COMMAND);
    right_thruster.writeMicroseconds(       MOTOR_STOP_COMMAND);
    front_right_thruster.writeMicroseconds( MOTOR_STOP_COMMAND);
    front_left_thruster.writeMicroseconds(  MOTOR_STOP_COMMAND);
    back_left_thruster.writeMicroseconds(   MOTOR_STOP_COMMAND);
    back_right_thruster.writeMicroseconds(  MOTOR_STOP_COMMAND);

}

/*********************************************************
 * 
 * Name:  printSensorInitStatusCodeType
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::printSensorInitStatusCodeType(
    SensorInitStatusCodeType in) 
{
    
}

/*********************************************************
 * 
 * Name:  processInfo
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::processInfo() {

    init();

    while(imuInitStatus == IMU_GOOD && baroInitStatus == BARO_GOOD) {
        #ifdef SERIAL_OUT
        //Serial.println("[TTC] this is inside TTCInterface");
        #endif
        if (sohMutex && xSemaphoreTake(sohMutex, TTC_SENSOR_MUTEX) == pdTRUE) {
            #ifdef SERIAL_OUT
            //Serial.println("[TTC] processInfo has semaphore");
            #endif
            
            baro.read();

            // Sensors
            sensors_event_t event; 
            bno.getEvent(&event);
            
            baroPressure    = baro.pressure();
            baroTemp        = baro.temperature();
            baroDepth       = baro.depth();
            baroAltitude    = baro.altitude();

            imuOrientX      = event.orientation.x;
            imuOrientY      = event.orientation.y;
            imuOrientZ      = event.orientation.z;
            imuTemp         = bno.getTemp();

            // Incorporate calibration tasking later
            //=====================================================================
            // char cal_buffer[50];  // Ensure the buffer is large enough
            // uint8_t sys, gyr, acc, mag;
            // bno.getCalibration(&sys, &gyr, &acc, &mag);

            // sprintf(cal_buffer, "CALIBRATION: SYS=%d GYR=%d ACC=%d MAG=%d", sys, gyr, acc, mag);
            // Serial.println(cal_buffer);

            // if (sys == 3 && gyr == 3 && acc == 3 && mag == 3) {
            //     uint8_t calData[22];
            //     bno.getSensorOffsets(calData);
                
            //     Serial.print("Calibration Data: ");
            //     for (int i = 0; i < 22; i++) {
            //         Serial.print(calData[i], HEX);
            //         Serial.print(" ");
            //     }
            //     Serial.println();
            // }
            //=====================================================================

            // Thrusters
            processThrusterState();

            xSemaphoreGive(sohMutex);
            
        } else {
            // send error code through serial
            #ifdef SERIAL_OUT
            Serial.println("[TTC] processInfo could not take semaphore");
            #endif
        }
        
        vTaskDelay(TTC_TASK_DELAY);
    }

    // EOL for TTC task
}

/*********************************************************
 * 
 * Name:  processInfoStatic
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::processInfoStatic(void *pvParams) {
    // Retrieve the singleton instance of the TTCInterface class.
    // This ensures that only one instance of TTCInterface exists 
    // and is used throughout the program.

    TTCInterface *ttcInterface = static_cast<TTCInterface *>(pvParams);
    ttcInterface->processInfo();
}

/*********************************************************
 * 
 * Name:  processThrusterState
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::processThrusterState() {
    // Thruster control has special state
    // machine inside to make sure it can 
    // run some visual tests

    static TickType_t startTickCount = 0;
    TickType_t currTickCount = xTaskGetTickCount();

    switch (currThrusterState) {
        case THRUSTER_INIT:
            
            if (currTickCount - startTickCount >= FIVE_SECONDS_DELAY) {
                #ifdef SERIAL_OUT
                Serial.println("change to THRUSTER_ARM state");
                #endif
                currThrusterState   = THRUSTER_ARM;
                startTickCount      = currTickCount;
            }
            break;
        case THRUSTER_ARM:
            
            setThrusterSpeed(MOTOR_STOP_COMMAND);

            if (currTickCount - startTickCount >= TEN_SECONDS_DELAY) {
                #ifdef SERIAL_OUT
                Serial.println("change to THRUSTER_TESTING state");
                #endif
                currThrusterState   = THRUSTER_TESTING;
                startTickCount      = currTickCount;
            }
            break;
        case THRUSTER_TESTING:

            // do whatever
            // Use ThrusterTestProgress
            // Want to clear validity masks somewhere beforehand.

            if (currTickCount - startTickCount >= FIVE_SECONDS_DELAY) {
                #ifdef SERIAL_OUT
                Serial.println("change to THRUSTER_GO state");
                #endif
                currThrusterState   = THRUSTER_GO;
                startTickCount      = currTickCount;
            }
            break;
        case THRUSTER_GO:

            // do whatever
            // update local thruster values if needed
            // from commands

            // then update actual thrusters

            // Here, most likely we want to process any information that
            // involves changing of the motor values of the robot
            // delay from VCP might be 0.5 - 1 seconds

            break;
        case THRUSTER_NOGO:

            break;
        default:
            break;
    }
}

/*********************************************************
 * 
 * Name:  setThrusterSpeed
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::setThrusterSpeed(int16_t spds) {
    
    left_thruster.writeMicroseconds(        spds);
    right_thruster.writeMicroseconds(       spds);
    front_right_thruster.writeMicroseconds( spds);
    front_left_thruster.writeMicroseconds(  spds);
    back_left_thruster.writeMicroseconds(   spds);
    back_right_thruster.writeMicroseconds(  spds);

}

/*********************************************************
 * 
 * Name:  setThrusterSpeed
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::setThrusterSpeed(ThrusterSpeedsType spds) {
    
    left_thruster.writeMicroseconds(        spds.left_thruster_speed);
    right_thruster.writeMicroseconds(       spds.right_thruster_speed);
    front_right_thruster.writeMicroseconds( spds.front_right_thruster_speed);
    front_left_thruster.writeMicroseconds(  spds.front_left_thruster_speed);
    back_left_thruster.writeMicroseconds(   spds.back_left_thruster_speed);
    back_right_thruster.writeMicroseconds(  spds.back_right_thruster_speed);

}

/*********************************************************
 * 
 * Name:  setLocalThrusterSpeed
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::setLocalThrusterSpeed(int16_t spds) {
    
    // left_thruster.writeMicroseconds(        spds);
    // right_thruster.writeMicroseconds(       spds);
    // front_right_thruster.writeMicroseconds( spds);
    // front_left_thruster.writeMicroseconds(  spds);
    // back_left_thruster.writeMicroseconds(   spds);
    // back_right_thruster.writeMicroseconds(  spds);

}

/*********************************************************
 * 
 * Name:  setLocalThrusterSpeed
 * Notes: See controller.h
 * 
 *********************************************************/
void TTCInterface::setLocalThrusterSpeed(ThrusterSpeedsType spds) {
    
    // left_thruster.writeMicroseconds(        spds.left_thruster_speed);
    // right_thruster.writeMicroseconds(       spds.right_thruster_speed);
    // front_right_thruster.writeMicroseconds( spds.front_right_thruster_speed);
    // front_left_thruster.writeMicroseconds(  spds.front_left_thruster_speed);
    // back_left_thruster.writeMicroseconds(   spds.back_left_thruster_speed);
    // back_right_thruster.writeMicroseconds(  spds.back_right_thruster_speed);

}