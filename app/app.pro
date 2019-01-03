include(../defaults.pri)

QT       += widgets gui core
CONFIG   += console

TEMPLATE = app

SOURCES += main.cpp \
    example.cpp

FORMS += \
    ../src/QDarkStyleSheet/example/ui/mw_menus.ui \
    ../src/QDarkStyleSheet/example/ui/dw_buttons.ui \
    ../src/QDarkStyleSheet/example/ui/dw_containers_no_tabs.ui \
    ../src/QDarkStyleSheet/example/ui/dw_containers_tabs.ui \
    ../src/QDarkStyleSheet/example/ui/dw_displays.ui \
    ../src/QDarkStyleSheet/example/ui/dw_inputs_fields.ui \
    ../src/QDarkStyleSheet/example/ui/dw_inputs_no_fields.ui \
    ../src/QDarkStyleSheet/example/ui/dw_views.ui \
    ../src/QDarkStyleSheet/example/ui/dw_widgets.ui

HEADERS += \
    customdockwidget.h \
    example.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -llibqdark
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -llibqdark
else:unix: LIBS += -L$$OUT_PWD/../src/ -llibqdark

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src
