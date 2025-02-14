#ifndef TASKGLOBALS_H
#define TASKGLOBALS_H

//========================================================
//=========== Delays =====================================
//========================================================
#define TTC_TASK_DELAY                  pdMS_TO_TICKS(20)
#define MAIN_CONTROLLER_TASK_DELAY      pdMS_TO_TICKS(100)

#define TEN_SECONDS_DELAY               pdMS_TO_TICKS(10000) 
#define FIVE_SECONDS_DELAY              pdMS_TO_TICKS(5000) 
#define SYS_BAUD_RATE                   115200
#define TTC_SENSOR_MUTEX                20
#define TTC_THRUSTER_MUTEX              15

//========================================================
//=========== Misc =======================================
//========================================================
#define STX 0x02
#define ETX 0x03

#define SERIAL_SEND_MS          150
#define SENSORS_TASK_MS         150

#define SET_STATUS_BYTE         1
#define SENSOR_BUFFER_SIZE      32
#define THRUSTER_BUFFER_SIZE    16
#define OUT_BUFFER_SIZE         SENSOR_BUFFER_SIZE + THRUSTER_BUFFER_SIZE

//========================================================
//=========== Thrusters ==================================
//========================================================
// Pins should 8-13
#define LEFT_THRUSTER_PIN               8         // 8
#define RIGHT_THRUSTER_PIN              13        // 13
#define FRONT_RIGHT_THRUSTER_PIN        12        // 12
#define FRONT_LEFT_THRUSTER_PIN         10        // 10
#define BACK_LEFT_THRUSTER_PIN          9         // 9
#define BACK_RIGHT_THRUSTER_PIN         11        // 11

// Motor command macros
#define MOTOR_STOP_COMMAND              1500
#define SLOW_SPEED_UP                   1600
#define SLOW_SPEED_DOWN                 1400

#define THRUSTER_COMMAND_BUFFER_SIZE    32 //temp size may change
#define THRUSTER_INFO_SIZE              16
#define THRUSTER_READY_MS               7000
#define SERIAL_SEND_MS                  150
#define THRUSTER_TASK_MS                100

//========================================================
//=========== Debug ======================================
//========================================================
#define DEBUG_SENSORS           0
#define DEBUG_TXRX              0
#define DEBUG_SINGLE_TXRX       0
#define DEBUG_SERIAL            0

#endif // TASKGLOBALS_H