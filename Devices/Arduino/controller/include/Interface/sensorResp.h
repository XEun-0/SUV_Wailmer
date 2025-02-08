#ifndef SENSORRESP_H
#define SENSORRESP_H

#include <Wire.h>               // Arduino class
#include <stdint.h>             
#include <Arduino_FreeRTOS.h>   // Arduino FreeRTOS class
#include <semphr.h>             // semaphore class
#include <event_groups.h>       // event group

#define SENSOR_TASK_DONE    (1 << 0)

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

#endif // SENSORRESP_H