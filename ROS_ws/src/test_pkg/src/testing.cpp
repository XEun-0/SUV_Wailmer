#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <unistd.h>
#include <stdio.h>

#include "serialib.h"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */
#define SENSOR_BUFFER_SIZE      32

struct SensorInfo {
  float baroPressure;
  float baroTemp;
  float baroDepth;
  float baroAltitude;

  float imuOrientX;
  float imuOrientY;
  float imuOrientZ;
  int8_t imuTemp;
  int8_t padding1;
}; // 4 x 7 + 1 + 1 = 32 bytes
SensorInfo sensor_info;

uint8_t sensorBuffer[SENSOR_BUFFER_SIZE];

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
  : Node("minimal_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    auto message = std_msgs::msg::String();
    message.data = "Hello, world! " + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  size_t count_;
};

#if defined (_WIN32) || defined(_WIN64)
    #define SERIAL_PORT "\\\\.\\COM5"
#endif
#if defined (__linux__) || defined(__APPLE__)
  #define SERIAL_PORT "/dev/ttyUSB0"
#endif

int main(int argc, char * argv[])
{
  serialib serial;
  char errorOpening = serial.openDevice(SERIAL_PORT, 9600);
    
  // If connection fails, return the error code otherwise, display a success message
  if (errorOpening!=1) return errorOpening;
    printf ("Successful connection to %s\n",SERIAL_PORT);
  
  while(true) {
    // Create the string
    char buffer[15] = "hello\n";

    // Write the string on the serial device
    //serial.writeString(buffer);
    //printf ("String sent: %s", buffer);
    //usleep(1000000);
    
    // Read the string
    // serial.readString(buffer, '\n', 14, 2000);
    // printf("String read: %s\n", buffer);

    serial.readBytes(sensorBuffer, 32, 2000);
    //printf("CTX: 0x%04X\n", sensorBuffer[0]);
    for(int i = 0; i < SENSOR_BUFFER_SIZE; i++) {
    	printf("%02X", sensorBuffer[i]);
    }
    printf("\n");
    
    printf("%ld\n", sizeof(sensor_info));
    
    memcpy(&sensor_info, sensorBuffer, sizeof(SensorInfo));
    printf("Testing: %f\n", sensor_info.imuOrientX);
    // Close the serial device
    //return 0 ;
    usleep(500000);
  }
  serial.closeDevice();
//   rclcpp::init(argc, argv);
//   rclcpp::spin(std::make_shared<MinimalPublisher>());
//   rclcpp::shutdown();
//   return 0;
}
