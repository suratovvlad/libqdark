#ifndef QDARKTHEMEPLUGIN_P_H
#define QDARKTHEMEPLUGIN_P_H

#include <QSharedPointer>

class QMainWindow;
class QMenu;
class QAction;
class QDarkThemePlugin;
class SettingsManager;

class QDarkThemePluginPrivate
{
    Q_DISABLE_COPY(QDarkThemePluginPrivate)
    Q_DECLARE_PUBLIC(QDarkThemePlugin)
    QDarkThemePlugin* const q_ptr;
    Q_PRIVATE_SLOT(d_func(), void _q_ToggleTheme())

public:
    QDarkThemePluginPrivate(QDarkThemePlugin* lib);

    void initialize() const;

    void _q_ToggleTheme();
private:
    QSharedPointer<QMenu> m_darkThemeMenu;
    QSharedPointer<QAction> m_enableDarkThemeAction;
    QSharedPointer<QAction> m_aboutAction;
    QString m_darkThemeStyleSheet;

    mutable QMainWindow* m_mainWindow;
    QSharedPointer<SettingsManager> m_settingsManager;
};

#endif // QDARKTHEMEPLUGIN_P_H
