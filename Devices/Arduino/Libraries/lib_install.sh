#!/bin/bash

# Absolute path to the library directory
LIBRARY_PATH="~/../workspace/ROS2-SW/Devices/Arduino/Libraries/Adafruit_BNO055"

# Install the library
arduino-cli lib install "$LIBRARY_PATH"

