#include "QDarkThemePlugin_p.h"

#include <QApplication>
#include <QFile>
#include <QPalette>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QPalette>

#include "QDarkThemePlugin.h"
#include "SettingsManager.h"

QDarkThemePluginPrivate::QDarkThemePluginPrivate(QDarkThemePlugin* lib)
    : q_ptr(lib)
    , m_settingsManager(new SettingsManager())
{
    m_darkThemeMenu = QSharedPointer<QMenu>(new QMenu(QObject::tr("Dark Theme Plugin")));
    m_enableDarkThemeAction = QSharedPointer<QAction>(new QAction(QObject::tr("Enable")));

    m_enableDarkThemeAction->setCheckable(true);
    m_darkThemeMenu->addAction(m_enableDarkThemeAction.data());

    m_aboutAction = QSharedPointer<QAction>(new QAction(QObject::tr("About")));
    m_darkThemeMenu->addAction(m_aboutAction.data());

    QObject::connect(m_enableDarkThemeAction.data(), &QAction::toggled, [this]{ _q_ToggleTheme(); });

    QFile file(":qdarkstyle/style.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_darkThemeStyleSheet = file.readAll();
        file.close();
    }

    // TODO: Add if it needs
    //	QPalette palette = app.palette();
    //	palette.setColor(QPalette::Active, QPalette::Base, QColor(100,100,100));
    //	palette.setColor(QPalette::Link, QColor("#00bfff"));
    //	app.setPalette(palette);
}

void QDarkThemePluginPrivate::initialize() const
{
    m_mainWindow = NULL;

    while (!m_mainWindow)
    {
        foreach (QWidget *widget, qApp->topLevelWidgets()) {
            if (widget->objectName() == "MainWindow")
                m_mainWindow = qobject_cast<QMainWindow*>(widget);
        }

        qApp->processEvents();
    }

    m_mainWindow->menuBar()->addMenu(m_darkThemeMenu.data());

    bool isFirstStart = m_settingsManager->getBoolValue(SettingsManager::SETTING_FIRST_START);
    if (isFirstStart) {
        QString defaultStyleSheet = m_mainWindow->styleSheet();
        m_settingsManager->updateValue(SettingsManager::SETTING_FIRST_START, false);
        m_settingsManager->updateValue(SettingsManager::SETTING_DEFAULT_STYLE, defaultStyleSheet);
    } else {
        bool isDark = m_settingsManager->getBoolValue(SettingsManager::SETTING_DARK_THEME_ENABLED);
        if (isDark) {
            m_enableDarkThemeAction->blockSignals(true);
            m_enableDarkThemeAction->toggle();
            m_mainWindow->setStyleSheet(m_darkThemeStyleSheet);
            m_enableDarkThemeAction->blockSignals(false);
        }
    }
}

void QDarkThemePluginPrivate::_q_ToggleTheme()
{
    bool isDark = m_settingsManager->getBoolValue(SettingsManager::SETTING_DARK_THEME_ENABLED);
    if (!isDark) {
        m_mainWindow->setStyleSheet(m_darkThemeStyleSheet);
        m_settingsManager->updateValue(SettingsManager::SETTING_DARK_THEME_ENABLED, true);
    } else {
        m_mainWindow->setStyleSheet(m_settingsManager->getStringValue(SettingsManager::SETTING_DEFAULT_STYLE));
        m_settingsManager->updateValue(SettingsManager::SETTING_DARK_THEME_ENABLED, false);
    }
}
