#include "ThrusterController.h"

ThrusterController::ThrusterController() {}

int ThrusterController::InitializeThrusters() {
    thrusterCommState = THRUSTER_INIT;
    return 0;
}

int ThrusterController::Run() { 
    // // Thruster Controller State
    // enum ThrusterControllerState {
    //   THRUSTER_INIT,
    //   THRUSTER_ARM,
    //   THRUSTER_TESTING,
    //   THRUSTER_GO,
    //   THRUSTER_NOGO
    // };

    switch (thrusterCommState) {
    case THRUSTER_INIT:
        // BlueRobotics suggest waiting 7 seconds.
        InitializeControls();
        
        vTaskDelay(pdMS_TO_TICKS(THRUSTER_READY_MS));
        
        left_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
        vTaskDelay(pdMS_TO_TICKS(THRUSTER_READY_MS));

        thrusterCommState = THRUSTER_ARM;
        break;
    case THRUSTER_ARM:
        // Thruster testing and thruster arm can probably be in the 
        // same state. SUV-19
        
        // Arm thrusters by sending
        left_thruster.writeMicroseconds(1600);
        
        // 50 * 100 = 5 Seconds
        vTaskDelay((THRUSTER_TASK_MS  * 50) / portTICK_PERIOD_MS);
        break;
    case THRUSTER_TESTING:
        // Sends a command to 1 motor at a time to spin for 2 seconds
        // and then stop, and then move to the next motor. 

        // Semaphore control, send information regarding
        // thruster test success and fail status.
        vTaskDelay(THRUSTER_TASK_MS / portTICK_PERIOD_MS);
        break;
    case THRUSTER_GO:
        // Construct thruster buffer to send

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
    right_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
    front_right_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
    front_left_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
    back_left_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
    back_right_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);

    return 0;
}

void ThrusterController::SlowMotorSpin() {
    left_thruster.writeMicroseconds(1600);
    right_thruster.writeMicroseconds(1600);
    front_right_thruster.writeMicroseconds(1600);
    front_left_thruster.writeMicroseconds(1600);
    back_left_thruster.writeMicroseconds(1600);
    back_right_thruster.writeMicroseconds(1600);
}

int ThrusterController::GetTaskMS() {
    return THRUSTER_TASK_MS;
}