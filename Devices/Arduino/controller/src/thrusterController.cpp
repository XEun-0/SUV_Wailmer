#include "thrusterController.h"

ThrusterController::ThrusterController() {}

/*********************************************************
 * 
 * Name:  InitializeThrusters
 * Notes: See thrusterController.h
 * 
 *********************************************************/
int ThrusterController::InitializeThrusters() {
    thrusterCommState = THRUSTER_INIT;
    memset(&thruster_info, 0, THRUSTER_INFO_SIZE);
    return 0;
}

/*********************************************************
 * 
 * Name:  Run
 * Notes: See thrusterController.h
 * 
 *********************************************************/
int ThrusterController::Run(TaskParams_t* params) { 
    // // Thruster Controller State
    // enum ThrusterControllerState {
    //   THRUSTER_INIT,
    //   THRUSTER_ARM,
    //   THRUSTER_TESTING,
    //   THRUSTER_GO,
    //   THRUSTER_NOGO
    // };
    //Serial.println("Thruster task hit");
    
    thruster_info.thrusterControllerState = thrusterCommState;
    thruster_info.thrusterTestState = THRUSTER_ONE;

    switch (thrusterCommState) {
    case THRUSTER_INIT:
        #ifdef SERIAL_OUT
            Serial.println("THRUSTER_INIT");
        #endif
        // BlueRobotics suggest waiting 7 seconds.
        InitializeControls();
        
        vTaskDelay(pdMS_TO_TICKS(THRUSTER_READY_MS));

        left_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
        // right_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
        // front_right_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
        // front_left_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
        // back_left_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
        // back_right_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);

        vTaskDelay(pdMS_TO_TICKS(THRUSTER_READY_MS));

        thrusterCommState = THRUSTER_ARM;
        
        break;
    case THRUSTER_ARM:
        #ifdef SERIAL_OUT
            Serial.println("THRUSTER_ARM");
        #endif
        // Thruster testing and thruster arm can probably be in the 
        // same state. SUV-19
        
        // Arm thrusters by sending
        left_thruster.writeMicroseconds(1600);
        // right_thruster.writeMicroseconds(1600);
        // front_right_thruster.writeMicroseconds(1600);
        // front_left_thruster.writeMicroseconds(1600);
        // back_left_thruster.writeMicroseconds(1600);
        // back_right_thruster.writeMicroseconds(1600);
        
        // 50 * 100 = 5 Seconds
        vTaskDelay((THRUSTER_TASK_MS  * 50) / portTICK_PERIOD_MS);
        // Go to THRUSTER_GO state after arming the thrusters.
        // thruster testing state implemented later. 

        thrusterCommState = THRUSTER_GO;

        break;
    case THRUSTER_TESTING:
        #ifdef SERIAL_OUT
            Serial.println("THRUSTER_TESTING");
        #endif
        // Sends a command to 1 motor at a time to spin for 2 seconds
        // and then stop, and then move to the next motor. 

        // Semaphore control, send information regarding
        // thruster test success and fail status.

        vTaskDelay(THRUSTER_TASK_MS / portTICK_PERIOD_MS);

        break;
    case THRUSTER_GO:
        #ifdef SERIAL_OUT
            Serial.println("THRUSTER_GO");
        #endif
        // Construct thruster buffer to send
        if (params->xSerialMutex != NULL) {
            if (xSemaphoreTake(params->xSerialMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
                #ifdef SERIAL_OUT
                    Serial.println("[Thruster] has semaphore");
                #endif

                // struct ThrusterInfo {
                //     ThrusterControllerState   thrusterControllerState;
                //     ThrusterTestState         thrusterTestState;
                //     int16_t                   left_thruster_speed;
                //     int16_t                   right_thruster_speed;
                //     int16_t                   front_right_thruster_speed;
                //     int16_t                   front_left_thruster_speed;
                //     int16_t                   back_left_thruster_speed;
                //     int16_t                   back_right_thruster_speed;
                // }; // 2 + 2 + (2 x 6) = 16 bytes

                // TODO Serial read to get information and thruster speed accordingly
                // get thruster speed to set the thrusters, then fill out status
                // then send out back to VCS (Vehicle Control Software).
                // getThrusterSpeeds();

                thruster_info.thrusterSpeeds.left_thruster_speed = 1200;
                thruster_info.thrusterSpeeds.right_thruster_speed = 1200;
                thruster_info.thrusterSpeeds.front_right_thruster_speed = 1200;
                
                thruster_info.thrusterSpeeds.front_left_thruster_speed = 1200;
                thruster_info.thrusterSpeeds.back_left_thruster_speed = 1200;
                thruster_info.thrusterSpeeds.back_right_thruster_speed = 1200;

                params->statusMsg->SetThrusterOutInfo(&thruster_info);
                params->statusMsg->SetIsThrusterPopulated(true);
                
                vTaskDelay(pdMS_TO_TICKS(50));
                
                xEventGroupSetBits(params->xSerialEventGroup, THRUSTER_TASK_DONE);

                xSemaphoreGive(params->xSerialMutex);

            } else {
                #ifdef SERIAL_OUT
                    Serial.println("[Thruster] could not acquire semaphore.");
                #endif
            }

        } else {
            #ifdef SERIAL_OUT
                Serial.println("[Thruster] Semaphore does not exist.");
            #endif
        }
        
        // receive a command [ go down ]
        // commmand -> send the values of the command to my motors
        vTaskDelay(THRUSTER_TASK_MS / portTICK_PERIOD_MS);
        break;
    default:
        break;
    }
}

/*********************************************************
 * 
 * Name:  InitializeControls
 * Notes: See thrusterController.h
 * 
 *********************************************************/
int ThrusterController::InitializeControls() {
    // Assigned thruster hardware pins to servo objects
    left_thruster.attach(LEFT_THRUSTER_PIN);
    right_thruster.attach(RIGHT_THRUSTER_PIN);
    front_right_thruster.attach(FRONT_RIGHT_THRUSTER_PIN);
    front_left_thruster.attach(FRONT_LEFT_THRUSTER_PIN);
    back_left_thruster.attach(BACK_LEFT_THRUSTER_PIN);
    back_right_thruster.attach(BACK_RIGHT_THRUSTER_PIN);

    // Send stop command to thrusters
    left_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
    right_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
    front_right_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
    front_left_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
    back_left_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
    back_right_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);

    return 0;
}

