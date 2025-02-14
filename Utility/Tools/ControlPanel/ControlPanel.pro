TEMPLATE = app
TARGET = ControlPanel
SOURCES += main.cpp mainLayout.cpp ../../../ROS_ws/src/Common/serialib.cpp \
           ../../../Vehicle/controller/src/util.cpp
HEADERS += mainLayout.h ../../../ROS_ws/src/Common/serialib.h \
           ../../../Vehicle/controller/include/Interface/ttcSohResp.h \
           ../../../Vehicle/controller/include/Common/util.h

INCLUDEPATH += ../../../Vehicle/controller/include/

DEFINES += CONTROL_PANEL

# Add these lines to specify Qt modules
QT += core widgets
