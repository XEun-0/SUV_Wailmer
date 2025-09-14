#!/bin/bash

# Absolute path to the library directory
#LIBRARY_PATH="Adafruit_Unified_Sensor-1.1.14.zip \
#              Adafruit_BNO055-1.6.3.zip \
#              BlueRobotics_MS5837_Library-1.1.0.zip \
#              Adafruit_BusIO-1.16.1.zip"

# arduino-cli lib install Adafruit_BNO055-1.6.3.zip 
# arduino-cli config set library.enable_unsafe_install true
# arduino-cli config dump
# arduino-cli lib list

# Install the library
arduino-cli lib install FreeRTOS
arduino-cli lib install Servo
arduino-cli lib install --zip-path Adafruit_Unified_Sensor-1.1.14.zip
arduino-cli lib install --zip-path Adafruit_BNO055-1.6.3.zip
arduino-cli lib install --zip-path Adafruit_BusIO-1.16.1.zip
arduino-cli lib install --zip-path BlueRobotics_MS5837_Library-1.1.0.zip
