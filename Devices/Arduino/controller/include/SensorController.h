#ifndef SENSORCONTROLLER_H
#define SENSORCONTROLLER_H

#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#include "statusMsg.h"
#include "Common/commonTypes.h"

// Macros
#define SENSOR_BUFFER_SIZE              32
#define CHECKSUM_SIZE                   2
#define STX_AND_ETX_BYTES               4
#define OUT_BUFFER_SIZE                 SENSOR_BUFFER_SIZE + CHECKSUM_SIZE + STX_AND_ETX_BYTES

#define DEBUG_SENSORS           0
#define DEBUG_TXRX              0
#define DEBUG_SINGLE_TXRX       0
#define DEBUG_SERIAL            1

#define STX 0x02
#define ETX 0x03

#define SERIAL_SEND_MS      150
#define SENSORS_TASK_MS     150

class SensorController {
public:
    SensorController();
    
    int Run(TaskParams_t* params);
    int InitializeSensors();
    int GetTaskMS();

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

    TaskHandle_t AggregateSensorsTaskHandle;
private:
    Adafruit_BNO055 bno;
    MS5837 sensor;
    SensorInfo sensor_info;
    uint8_t sensorBuffer[SENSOR_BUFFER_SIZE];
    int InitializeBarometer();        
    int InitializeIMU();
};

#endif // SENSORCONTROLLER_H
