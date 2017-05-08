#ifndef QDARKTHEMEPLUGIN_P_H
#define QDARKTHEMEPLUGIN_P_H

#include <QSharedPointer>

class QMainWindow;
class QMenu;
class QAction;
class QDarkThemePlugin;
class SettingsManager;
class AboutDialog;

class QDarkThemePluginPrivate
{
    Q_DISABLE_COPY(QDarkThemePluginPrivate)
    Q_DECLARE_PUBLIC(QDarkThemePlugin)
    QDarkThemePlugin* const q_ptr;
	Q_PRIVATE_SLOT(d_func(), void _q_ToggleTheme())
	Q_PRIVATE_SLOT(d_func(), void _q_paletteChanged(QPalette))
public:
    QDarkThemePluginPrivate(QDarkThemePlugin* lib);

	void changeLinksColor();
	void initialize();
	void _q_ToggleTheme();
private:
    QSharedPointer<QMenu> m_darkThemeMenu;
    QSharedPointer<QAction> m_enableDarkThemeAction;
    QSharedPointer<QAction> m_aboutAction;
    QString m_darkThemeStyleSheet;

	QMainWindow* m_mainWindow;
    QSharedPointer<SettingsManager> m_settingsManager;
	QSharedPointer<AboutDialog> m_aboutDialog;
};

#endif // QDARKTHEMEPLUGIN_P_H
