TEMPLATE = app
TARGET = ControlPanel
SOURCES += main.cpp mainLayout.cpp ../../../ROS_ws/src/Common/serialib.cpp \
           ../../../Devices/Arduino/controller/src/util.cpp
HEADERS += mainLayout.h ../../../ROS_ws/src/Common/serialib.h \
           ../../../Devices/Arduino/controller/include/Interface/ttcSohResp.h \
           ../../../Devices/Arduino/controller/include/Common/util.h

INCLUDEPATH += ../../../Devices/Arduino/controller/include/

DEFINES += CONTROL_PANEL

# Add these lines to specify Qt modules
QT += core widgets
