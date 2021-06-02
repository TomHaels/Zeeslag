TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += ZMQ_STATIC
LIBS += -L$$PWD/../lib -lzmq -lws2_32 -lIphlpapi
INCLUDEPATH += $$PWD/../include
#LIBS += -L A:/NP/zeeslag/Zeeslag/lib -lzmq -lws2_32 -lIphlpapi
#INCLUDEPATH += A:/NP/zeeslag/Zeeslag/include


SOURCES += \
        main.cpp

HEADERS += \

