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
    AboutDarkThemePluginDialog.cpp \
    SystemThemeHelper.cpp

HEADERS += \
    libqdark_global.h \
    QDarkThemePlugin.h \
    QDarkThemePlugin_p.h \
    SettingsManager.h \
    AboutDarkThemePluginDialog.h \
    SystemThemeHelper.h

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

win32 {
    #Skip version tagging on windows
    #https://stackoverflow.com/a/34470370
    #https://stackoverflow.com/q/404774
    CONFIG += skip_target_version_ext

    SOURCES += \
        Windows10ThemeNotifier.cpp

    HEADERS += \
        Windows10ThemeNotifier.h

    # Taken from https://github.com/pvanhoof/dir-examples/blob/master/qmake-example/src/libs/qmake-example/qmake-example.pro
    # See also https://stackoverflow.com/questions/6823862/qmake-creating-a-pc-file
    # See also https://stackoverflow.com/questions/6794710/qt-qmake-generating-pkgconfig-for-a-project

    ## This is just because qmake ..

    isEmpty(PREFIX) {
            PREFIX = /usr
    }

    DEFINES += PREFIX=\\\"$${PREFIX}\\\"

    target.path = $${PREFIX}/lib

    ## We will install the headers in a API specific include path
    includes.path = $${PREFIX}/include/$${TARGET}

    ## Here we put only the publicly installed headers
    includes.files = libqdark_global.h \
                     QDarkThemePlugin.h

    ## These will create a pkg-config .pc file for us
    CONFIG += create_pc create_prl no_install_prl

    # This makes sure that the pkg-config file is qmake-example-4.3.pc
    QMAKE_PKGCONFIG_FILE = $${TARGET}

    # This fills in the Name property
    QMAKE_PKGCONFIG_NAME = $${TARGET}

    # This fills in the Description property
    QMAKE_PKGCONFIG_DESCRIPTION = Plugin for Qt applications. Choose the dark side of the style.

    ## Here we will install the library to. If somebody has a better or
    ## more standardized way, let me know (I don't like this very much)
    QMAKE_PKGCONFIG_LIBDIR = $$target.path

    # This is where our API specific headers are
    QMAKE_PKGCONFIG_INCDIR = $$includes.path
    QMAKE_PKGCONFIG_DESTDIR = pkgconfig
    QMAKE_PKGCONFIG_PREFIX = $${PREFIX}

    # Usually people take the semver version here
    QMAKE_PKGCONFIG_VERSION = $$PROJECT_VERSION

    ## Installation targets (the pkg-config seems to install automatically)
    INSTALLS += target \
                includes

}

DISTFILES += \
    version.pri

FORMS += \
    AboutDarkThemePluginDialog.ui
