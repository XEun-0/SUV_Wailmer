#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#include <Arduino_FreeRTOS.h>
#include <Servo.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();
MS5837 sensor;

// Motors
// Pins should 8-13
#define LEFT_THRUSTER_PIN         11
#define RIGHT_THRUSTER_PIN        10
#define FRONT_RIGHT_THRUSTER_PIN  5
#define FRONT_LEFT_THRUSTER_PIN   6
#define BACK_LEFT_THRUSTER_PIN    9
#define BACK_RIGHT_THRUSTER_PIN   7

#define MOTOR_STOP_COMMAND        1500

Servo left_thruster;
Servo right_thruster;
Servo front_right_thruster;
Servo front_left_thruster;
Servo back_left_thruster;
Servo back_right_thruster;

// Macros
#define SENSOR_BUFFER_SIZE      32
#define CHECKSUM_SIZE           2
#define STX_AND_ETX_BYTES       4
#define OUT_BUFFER_SIZE         SENSOR_BUFFER_SIZE + CHECKSUM_SIZE + STX_AND_ETX_BYTES

#define DEBUG_SENSORS           0
#define DEBUG_TXRX              0
#define DEBUG_SINGLE_TXRX       0

#define STX 0x02
#define ETX 0x03

#define SERIAL_SEND_MS 150

bool firstTime = true;

TaskHandle_t AggregateSensorsTaskHandle, MotorCommandsTaskHandle;

// Enums
enum Progress_State {
  INITIALIZED_SENSORS,
  GO,
  RUNNING,
  NOGO
};

Progress_State currProg;

struct SensorInfo {
  float baroPressure;   // 4
  float baroTemp;       // 4
  float baroDepth;      // 4
  float baroAltitude;   // 4

  float imuOrientX;     // 4
  float imuOrientY;     // 4
  float imuOrientZ;     // 4
  uint8_t imuTemp;      // 1
  uint8_t padding1[1];  // 1 byte. Current patch leaving at 1 padding byte just in
                        // checksum might need to be used. 
  uint16_t checksum;    // 2
}; // 4 x 7 + 1 + 2 = 32 bytes
SensorInfo sensor_info;

uint8_t sensorBuffer[SENSOR_BUFFER_SIZE];

void AggregateSensorsTask(void *pvParams) {
  while (1) {
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

    Serial.write((uint8_t*)&sensorBuffer, SENSOR_BUFFER_SIZE);

    vTaskDelay(SERIAL_SEND_MS / portTICK_PERIOD_MS);
  }
}

void MotorCommandsTask(void *pvParams) {
  while (1) {
    if (firstTime) {
      vTaskDelay(pdMS_TO_TICKS(7000));
    }
    
    //SlowMotorSpin();
    
    vTaskDelay(SERIAL_SEND_MS / portTICK_PERIOD_MS);
  }
}

// Setup, initialize
void setup() {
  Serial.begin(115200);
  memset(sensorBuffer, 0, SENSOR_BUFFER_SIZE);
  memset(&sensor_info, 0, SENSOR_BUFFER_SIZE);
  
  currProg = INITIALIZED_SENSORS;

  int baroStatus =      InitializeBarometer();
  int imuStatus =       InitializeIMU();
  //int controlsStatus =  InitializeControls();

  byte readyMessage[] = {0xAA};
  byte confirmationMessage[] = {0xBB};

  // Create Sensor Aggregation Task
  xTaskCreate(
    AggregateSensorsTask,    // Function to be called
    "AggregateSensorsTask", // Name of the task
    512,      // Stack size
    NULL,     // Parameters passed to task
    1,        // Task priority (higher number = higher priority)
    &AggregateSensorsTaskHandle // Task handle for reference
  );

  // Create Motor Commands Task
  xTaskCreate(
    MotorCommandsTask,    // Function to be called
    "MotorCommandsTask", // Name of the task
    256,      // Stack size
    NULL,     // Parameters passed to task
    2,        // Task priority
    &MotorCommandsTaskHandle // Task handle for reference
  );
  
  // Start the scheduler
  vTaskStartScheduler();
}

// RTOS removes loop functionality
void loop() {}

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

int InitializeControls() {
  left_thruster.attach(LEFT_THRUSTER_PIN);
  right_thruster.attach(RIGHT_THRUSTER_PIN);
  front_right_thruster.attach(FRONT_RIGHT_THRUSTER_PIN);
  front_left_thruster.attach(FRONT_LEFT_THRUSTER_PIN);
  back_left_thruster.attach(BACK_LEFT_THRUSTER_PIN);
  back_right_thruster.attach(BACK_RIGHT_THRUSTER_PIN);

  left_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
  right_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
  front_right_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
  front_left_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
  back_left_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);
  back_right_thruster.writeMicroseconds(MOTOR_STOP_COMMAND);

  return 0;
}

void SlowMotorSpin() {
  left_thruster.writeMicroseconds(1600);
  right_thruster.writeMicroseconds(1600);
  front_right_thruster.writeMicroseconds(1600);
  front_left_thruster.writeMicroseconds(1600);
  back_left_thruster.writeMicroseconds(1600);
  back_right_thruster.writeMicroseconds(1600);
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