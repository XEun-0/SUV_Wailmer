#include "sensorController.h"

#define BNO055_SAMPLERATE_DELAY_MS (100)

SensorController::SensorController() : bno(Adafruit_BNO055()), sensor() {}

int SensorController::InitializeSensors() {
    memset(sensorBuffer, 0, SENSOR_BUFFER_SIZE);
    memset(&sensor_info, 0, SENSOR_BUFFER_SIZE);
    int baroStatus =      InitializeBarometer();
    int imuStatus =       InitializeIMU();

    // Set validation boolean
    isValBitSet = false;
}

int SensorController::Run(TaskParams_t* params) { 
    sensor.read();

    // IMU get new sensor event
    sensors_event_t event; 
    bno.getEvent(&event);

    sensor_info.baroPressure =  sensor.pressure();
    sensor_info.baroTemp =      sensor.temperature();
    sensor_info.baroDepth =     sensor.depth();
    sensor_info.baroAltitude =  sensor.altitude();

    sensor_info.imuOrientX =    event.orientation.x;
    sensor_info.imuOrientY =    event.orientation.y;
    sensor_info.imuOrientZ =    event.orientation.z;
    sensor_info.imuTemp =       bno.getTemp();

    // bytes 0-27 = 28 bytes
    memcpy(&sensorBuffer[0],  &sensor_info.baroPressure,  sizeof(sensor_info.baroPressure));
    memcpy(&sensorBuffer[4],  &sensor_info.baroTemp,      sizeof(sensor_info.baroTemp));
    memcpy(&sensorBuffer[8],  &sensor_info.baroDepth,     sizeof(sensor_info.baroDepth));
    memcpy(&sensorBuffer[12], &sensor_info.baroAltitude,  sizeof(sensor_info.baroAltitude));

    memcpy(&sensorBuffer[16], &sensor_info.imuOrientX,    sizeof(sensor_info.imuOrientX));
    memcpy(&sensorBuffer[20], &sensor_info.imuOrientY,    sizeof(sensor_info.imuOrientY));
    memcpy(&sensorBuffer[24], &sensor_info.imuOrientZ,    sizeof(sensor_info.imuOrientZ));

    // bytes 28-29 = 1 byte
    memcpy(&sensorBuffer[28], &sensor_info.imuTemp, sizeof(sensor_info.imuTemp));

#if DEBUG_SERIAL != 1
    Serial.write((uint8_t*)&sensorBuffer, SENSOR_BUFFER_SIZE);
#else
    //Serial.println("Hello");

#endif
    if (params->snsrSemaphore != NULL) {
        // if (!params->statusMsg->GetIsSensorPopulated()) {
        if (xSemaphoreTake(params->snsrSemaphore, pdMS_TO_TICKS(100)) == pdTRUE) {
            Serial.println("[Sensor] has taken semaphore.");
            params->statusMsg->SetSensorOutInfo(sensor_info);
            // if ((params->statusMsg->GetValidationByte() & (1 << 7)) == 0b10000000) {
            //     Serial.println("Sensor msg stored.");
            //     isValBitSet = true;
            // }
            
            // Debug information
            // Print the buffer
            for (int i = 0; i < 32; i++) {
                Serial.print(sensorBuffer[i], HEX); // Print as hexadecimal
                Serial.print(",");
            }
            Serial.println("\n");
            for (int i = 0; i < 32; i++) {
                Serial.print(params->statusMsg->GetSensorOutInfo(i), HEX); // Print as hexadecimal
                Serial.print(",");
            }
            Serial.println(params->statusMsg->GetSensorOutInfo(32), HEX);
            Serial.println("\n");

            params->statusMsg->SetIsSensorPopulated(true);
            xSemaphoreGive(params->snsrSemaphore);
        } else {
            Serial.println("[Sensor] could not take semaphore.");
        }
        // }
        
        
    } else {
        Serial.println("[Sensor] Semaphore does not exist.");
    }
}

// Turns Barometer data collecting on
int SensorController::InitializeBarometer() {
    Wire.begin();       //Can use this if dealing with only 1 sensor
    //Wire.beginTransmission(0x76);
    while (!sensor.init()) {
        Serial.println("Initilization failed");
        delay(2000);
    }
    sensor.setModel(MS5837::MS5837_30BA);
    sensor.setFluidDensity(997);      //(air ~ 1.23, freshwater ~ 997, seawater ~1029)
    
    return 1;
}                                  

// Turns IMU data collecting on
int SensorController::InitializeIMU() {
    //Wire.begin();       //Can use this if dealing with only 1 sensor
    Wire.beginTransmission(0x28);
    if (!bno.begin()) {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("No BNO055 detected ... Check your wiring or I2C ADDR");
        while (1);
    }
    bno.setExtCrystalUse(true);

    return 1;
}

int SensorController::GetTaskMS() {
    return SENSORS_TASK_MS;
}