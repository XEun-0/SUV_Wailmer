TEMPLATE = app
TARGET = ControlPanel
SOURCES += main.cpp mainLayout.cpp \
           ../../../Vehicle/controller/src/util.cpp \
           ../../../Vehicle/AppCommon/pPrintf.cpp \
           ../../../Vehicle/AppCommon/serialib.cpp
HEADERS += mainLayout.h \
           ../../../Vehicle/controller/include/Interface/ttcSohResp.h \
           ../../../Vehicle/controller/include/Common/util.h \
           ../../../Vehicle/AppCommon/pPrintf.h \
           ../../../Vehicle/AppCommon/serialib.h

INCLUDEPATH += . ../../../Vehicle/controller/include/ \
               ../../../Vehicle/AppCommon/

DEFINES += CONTROL_PANEL

# Add these lines to specify Qt modules
QT += core widgets
