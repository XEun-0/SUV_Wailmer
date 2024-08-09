# Submarine Robotics Introduction
SUV - Small Underwater Vehicle. Codename WAILMER
# What does the robot look like?
(1) Robot Configuration: This robot has a configuration as follows. It has in total 6 thrusters with four (1-4) for leveling and depth control, and two (5-6) for horizontal motion control (forward, backward, turning). 
                  
                    //\\
                    \\// (5)
            //\\     ||     //\\
            \\// (1) ||     \\//  (2)
           ====================
            xxxx             xx
            xxxx             xx  
           ==================== 
            //\\ (3) ||     //\\  (4)
            \\//     ||     \\//
                    //\\
                    \\// (6)

(2) Sensors: Camera, IMU, Temperature sensor
(3) Mother Board - Jetson Nano (run the overall program)  
(4) Microcontroller - Arduino Mega (for thruster command execution), Arduno Nano (IMU, Barometer, Temperature aggregation)

---
## 2.5 TODO:
- interface with Arduino Nano for sensor aggregation
        - serial information
        - ~~~be able to compile and upload sketch to arduino nano~~~

---
## 2.5 Setup VNC
(TBD)