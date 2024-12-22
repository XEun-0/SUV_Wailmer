#include "thrusterController.h"

ThrusterController::ThrusterController() {}

int ThrusterController::InitializeThrusters() {
    thrusterCommState = THRUSTER_INIT;
    memset(&thruster_info, 0, THRUSTER_INFO_SIZE);
    return 0;
}

int ThrusterController::Run(TaskParams_t* params) { 
    // // Thruster Controller State
    // enum ThrusterControllerState {
    //   THRUSTER_INIT,
    //   THRUSTER_ARM,
    //   THRUSTER_TESTING,
    //   THRUSTER_GO,
    //   THRUSTER_NOGO
    // };
    Serial.println("Thruster task hit");
    

    switch (thrusterCommState) {
    case THRUSTER_INIT:
        Serial.println("THRUSTER_INIT");

        // BlueRobotics suggest waiting 7 seconds.
        InitializeControls();
        
        vTaskDelay(pdMS_TO_TICKS(THRUSTER_READY_MS));
        
        left_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
        vTaskDelay(pdMS_TO_TICKS(THRUSTER_READY_MS));

        thrusterCommState = THRUSTER_ARM;
        break;
    case THRUSTER_ARM:
        Serial.println("THRUSTER_ARM");
        
        // Thruster testing and thruster arm can probably be in the 
        // same state. SUV-19
        
        // Arm thrusters by sending
        left_thruster.writeMicroseconds(1600);
        
        // 50 * 100 = 5 Seconds
        vTaskDelay((THRUSTER_TASK_MS  * 50) / portTICK_PERIOD_MS);
        // Go to THRUSTER_GO state after arming the thrusters.
        // thruster testing state implemented later. 
        thrusterCommState = THRUSTER_GO;
        break;
    case THRUSTER_TESTING:
        Serial.println("THRUSTER_TESTING");
        // Sends a command to 1 motor at a time to spin for 2 seconds
        // and then stop, and then move to the next motor. 

        // Semaphore control, send information regarding
        // thruster test success and fail status.
        vTaskDelay(THRUSTER_TASK_MS / portTICK_PERIOD_MS);
        break;
    case THRUSTER_GO:
        Serial.println("THRUSTER_GO");
        // Construct thruster buffer to send
        if (params->thrsSemaphore != NULL) {
            // if (!params->statusMsg->GetIsThrusterPopulated()) {
            if (xSemaphoreTake(params->thrsSemaphore, pdMS_TO_TICKS(100)) == pdTRUE) {
                Serial.println("[Thruster] has semaphore");
                
                vTaskDelay(pdMS_TO_TICKS(50)); 

                params->statusMsg->SetIsThrusterPopulated(true);
                xSemaphoreGive(params->thrsSemaphore);
            } else {
                Serial.println("[Thruster] could not acquire semaphore.");
            }
            // }
            
        } else {
            Serial.println("[Thruster] Semaphore does not exist.");
        }
        // receive a command [ go down ]

        // commmand -> send the values of the command to my motors
        vTaskDelay(THRUSTER_TASK_MS / portTICK_PERIOD_MS);
        break;
    default:
        break;
    }
}

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

void ThrusterController::SetThrusterSpeed(uint16_t speed) {
    left_thruster.writeMicroseconds(speed);
    right_thruster.writeMicroseconds(speed);
    front_right_thruster.writeMicroseconds(speed);
    front_left_thruster.writeMicroseconds(speed);
    back_left_thruster.writeMicroseconds(speed);
    back_right_thruster.writeMicroseconds(speed);
}


void ThrusterController::SlowMotorSpin() {
    SetThrusterSpeed(SLOW_SPEED);
}

int ThrusterController::GetTaskMS() {
    return THRUSTER_TASK_MS;
}