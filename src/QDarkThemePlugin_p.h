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
class SystemThemeHelper;
class AboutDarkThemePluginDialog;

class QDarkThemePlugin::QDarkThemePluginPrivate final
{
    Q_DECLARE_PUBLIC(QDarkThemePlugin)
    QDarkThemePlugin* const q_ptr;
    Q_PRIVATE_SLOT(d_func(), void _q_SetLightTheme())
    Q_PRIVATE_SLOT(d_func(), void _q_SetDarkTheme())
    Q_PRIVATE_SLOT(d_func(), void _q_SetSystemTheme())
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
    void initialize(QMainWindow* mainWindow);

    void _q_SetLightTheme();
    void _q_SetDarkTheme();
    void _q_SetSystemTheme();

private:
    void init();
    void switchToCurrentTheme();

    QMenu* m_darkThemeMenu;

    QAction* m_lightThemeAction;
    QAction* m_darkThemeAction;
    QAction* m_systemThemeAction;

    QAction* m_aboutAction;
    QString m_darkThemeStyleSheet;
    QString m_darkThemeStyleSheet_2_1;

    QMainWindow* m_mainWindow;
    QSharedPointer<libqdark::SettingsManager> m_settingsManager;
    libqdark::AboutDarkThemePluginDialog* m_aboutDialog;

    QSharedPointer<libqdark::SystemThemeHelper> m_systemThemeHelper;
};

} // namespace libqdark
