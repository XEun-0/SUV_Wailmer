//#include <PID_v1.h>
#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_BNO055.h>
#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();
MS5837 sensor;

// #define SERIAL_DEBUG true
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
}; // 4 x 7 + 1 + 1 + 1 + 1 = 32 bytes
SensorInfo sensor_info;

void setup() {
  
 Serial.begin(9600);
//  
//  Serial.println("Starting");
//  Serial.println(F("\nI2C PINS"));
//  Serial.print(F("\tSDA = ")); Serial.println(SDA);
//  Serial.print(F("\tSCL = ")); Serial.println(SCL);

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

//   if SERIAL_DEBUG {
//
//   }



//  Serial.print("sizeof(SensorInfo)");
//  Serial.println(sizeof(SensorInfo));
//  Serial.println(sensor_info.baroPressure);
//  Serial.println(sizeof(sensor_info));
  
  // if we're waiting on the go msg from the main processor
  // wait and delay 1s until the go msg comes through
  // Should be continue sending readyMessage and reading if
  // anything was receieved, then if received, send a confirmation
  // msg and then start sending sensor readings
  
//
//  Serial.println("Start ============================");
//  Serial.print("Pressure: "); 
////  Serial.print(sensor.pressure()); 
////  Serial.println(" mbar");
//  Serial.println(sizeof(sensor.pressure()));
//  
//  Serial.print("Temperature: "); 
//  Serial.print(sensor.temperature()); 
//  Serial.println(" deg C");
//  
//  Serial.print("Depth: "); 
//  Serial.print(sensor.depth()); 
//  Serial.println(" m");
//  
//  Serial.print("Altitude: "); 
//  Serial.print(sensor.altitude()); 
//  Serial.println(" m above mean sea level");
//
//
//  
//  /* Display the floating point data */
//  Serial.print("X: ");
//  Serial.print(event.orientation.x, 4);
//  Serial.print("\tY: ");
//  Serial.print(event.orientation.y, 4);
//  Serial.print("\tZ: ");
//  Serial.print(event.orientation.z, 4);
//  Serial.println("");
//
//  /* Display the current temperature */
//  int8_t temp = bno.getTemp();
//  
//  Serial.print("Current Temperature: ");
//  Serial.print(temp);
//  Serial.println(" C");
//  Serial.println("");
//
//  Serial.println("End =============================");
  Serial.write((uint8_t *) &sensor_info, sizeof(SensorInfo));
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
