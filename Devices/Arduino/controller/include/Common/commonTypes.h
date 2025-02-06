#ifndef COMMONSTRUCTS_H
#define COMMONSTRUCTS_H

#include <Wire.h>               // Arduino class
#include <stdint.h>             
#include <Arduino_FreeRTOS.h>   // Arduino FreeRTOS class
#include <semphr.h>             // semaphore class
#include <event_groups.h>       // event group

#define SENSOR_TASK_DONE    (1 << 0)
#define THRUSTER_TASK_DONE  (1 << 1)

// Forward declarations

class StatusMsg;

// Enums

/*********************************************************
 * 
 * Name:  ProgressState_t
 * Notes: n/a
 * 
 *********************************************************/
typedef enum {
  INITIALIZED_SENSORS,
  GO,
  RUNNING,
  NOGO
} ProgressState_t;

/*********************************************************
 * 
 * Name:  ThrusterControllerState
 * Notes: Current thruster controller state
 * 
 *********************************************************/
enum ThrusterControllerState {
  THRUSTER_INIT,
  THRUSTER_ARM,
  THRUSTER_TESTING,
  THRUSTER_GO,
  THRUSTER_NOGO
};

/*********************************************************
 * 
 * Name:  ThrusterTestState
 * Notes: Enum for current thruster being tested.
 * 
 *********************************************************/
enum ThrusterTestState {
  THRUSTER_ONE,       // 0
  THRUSTER_TWO,
  THRUSTER_THREE,
  THRUSTER_FOUR,
  THRUSTER_FIVE,
  THRUSTER_SIX
};

// Structs

/*********************************************************
 * 
 * Name:  TaskParams_t
 * Notes: Parameter struct for each task variable sharing.
 * 
 *********************************************************/
typedef struct {
  SemaphoreHandle_t     xSerialMutex;
  EventGroupHandle_t    xSerialEventGroup;
  StatusMsg*            statusMsg;
} TaskParams_t;

/*********************************************************
 * 
 * Name:  SensorInfo
 * Notes: Sensor information aggregation struct.
 * 
 *********************************************************/
struct SensorInfo {
  float     baroPressure;     // 4
  float     baroTemp;         // 4
  float     baroDepth;        // 4
  float     baroAltitude;     // 4

  float     imuOrientX;       // 4
  float     imuOrientY;       // 4
  float     imuOrientZ;       // 4
  uint8_t   imuTemp;          // 1
  uint8_t   padding1[1];      // 1 byte. Current patch leaving at 1 padding byte just in
                              // checksum might need to be used. 
  uint16_t  checksum;         // 2
}; // 4 x 7 + 1 + 2 = 32 bytes

/*********************************************************
 * 
 * Name:  ThrusterSpeedsType
 * Notes: n/a
 * 
 *********************************************************/
struct ThrusterSpeedsType {
  int16_t   left_thruster_speed;
  int16_t   right_thruster_speed;
  int16_t   front_right_thruster_speed;
  int16_t   front_left_thruster_speed;
  int16_t   back_left_thruster_speed;
  int16_t   back_right_thruster_speed;
}; // Total Size: 2 + 2 + 2 + 2 + 2 + 2 = 12 bytes

/*********************************************************
 * 
 * Name:  ThrusterInfo
 * Notes: Current information about thruster.
 * 
 *********************************************************/
struct ThrusterInfo {
  ThrusterControllerState   thrusterControllerState;
  ThrusterTestState         thrusterTestState;
  ThrusterSpeedsType        thrusterSpeeds;
}; // 2 + 2 + (2 x 6) = 16 bytes

#endif //COMMONSTRUCTS_H