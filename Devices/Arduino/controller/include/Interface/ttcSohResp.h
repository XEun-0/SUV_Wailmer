#ifndef TTSSOHRESP_H
#define TTSSOHRESP_H

#if defined(__AVR_ATmega2560__)
#include <Wire.h>               // Arduino class
#include <Arduino_FreeRTOS.h>   // Arduino FreeRTOS class
#include <semphr.h>             // semaphores
#endif
#include <stdint.h>             


// CONFIGURATION: https://docs.platformio.org/page/boards/atmelavr/megaatmega2560.html
// PLATFORM: Atmel AVR (5.1.0) > Arduino Mega or Mega 2560 ATmega2560 (Mega 2560)
// HARDWARE: ATMEGA2560 16MHz, 8KB RAM, 248KB Flash

//========================================================
//=========== Enums ======================================
//========================================================

enum ThrusterState {
    THRUSTER_INIT,          // 0
    THRUSTER_ARM,
    THRUSTER_TESTING,
    THRUSTER_GO,
    THRUSTER_NOGO
};

enum ThrusterTestState {
    THRUSTER_ONE,           // 0
    THRUSTER_TWO,
    THRUSTER_THREE,
    THRUSTER_FOUR,
    THRUSTER_FIVE,
    THRUSTER_SIX
};

enum ThrusterTestingState {
    THRUSTER_TEST_DONE,     // 0
    THRUSTER_TEST_GO,
    THRUSTER_TEST_NOGO
};

//========================================================
//=========== Structs ====================================
//========================================================

struct SensorInfoType {
  uint8_t   soh_id;           // 1
  uint8_t   align1;           // 1
  uint16_t  align2;           // 2
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
}; // Total Size: (2 * 1) + 2 + (7 * 4) + 1 + 1 + 2 = 36 bytes

// packed struct each field needs to be range 0-2
// use ThrusterTestingState
struct __attribute__((packed)) ThrusterIndividualTestProgress {
    uint8_t     thruster_1_test_prog    : 2;    // lsb
    uint8_t     thruster_2_test_prog    : 2;
    uint8_t     thruster_3_test_prog    : 2;
    uint8_t     thruster_4_test_prog    : 2;
    uint8_t     thruster_5_test_prog    : 2;
    uint8_t     thruster_6_test_prog    : 2;
    uint16_t    padding                 : 4;
}; // Total Size: (6 * 2) + 4 = 16 bits (2 bytes)

// struct ThrusterTestProgress {
//     int8_t      testStateValidityMask;
//     int8_t      thrusterNum[6];
// }

struct ThrusterSpeedsType {
  int16_t   left_thruster_speed;
  int16_t   right_thruster_speed;
  int16_t   front_right_thruster_speed;
  int16_t   front_left_thruster_speed;
  int16_t   back_left_thruster_speed;
  int16_t   back_right_thruster_speed;
}; // Total Size: 2 + 2 + 2 + 2 + 2 + 2 = 12 bytes

#if defined(__AVR_ATmega2560__)
struct ThrusterInfoType {
#else
struct __attribute__((packed)) ThrusterInfoType {
#endif
  ThrusterState                     thrusterControllerState;
  ThrusterIndividualTestProgress    thrusterTestState;
  ThrusterSpeedsType                thrusterSpeeds;
}; // Total Size: 2 + 2 + 12 = 16 bytes

#if defined(__AVR_ATmega2560__)
struct TTCSohRespType {
#else
struct __attribute__((packed)) TTCSohRespType {
#endif
    SensorInfoType      sensorInfo;
    ThrusterInfoType    thrusterInfo;
}; // Total Size: 36 + 16 = 52 bytes

struct ThrusterCommandsType {
    ThrusterSpeedsType  rxThrusterSpeeds;
}; // Total Size: 12 bytes

#endif // TTSSOHRESP_H