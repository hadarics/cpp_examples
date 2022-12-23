TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

LIBS += -ldl

unix:!macx: LIBS += -L$$PWD/../dlib_dir/ -ldlib

INCLUDEPATH += $$PWD/../dlib_dir
DEPENDPATH += $$PWD/../dlib_dir

HEADERS += \
    square.hpp
