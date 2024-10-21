#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();
MS5837 sensor;

// Macros
#define SENSOR_BUFFER_SIZE 32
#define CHECKSUM_SIZE 2
#define STX_AND_ETX_BYTES 4
#define OUT_BUTTER_SIZE  SENSOR_BUFFER_SIZE + CHECKSUM_SIZE + STX_AND_ETX_BYTES

#define DEBUG_SENSORS 0
#define DEBUG_TXRX 0
#define DEBUG_SINGLE_TXRX 1

#define STX 0x02
#define ETX 0x03

#define SERIAL_SEND_MS 150

// Enums
enum Progress_State {
  INITIALIZED_SENSORS,
  GO,
  RUNNING,
  NOGO
};

Progress_State currProg;

struct SensorInfo {
  float baroPressure;  // Changed from float to float
  float baroTemp;      // Changed from float to float
  float baroDepth;     // Changed from float to float
  float baroAltitude;  // Changed from float to float

  float imuOrientX;    // Changed from float to float
  float imuOrientY;    // Changed from float to float
  float imuOrientZ;    // Changed from float to float
  uint8_t imuTemp;
  uint8_t padding1[1];    // Adjust padding for alignment (optional)
  uint16_t checksum;      // Changed checksum to be 2 bytes (uint16_t)
}; // 4 x 7 + 1 + 1 = 32 bytes
SensorInfo sensor_info;

uint8_t sensorBuffer[SENSOR_BUFFER_SIZE];

// Setup, initialize
void setup() {
  Serial.begin(115200);
  memset(sensorBuffer, 0, SENSOR_BUFFER_SIZE);
  memset(&sensor_info, 0, SENSOR_BUFFER_SIZE);
  
  currProg = INITIALIZED_SENSORS;

  int baroStatus =  InitializeBarometer();
  int imuStatus =   InitializeIMU();

  byte readyMessage[] = {0xAA};
  byte confirmationMessage[] = {0xBB};
}

// Loop forever
void loop() {

#if DEBUG_TXRX
  if (Serial.available() >= SENSOR_BUFFER_SIZE) {
  Serial.readBytes(sensorBuffer, SENSOR_BUFFER_SIZE);
  memcpy(&sensor_info, sensorBuffer, SENSOR_BUFFER_SIZE);
  incrementSensorData();
  memcpy(sensorBuffer, &sensor_info, SENSOR_BUFFER_SIZE);
  Serial.write(sensorBuffer, SENSOR_BUFFER_SIZE);
  }
#elif DEBUG_SINGLE_TXRX
  sensor.read();
  
  // IMU get new sensor event
  sensors_event_t event; 
  bno.getEvent(&event);

  sensor_info.baroPressure = sensor.pressure();
  sensor_info.baroTemp = sensor.temperature();
  sensor_info.baroDepth = sensor.depth();
  sensor_info.baroAltitude = sensor.altitude();
  
  sensor_info.imuOrientX = event.orientation.x;
  sensor_info.imuOrientY = event.orientation.y;
  sensor_info.imuOrientZ = event.orientation.z;
  sensor_info.imuTemp = bno.getTemp();
  
  // bytes 0-27 = 28 bytes
  memcpy(&sensorBuffer[0], &sensor_info.baroPressure, sizeof(sensor_info.baroPressure));
  memcpy(&sensorBuffer[4], &sensor_info.baroTemp, sizeof(sensor_info.baroTemp));
  memcpy(&sensorBuffer[8], &sensor_info.baroDepth, sizeof(sensor_info.baroDepth));
  memcpy(&sensorBuffer[12], &sensor_info.baroAltitude, sizeof(sensor_info.baroAltitude));

  memcpy(&sensorBuffer[16], &sensor_info.imuOrientX, sizeof(sensor_info.imuOrientX));
  memcpy(&sensorBuffer[20], &sensor_info.imuOrientY, sizeof(sensor_info.imuOrientY));
  memcpy(&sensorBuffer[24], &sensor_info.imuOrientZ, sizeof(sensor_info.imuOrientZ));
  
  // bytes 28-29 = 1 byte
  memcpy(&sensorBuffer[28], &sensor_info.imuTemp, sizeof(sensor_info.imuTemp));

  //Serial.write((uint8_t*)&sensor_info.imuOrientX, sizeof(sensor_info.imuOrientX));

  Serial.write((uint8_t*)&sensorBuffer, SENSOR_BUFFER_SIZE);
  // Serial.println(sensor_info.imuOrientX);
#else
  // Update pressure and temperature readings
  sensor.read();
  
  // IMU get new sensor event
  sensors_event_t event; 
  bno.getEvent(&event);

  // Populate sensor info struct
  sensor_info.baroPressure = sensor.pressure();
  sensor_info.baroTemp = sensor.temperature();
  sensor_info.baroDepth = sensor.depth();
  sensor_info.baroAltitude = sensor.altitude();
  
  sensor_info.imuOrientX = event.orientation.x;
  sensor_info.imuOrientY = event.orientation.y;
  sensor_info.imuOrientZ = event.orientation.z;
  sensor_info.imuTemp = bno.getTemp();

  sensor_info.checksum = calculateChecksum((uint8_t*)&sensor_info, SENSOR_BUFFER_SIZE - 1); // Exclude the checksum byte

  memcpy(sensorBuffer, &sensor_info, SENSOR_BUFFER_SIZE);
  Serial.write(sensorBuffer, SENSOR_BUFFER_SIZE);
#endif 

#if DEBUG_SENSORS
  Serial.println("\n\n\n\n\n");
  Serial.print("Baro Pressure: "); Serial.println(sensor_info.baroPressure);
  Serial.print("Baro Temp: "); Serial.println(sensor_info.baroTemp);
  Serial.print("Baro Depth: "); Serial.println(sensor_info.baroDepth);
  Serial.print("Baro Altitude: "); Serial.println(sensor_info.baroAltitude);

  Serial.print("IMU Orientation X: "); Serial.println(sensor_info.imuOrientX);
  Serial.print("IMU Orientation Y: "); Serial.println(sensor_info.imuOrientY);
  Serial.print("IMU Orientation Z: "); Serial.println(sensor_info.imuOrientZ);
  Serial.print("IMU Temp: "); Serial.println(sensor_info.imuTemp);

  Serial.print("Checksum: "); Serial.println(sensor_info.checksum);
#endif

    //Serial.println(sizeof(sensor_info));

  delay(SERIAL_SEND_MS);
  // }
}

// Function to increment each field of the sensor_info struct by 1
void incrementSensorData() {
    sensor_info.baroAltitude += 1;
    sensor_info.baroPressure += 1;
    sensor_info.baroTemp += 1;
    sensor_info.baroDepth += 1;
    sensor_info.imuOrientX += 1;
    sensor_info.imuOrientY += 1;
    sensor_info.imuOrientZ += 1;
    sensor_info.imuTemp += 1;
}

int16_t calculateChecksum(uint8_t *data, size_t length) {
  int16_t sum = 0;
  for (size_t i = 0; i < length; i++) {
    sum += data[i];
  }
  return sum;
}

// Turns Barometer data collecting on
int InitializeBarometer() {
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
int InitializeIMU() {
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