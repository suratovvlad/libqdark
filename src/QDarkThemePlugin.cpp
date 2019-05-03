#include "QDarkThemePlugin.h"
#include "QDarkThemePlugin_p.h"
#include <iostream>

using namespace libqdark;

QDarkThemePlugin::QDarkThemePlugin()
    : d_ptr{new QDarkThemePluginPrivate{ this }}
{
}

QDarkThemePlugin::~QDarkThemePlugin() = default;

void QDarkThemePlugin::changeLinksColor()
{
    Q_D(QDarkThemePlugin);
    d->changeLinksColor();
}

void QDarkThemePlugin::initialize()
{
    Q_D(QDarkThemePlugin);
    d->initialize();
}

void QDarkThemePlugin::initialize(QMainWindow *mainWindow)
{
    d_ptr->initialize(mainWindow);
}
