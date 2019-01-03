#pragma once

#include <QtCore/qobjectdefs.h>
#include <QString>
#include <QSharedPointer>

#include "QDarkThemePlugin.h"

class QMainWindow;
class QMenu;
class QAction;

namespace libqdark {

class QDarkThemePlugin;
class SettingsManager;
class AboutDarkThemePluginDialog;

class QDarkThemePlugin::QDarkThemePluginPrivate final
{
    Q_DECLARE_PUBLIC(QDarkThemePlugin)
    QDarkThemePlugin* const q_ptr;
    Q_PRIVATE_SLOT(d_func(), void _q_ToggleTheme())
    Q_PRIVATE_SLOT(d_func(), void _q_paletteChanged(QPalette))
public:
    QDarkThemePluginPrivate(QDarkThemePlugin* lib);

    ~QDarkThemePluginPrivate() = default;

    QDarkThemePluginPrivate(const QDarkThemePluginPrivate&) = delete;
    QDarkThemePluginPrivate(QDarkThemePluginPrivate&&) = delete;
    QDarkThemePluginPrivate& operator=(const QDarkThemePluginPrivate&) = delete;
    QDarkThemePluginPrivate& operator=(QDarkThemePluginPrivate&&) = delete;


    void changeLinksColor();
    void initialize();
    void _q_ToggleTheme();

private:
    QSharedPointer<QMenu> m_darkThemeMenu;
    QSharedPointer<QAction> m_enableDarkThemeAction;
    QSharedPointer<QAction> m_aboutAction;
    QString m_darkThemeStyleSheet;

    QMainWindow* m_mainWindow;
    QSharedPointer<libqdark::SettingsManager> m_settingsManager;
    QSharedPointer<libqdark::AboutDarkThemePluginDialog> m_aboutDialog;
};

} // namespace libqdark
