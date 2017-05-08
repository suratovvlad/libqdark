#include "QDarkThemePlugin_p.h"

#include <QApplication>
#include <QFile>
#include <QPalette>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QPalette>
#include <QMessageBox>

#include "QDarkThemePlugin.h"
#include "SettingsManager.h"
#include "AboutDarkThemePluginDialog.h"

QDarkThemePluginPrivate::QDarkThemePluginPrivate(QDarkThemePlugin* lib)
    : q_ptr(lib)
	, m_settingsManager(new SettingsManager())
{
	// Read stylesheet from resources
	QFile file(":qdarkstyle/style.qss");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		m_darkThemeStyleSheet = file.readAll();
		file.close();
	}
}

void QDarkThemePluginPrivate::changeLinksColor()
{
	QPalette palette = qApp->palette();
	palette.setColor(QPalette::Link, QColor("#00bfff"));
	qApp->setPalette(palette);
}

#include <QDebug>
void QDarkThemePluginPrivate::initialize()
{
	// Find QMainWindow widget
	m_mainWindow = NULL;
    while (!m_mainWindow)
    {
        foreach (QWidget *widget, qApp->topLevelWidgets()) {
            if (widget->objectName() == "MainWindow")
                m_mainWindow = qobject_cast<QMainWindow*>(widget);
        }

        qApp->processEvents();
	}

	// Initialize additional menu with actions as child of QMainWindow
	m_darkThemeMenu = QSharedPointer<QMenu>(new QMenu(QObject::tr("Dark Theme Plugin"), m_mainWindow));
	// Initialize checkable action for changing theme
	m_enableDarkThemeAction = QSharedPointer<QAction>(new QAction(QObject::tr("Enable")));
	m_enableDarkThemeAction->setCheckable(true);
	// Connect this action to slot
	QObject::connect(m_enableDarkThemeAction.data(), &QAction::toggled, [this]{ _q_ToggleTheme(); });
	// Add this action to menu
	m_darkThemeMenu->addAction(m_enableDarkThemeAction.data());
	// Initialize action for about menu
	m_aboutAction = QSharedPointer<QAction>(new QAction(QObject::tr("About")));
	// Add this action to menu
	m_darkThemeMenu->addAction(m_aboutAction.data());
	// Create about dialog as child of QMainWindow
	m_aboutDialog = QSharedPointer<AboutDarkThemePluginDialog>(new AboutDarkThemePluginDialog(m_mainWindow));
	m_aboutDialog->setUpdatesEnabled(true);
	// Connect about action with the about dialog
	QObject::connect(m_aboutAction.data(), &QAction::triggered, m_aboutDialog.data(), &QDialog::show);
	// Add new menu to QMenuBar of the application
	m_mainWindow->menuBar()->addMenu(m_darkThemeMenu.data());

	// First initialization by saved settings
    bool isFirstStart = m_settingsManager->getBoolValue(SettingsManager::SETTING_FIRST_START);
    if (isFirstStart) {
        QString defaultStyleSheet = m_mainWindow->styleSheet();
        m_settingsManager->updateValue(SettingsManager::SETTING_FIRST_START, false);
        m_settingsManager->updateValue(SettingsManager::SETTING_DEFAULT_STYLE, defaultStyleSheet);
    } else {
        bool isDark = m_settingsManager->getBoolValue(SettingsManager::SETTING_DARK_THEME_ENABLED);
        if (isDark) {
			// Block signals to avoid second toggling
            m_enableDarkThemeAction->blockSignals(true);
			// Toggle the action state
            m_enableDarkThemeAction->toggle();
			// Change theme to dark
			m_mainWindow->setStyleSheet(m_darkThemeStyleSheet);
			// Unblock signals
			m_enableDarkThemeAction->blockSignals(false);
        }
    }

}

void QDarkThemePluginPrivate::_q_ToggleTheme()
{
	// Check current theme in settings manager
    bool isDark = m_settingsManager->getBoolValue(SettingsManager::SETTING_DARK_THEME_ENABLED);
    if (!isDark) {
		// Change theme to dark
        m_mainWindow->setStyleSheet(m_darkThemeStyleSheet);
		// Update settings value
		m_settingsManager->updateValue(SettingsManager::SETTING_DARK_THEME_ENABLED, true);
	} else {
		// Change theme to default style
        m_mainWindow->setStyleSheet(m_settingsManager->getStringValue(SettingsManager::SETTING_DEFAULT_STYLE));
		// Update settings value
		m_settingsManager->updateValue(SettingsManager::SETTING_DARK_THEME_ENABLED, false);
	}
}
