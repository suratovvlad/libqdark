#include "QDarkThemePlugin.h"
#include "QDarkThemePlugin_p.h"

QDarkThemePlugin::QDarkThemePlugin():
    d_ptr(new QDarkThemePluginPrivate(this))
{
}

QDarkThemePlugin::~QDarkThemePlugin()
{
}

void QDarkThemePlugin::initialize()
{
	Q_D(QDarkThemePlugin);
    d->initialize();
}
