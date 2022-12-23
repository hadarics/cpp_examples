TARGET = dlib
TEMPLATE = lib

CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    square.cpp

HEADERS += \
    square.hpp

unix {
    target.path = /usr/local/lib
    INSTALLS += target
}
