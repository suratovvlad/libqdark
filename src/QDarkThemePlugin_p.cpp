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
#include "SystemThemeHelper.h"

using namespace libqdark;

QDarkThemePlugin::QDarkThemePluginPrivate::QDarkThemePluginPrivate(QDarkThemePlugin* lib)
    : q_ptr(lib)
    , m_settingsManager{ new SettingsManager{} }
    , m_systemThemeHelper{ new SystemThemeHelper{} }
{
    auto fileLoader = [](const QString& filePath, QString* classField){
        // Read stylesheet from resources
        QFile file = { filePath };

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream{ &file };
            classField->append(stream.readAll());
            file.close();
        }
    };

    fileLoader(":qdarkstyle/style.qss", &m_darkThemeStyleSheet);
    fileLoader(":qdarkstyle_2_1/style_2.1.qss", &m_darkThemeStyleSheet_2_1);

    QObject::connect(m_systemThemeHelper.get(), &SystemThemeHelper::signalLightTheme, [this]{ _q_SetLightTheme(); });
    QObject::connect(m_systemThemeHelper.get(), &SystemThemeHelper::signalDarkTheme, [this]{ _q_SetDarkTheme(); });
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

    init();
}

void QDarkThemePlugin::QDarkThemePluginPrivate::initialize(QMainWindow *mainWindow)
{
    m_mainWindow = mainWindow;
    init();
}

void QDarkThemePlugin::QDarkThemePluginPrivate::_q_SetLightTheme()
{
    // Change theme to default style
    qApp->setStyleSheet(m_settingsManager->getStringValue(SettingsManager::SETTING_DEFAULT_STYLE));
}

void QDarkThemePlugin::QDarkThemePluginPrivate::_q_SetDarkTheme()
{
    // Change theme to dark
    qApp->setStyleSheet(m_darkThemeStyleSheet);
}

void QDarkThemePlugin::QDarkThemePluginPrivate::_q_SetSystemTheme()
{
    m_systemThemeHelper->setEnabled(true);
    // Update settings value
    m_settingsManager->setCurrentTheme(SettingsManager::CurrentTheme::System);
}