/*********************************************************
 * 
 * Name:  SetThrusterSpeed
 * Notes: See thrusterController.h
 * 
 *********************************************************/
void ThrusterController::SetThrusterSpeed(ThrusterSpeedsType spds) {
    left_thruster.writeMicroseconds(spds.left_thruster_speed);
    right_thruster.writeMicroseconds(spds.right_thruster_speed);
    front_right_thruster.writeMicroseconds(spds.front_right_thruster_speed);
    front_left_thruster.writeMicroseconds(spds.front_left_thruster_speed);
    back_left_thruster.writeMicroseconds(spds.back_left_thruster_speed);
    back_right_thruster.writeMicroseconds(spds.back_right_thruster_speed);
}

/*********************************************************
 * 
 * Name:  SetSingleThrusterSpeed
 * Notes: See thrusterController.h
 * 
 *********************************************************/
void ThrusterController::SetSingleThrusterSpeed(Servo *thruster, uint16_t speed) {
    thruster->writeMicroseconds(speed);
}

/*********************************************************
 * 
 * Name:  SlowMotorSpin
 * Notes: See thrusterController.h
 * 
 *********************************************************/
void ThrusterController::SlowMotorSpin() {
    //SetThrusterSpeed(SLOW_SPEED);
}

/*********************************************************
 * 
 * Name:  GetTaskMS
 * Notes: See thrusterController.h
 * 
 *********************************************************/
int ThrusterController::GetTaskMS() {
    return THRUSTER_TASK_MS;
}

/*********************************************************
 * 
 * Name:  GetTaskHandle
 * Notes: See thrusterController.h
 * 
 *********************************************************/
TaskHandle_t* ThrusterController::GetTaskHandle() {
    return &ThrusterCommandsTaskHandle;
}