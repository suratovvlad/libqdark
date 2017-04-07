include(../defaults.pri)

QT       += widgets gui core
CONFIG   += console

TEMPLATE = app

SOURCES += main.cpp \
    example.cpp

FORMS += ../src/QDarkStyleSheet/example/ui/example.ui

#LIBS += libadvapi32 libshell32 libuser32
LIBS += -L../src/debug -llibqdark

HEADERS += \
    example.h
