//#include <PID_v1.h>
#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();
MS5837 sensor;

unsigned char buffer_RTT[4] = {0};
uint8_t CS;
#define COM 0x55
int Distance = 0;
int previousDistance = -1;  // Track previous distance to filter anomalies
int stableCount = 0;  // Track stability of the distance

#define SERIAL_DEBUG 0
struct SensorInfo {
  float baroPressure;
  float baroTemp;
  float baroDepth;
  float baroAltitude;

  float imuOrientX;
  float imuOrientY;
  float imuOrientZ;
  int8_t imuTemp;
  uint8_t padding1;
  uint8_t padding2;     
  uint8_t padding3;
  uint8_t distance;    
}; // 4 x 7 + 1 + 1 + 1 + 1 = 32 bytes
SensorInfo sensor_info;

void setup() {
  
  Serial.begin(115200);  // For communication with your computer
  Serial2.begin(115200); // Serial2 on Arduino Mega for sensor communication

#if SERIAL_DEBUG
  Serial.println("Starting");
  Serial.println(F("\nI2C PINS"));
  Serial.print(F("\tSDA = ")); Serial.println(SDA);
  Serial.print(F("\tSCL = ")); Serial.println(SCL);
#endif

  InitializeBarometer();
  InitializeIMU();

  byte readyMessage[] = {0xAA};
  byte confirmationMessage[] = {0xBB};
  
}

void loop() {
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

  sensor_info.distance = CheckDistance();

#if SERIAL_DEBUG
  Serial.print("sizeof(SensorInfo)");
  Serial.println(sizeof(SensorInfo));
  Serial.println(sensor_info.baroPressure);
  Serial.println(sizeof(sensor_info));
  
  // if we're waiting on the go msg from the main processor
  // wait and delay 1s until the go msg comes through
  // Should be continue sending readyMessage and reading if
  // anything was receieved, then if received, send a confirmation
  // msg and then start sending sensor readings
  
  Serial.println("Start ============================");
  Serial.print("Pressure: "); 
  Serial.print(sensor.pressure()); 
  Serial.println(" mbar");
  Serial.println(sizeof(sensor.pressure()));
  
  Serial.print("Temperature: "); 
  Serial.print(sensor.temperature()); 
  Serial.println(" deg C");
  
  Serial.print("Depth: "); 
  Serial.print(sensor.depth()); 
  Serial.println(" m");
  
  Serial.print("Altitude: "); 
  Serial.print(sensor.altitude()); 
  Serial.println(" m above mean sea level");

  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");

  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  
  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(euler.x());
  Serial.print(" Y: ");
  Serial.print(euler.y());
  Serial.print(" Z: ");
  Serial.print(euler.z());
  Serial.println("");

  /* Display the current temperature */
  int8_t temp = bno.getTemp();
  
  Serial.print("Current Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.println("");
  Serial.print("Distance: ");
  Serial.println(sensor_info.distance);
  Serial.println("End =============================");
#endif

#if SERIAL_DEBUG == 0
  Serial.write((uint8_t *) &sensor_info, sizeof(SensorInfo));
#endif

  delay(500);
}

//Turns Barometer data collecting on
void InitializeBarometer() {
  Wire.begin();       //Can use this if dealing with only 1 sensor
  //Wire.beginTransmission(0x76);
  while (!sensor.init()) {
    Serial.println("Initilization failed");
    delay(2000);
  }
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997);      //(air ~ 1.23, freshwater ~ 997, seawater ~1029)
}                                   //air is not accurate

//Turns IMU data collecting on
void InitializeIMU() {
  //Wire.begin();       //Can use this if dealing with only 1 sensor
  Wire.beginTransmission(0x28);
  if (!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("No BNO055 detected ... Check your wiring or I2C ADDR");
    while (1);
  }
  bno.setExtCrystalUse(true);
}

int CheckDistance() {
  // Clear any previous data in the Serial2 buffer
  while (Serial2.available()) {
    Serial2.read();
  }

  Serial2.write(COM);  // Send command to sensor

  // Alignment check: Wait until the first byte is 0xFF (the frame header)
  unsigned long startTime = millis();
  bool headerFound = false;

  while (true) {
    if (Serial2.available()) {
      if (Serial2.read() == 0xFF) {
        buffer_RTT[0] = 0xFF;  // Frame header found
        headerFound = true;
        break;  // Exit the loop, frame is aligned
      } else {
        //Serial.println("Incorrect byte, looking for 0xFF...");
      }
    }

    // Timeout handling to avoid waiting too long
    if (millis() - startTime > 100) {
      Serial.println("Timeout waiting for frame header");
      return;
    }
  }

  if (!headerFound) {
    return;
  }

  // Now that we have the frame header, read the remaining 3 bytes
  startTime = millis();
  while (Serial2.available() < 3) {  // Wait for the remaining bytes
    if (millis() - startTime > 100) {  // Timeout after 100ms
      Serial.println("Timeout waiting for remaining data");
      return;
    }
  }

  // Read the remaining 3 bytes (data and checksum)
  for (int i = 1; i < 4; i++) {
    buffer_RTT[i] = Serial2.read();
  }

  // Calculate checksum
  CS = buffer_RTT[0] + buffer_RTT[1] + buffer_RTT[2];

  if (buffer_RTT[3] == CS) {  // Verify checksum
    Distance = (buffer_RTT[1] << 8) + buffer_RTT[2];  // Calculate distance
    
    // Filtering: ignore 0mm distances and those out of sensor's range
    if (Distance == 0 || Distance < 30 || Distance > 6000) {
      //Serial.println("Invalid distance reading: out of range or zero.");
      return;
    }

    // Check if distance is stable
    if (Distance == previousDistance) {
      stableCount++;
    } else {
      stableCount = 0;  // Reset stability count if value changes
    }
    previousDistance = Distance;

    // Only print distance if stable for a few consecutive readings
    if (stableCount >= 2) {
      return Distance;
    }
  } else {

  }
  delay(50);  // Small delay to avoid overwhelming the sensor
}