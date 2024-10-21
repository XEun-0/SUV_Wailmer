TEMPLATE = app
TARGET = ControlPanel
SOURCES += main.cpp mainLayout.cpp ../../../ROS_ws/src/Common/serialib.cpp
HEADERS += mainLayout.h ../../../ROS_ws/src/Common/serialib.h

# Add these lines to specify Qt modules
QT += core widgets
