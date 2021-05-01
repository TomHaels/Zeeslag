TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -LA:/NP/zeeslag/Zeeslag/lib -lzmq -lws2_32 -lIphlpapi
INCLUDEPATH += A:/NP/zeeslag/Zeeslag/include

SOURCES += \
        main.cpp \
        ships.cpp

HEADERS += \
    ships.h \
    zmq.hpp \
    zmq_addon.hpp