void QDarkThemePlugin::QDarkThemePluginPrivate::init()
{
    // Initialize additional menu with actions as child of QMainWindow
    m_darkThemeMenu = new QMenu{ QObject::tr("Dark Theme Plugin"), m_mainWindow };

    auto m_enableDarkThemeActionGroup = new QActionGroup{ m_mainWindow };

    m_enableDarkThemeActionGroup->setExclusive(true);

    {
        m_lightThemeAction = m_darkThemeMenu->addAction(QObject::tr("Light"));
        QObject::connect(m_lightThemeAction, &QAction::triggered,
            [this] {
                m_systemThemeHelper->setEnabled(false);
                _q_SetLightTheme();
                // Update settings value
                m_settingsManager->setCurrentTheme(SettingsManager::CurrentTheme::Light);
            }
        );
        m_lightThemeAction->setChecked(true);
        m_lightThemeAction->setCheckable(true);

        m_enableDarkThemeActionGroup->addAction(m_lightThemeAction);
    }

    {
        m_darkThemeAction = m_darkThemeMenu->addAction(QObject::tr("Dark"));
        QObject::connect(m_darkThemeAction, &QAction::triggered,
            [this] {
                m_systemThemeHelper->setEnabled(false);
                _q_SetDarkTheme();
                // Update settings value
                m_settingsManager->setCurrentTheme(SettingsManager::CurrentTheme::Dark);
            }
        );
        m_darkThemeAction->setCheckable(true);

        m_enableDarkThemeActionGroup->addAction(m_darkThemeAction);
    }

    {
        m_systemThemeAction = m_darkThemeMenu->addAction(QObject::tr("Use system setting"));
        QObject::connect(m_systemThemeAction, &QAction::triggered, [this]{ _q_SetSystemTheme(); });
        m_systemThemeAction->setCheckable(true);
        m_enableDarkThemeActionGroup->addAction(m_systemThemeAction);

#ifdef Q_OS_WIN
        m_systemThemeAction->setDisabled(false);
        m_systemThemeAction->setVisible(true);
#else
        m_systemThemeAction->setDisabled(true);
        m_systemThemeAction->setVisible(false);
#endif

    }
    m_darkThemeMenu->addSeparator();

    auto oldThemeCheckBox = m_darkThemeMenu->addAction(QObject::tr("Use old theme"));
    oldThemeCheckBox->setCheckable(true);
    oldThemeCheckBox->setChecked(false);
    QObject::connect(oldThemeCheckBox, &QAction::triggered,
        [this] {
            std::swap(m_darkThemeStyleSheet, m_darkThemeStyleSheet_2_1);
            switchToCurrentTheme();
            // Update settings value
            const auto useOldTheme = m_settingsManager->getBoolValue(SettingsManager::SETTING_USE_OLD_THEME);
            m_settingsManager->updateValue(SettingsManager::SETTING_USE_OLD_THEME, !useOldTheme);
        }
    );

    // Initialize action for about menu
    m_aboutAction = new QAction{ QObject::tr("About") };

    // Add this action to menu
    // According to https://doc.qt.io/qt-5/qwidget.html#addAction, the ownership of action is not
    // transferred to this QWidget. So, we have to handle this pointer
    m_darkThemeMenu->addAction(m_aboutAction);

    // Create about dialog as child of QMainWindow
    m_aboutDialog = new AboutDarkThemePluginDialog{ m_mainWindow };
    m_aboutDialog->setUpdatesEnabled(true);

    // Connect about action with the about dialog
    QObject::connect(m_aboutAction, &QAction::triggered, m_aboutDialog, &QDialog::show);

    // Add new menu to QMenuBar of the
    // TODO: who is an ownership?
    m_mainWindow->menuBar()->addMenu(m_darkThemeMenu);

    // First initialization by saved settings
    const auto isFirstStart = m_settingsManager->getBoolValue(SettingsManager::SETTING_FIRST_START);

    if (isFirstStart) {
        const auto defaultStyleSheet = m_mainWindow->styleSheet();
        m_settingsManager->updateValue(SettingsManager::SETTING_FIRST_START, false);
        m_settingsManager->updateValue(SettingsManager::SETTING_DEFAULT_STYLE, defaultStyleSheet);
#ifdef Q_OS_WIN
        m_settingsManager->setCurrentTheme(SettingsManager::CurrentTheme::System);
#else
        m_settingsManager->setCurrentTheme(SettingsManager::CurrentTheme::Dark);
#endif
        m_settingsManager->updateValue(SettingsManager::SETTING_USE_OLD_THEME, false);
    }

    const auto useOldTheme = m_settingsManager->getBoolValue(SettingsManager::SETTING_USE_OLD_THEME);
    oldThemeCheckBox->setChecked(useOldTheme);
//    oldThemeCheckBox->trigger();

    if (useOldTheme) {
        std::swap(m_darkThemeStyleSheet, m_darkThemeStyleSheet_2_1);
    }

    switchToCurrentTheme();
}

void QDarkThemePlugin::QDarkThemePluginPrivate::switchToCurrentTheme()
{
    const auto currentTheme = m_settingsManager->getCurrentTheme();

    const auto setterAction = [&m_darkThemeMenu = this->m_darkThemeMenu](QAction* action)
    {
         m_darkThemeMenu->setActiveAction(action);
         action->setChecked(true);
         action->trigger();
    };

    switch (currentTheme) {
        case SettingsManager::CurrentTheme::Light:
           setterAction(m_lightThemeAction);
            break;
        case SettingsManager::CurrentTheme::Dark:
            setterAction(m_darkThemeAction);
            break;
        case SettingsManager::CurrentTheme::System:
            [[fallthrough]];
        default:
#ifdef Q_OS_WIN
            setterAction(m_systemThemeAction);
#else
            setterAction(m_darkThemeAction);
#endif
            break;
    }
}


