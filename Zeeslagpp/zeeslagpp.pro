TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += ZMQ_STATIC
LIBS += -L A:\NP\zeeslag\lib -lzmq -lws2_32 -lIphlpapi
INCLUDEPATH += A:/NP/zeeslag/include

SOURCES += \
        main.cpp \
        ships.cpp

HEADERS += \
    ships.h \

