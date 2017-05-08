include(../defaults.pri)
include(version.pri)

QT       += widgets gui

TARGET = libqdark
TEMPLATE = lib

DEFINES += LIBQDARK_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    QDarkThemePlugin.cpp \
    QDarkThemePlugin_p.cpp \
    SettingsManager.cpp \
    AboutDarkThemePluginDialog.cpp

HEADERS += \
    libqdark_global.h \
    QDarkThemePlugin.h \
    QDarkThemePlugin_p.h \
    SettingsManager.h \
    AboutDarkThemePluginDialog.h

RESOURCES += QDarkStyleSheet/qdarkstyle/style.qrc

unix:!macx {
    isEmpty( PREFIX ):PREFIX = /usr
    DEFINES += PREFIX=\\\"$${PREFIX}\\\"
    target.path = $${PREFIX}/lib
    includes.path = $${PREFIX}/include/libqdark
    includes.files = libqdark_global.h \
                     QDarkThemePlugin.h
    INSTALLS += target \
                includes
}

DISTFILES += \
    version.pri

FORMS += \
    AboutDarkThemePluginDialog.ui
