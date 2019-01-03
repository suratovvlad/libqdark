#include "QDarkThemePlugin_p.h"

#include <QString>
#include <QTextStream>
#include <QApplication>
#include <QFile>
#include <QPalette>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QPalette>
#include <QMessageBox>
#include <iostream>

#include "SettingsManager.h"
#include "AboutDarkThemePluginDialog.h"

using namespace libqdark;

QDarkThemePlugin::QDarkThemePluginPrivate::QDarkThemePluginPrivate(QDarkThemePlugin* lib)
    : q_ptr(lib)
    , m_settingsManager{ new SettingsManager{} }
{
    // Read stylesheet from resources
    QFile file = {":qdarkstyle/style.qss"};

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream{ &file };
        m_darkThemeStyleSheet.append(stream.readAll());
        file.close();
    }
}

void QDarkThemePlugin::QDarkThemePluginPrivate::changeLinksColor()
{
    auto palette = qApp->palette();
    palette.setColor(QPalette::Link, QColor("#00bfff"));
    qApp->setPalette(palette);
}

void QDarkThemePlugin::QDarkThemePluginPrivate::initialize()
{
    // Find QMainWindow widget
    m_mainWindow = nullptr;

    while (!m_mainWindow)
    {
        auto widgets = qApp->topLevelWidgets();

        for (auto& widget : widgets) {
            if (widget->objectName() == "MainWindow") {
                m_mainWindow = qobject_cast<QMainWindow*>(widget);
            }
        }

        qApp->processEvents();
    }

    // Initialize additional menu with actions as child of QMainWindow
    m_darkThemeMenu = QSharedPointer<QMenu>(new QMenu{ QObject::tr("Dark Theme Plugin"), m_mainWindow });

    // Initialize checkable action for changing theme
    m_enableDarkThemeAction = QSharedPointer<QAction>(new QAction{ QObject::tr("Enable") });
    m_enableDarkThemeAction->setCheckable(true);

    // Connect this action to slot
    QObject::connect(m_enableDarkThemeAction.get(), &QAction::toggled, [this]{ _q_ToggleTheme(); });

    // Add this action to menu
    // According to https://doc.qt.io/qt-5/qwidget.html#addAction, the ownership of action is not
    // transferred to this QWidget. So, we have to handle this pointer
    m_darkThemeMenu->addAction(m_enableDarkThemeAction.get());

    // Initialize action for about menu
    m_aboutAction = QSharedPointer<QAction>(new QAction{ QObject::tr("About") });

    // Add this action to menu
    // According to https://doc.qt.io/qt-5/qwidget.html#addAction, the ownership of action is not
    // transferred to this QWidget. So, we have to handle this pointer
    m_darkThemeMenu->addAction(m_aboutAction.get());

    // Create about dialog as child of QMainWindow
    m_aboutDialog = QSharedPointer<AboutDarkThemePluginDialog>(new AboutDarkThemePluginDialog{ m_mainWindow });
    m_aboutDialog->setUpdatesEnabled(true);

    // Connect about action with the about dialog
    QObject::connect(m_aboutAction.get(), &QAction::triggered, m_aboutDialog.get(), &QDialog::show);

    // Add new menu to QMenuBar of the
    // TODO: who is an ownership?
    m_mainWindow->menuBar()->addMenu(m_darkThemeMenu.get());

    // First initialization by saved settings
    const auto isFirstStart = m_settingsManager->getBoolValue(SettingsManager::SETTING_FIRST_START);

    if (isFirstStart) {
        const auto defaultStyleSheet = m_mainWindow->styleSheet();
        m_settingsManager->updateValue(SettingsManager::SETTING_FIRST_START, false);
        m_settingsManager->updateValue(SettingsManager::SETTING_DEFAULT_STYLE, defaultStyleSheet);
    } else {
        const auto isDark = m_settingsManager->getBoolValue(SettingsManager::SETTING_DARK_THEME_ENABLED);
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

void QDarkThemePlugin::QDarkThemePluginPrivate::_q_ToggleTheme()
{
    // Check current theme in settings manager
    const auto isDark = m_settingsManager->getBoolValue(SettingsManager::SETTING_DARK_THEME_ENABLED);

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
