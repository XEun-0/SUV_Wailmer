#ifndef COMMONSTRUCTS_H
#define COMMONSTRUCTS_H

#include <stdint.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Forward declaration for StatusMsg class
class StatusMsg;

// Structs
/**
 * Parameter struct for each task variable sharing.
 */
typedef struct {
  SemaphoreHandle_t   msgSemaphore;
  StatusMsg*          statusMsg;
} TaskParams_t;

/**
 * Sensor information aggregation struct.
 */
struct SensorInfo {
  float baroPressure;     // 4
  float baroTemp;         // 4
  float baroDepth;        // 4
  float baroAltitude;     // 4

  float imuOrientX;       // 4
  float imuOrientY;       // 4
  float imuOrientZ;       // 4
  uint8_t imuTemp;        // 1
  uint8_t padding1[1];    // 1 byte. Current patch leaving at 1 padding byte just in
                          // checksum might need to be used. 
  uint16_t checksum;      // 2
}; // 4 x 7 + 1 + 2 = 32 bytes

// Enums
/**
 * Progression state. Unused currently.
 */
typedef enum {
  INITIALIZED_SENSORS,
  GO,
  RUNNING,
  NOGO
} ProgressState_t;

/**
 * Current thruster controller state.
 * THRUSTER_INIT            Attach thruster motors to correct pin number
 * THRUSTER_ARM             Wait until thrusters ready and recieve STOP command
 * THRUSTER_TESTING         Testing state-machine state for making sure all thrusters
 *                          are spinning or able to recieve spin commands. 
 * THRUSTER_GO              
 * THRUSTER_NOGO
 */
enum ThrusterControllerState {
  THRUSTER_INIT,
  THRUSTER_ARM,
  THRUSTER_TESTING,
  THRUSTER_GO,
  THRUSTER_NOGO
};

/**
 * Enum for current thruster being tested.
 */
enum ThrusterTestState {
  THRUSTER_ONE,
  THRUSTER_TWO,
  THRUSTER_THREE,
  THRUSTER_FOUR,
  THRUSTER_FIVE,
  THRUSTER_SIX
};

/**
 * Current information about thruster.
 */
struct ThrusterInfo {
  ThrusterControllerState   thrusterControllerState;
  ThrusterTestState         thrusterTestState;
  int16_t                   left_thruster_speed;
  int16_t                   right_thruster_speed;
  int16_t                   front_right_thruster_speed;
  int16_t                   front_left_thruster_speed;
  int16_t                   back_left_thruster_speed;
  int16_t                   back_right_thruster_speed;
}; // 2 + 2 + (2 x 6) = 16 bytes

#endif //COMMONSTRUCTS_H