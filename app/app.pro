include(../defaults.pri)

QT       += widgets gui core
CONFIG   += console

TEMPLATE = app

SOURCES += main.cpp \
    example.cpp

FORMS += ../src/QDarkStyleSheet/example/ui/example.ui

HEADERS += \
    example.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -llibqdark
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -llibqdark
else:unix: LIBS += -L$$OUT_PWD/../src/ -llibqdark

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src
