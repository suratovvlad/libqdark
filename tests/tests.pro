TEMPLATE = app

include(../defaults.pri)

CONFIG += console
CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

#LIBS += -L../src -lqdarklib
