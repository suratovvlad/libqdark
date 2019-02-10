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
    // Read stylesheet from resources
    QFile file = {":qdarkstyle/style.qss"};

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream{ &file };
        m_darkThemeStyleSheet.append(stream.readAll());
        file.close();
    }


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

    // Initialize additional menu with actions as child of QMainWindow
    m_darkThemeMenu = QSharedPointer<QMenu>(new QMenu{ QObject::tr("Dark Theme Plugin"), m_mainWindow });

//    m_darkThemeMenu->addSection(QObject::tr("Choose Theme"));

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

    // Initialize checkable action for changing theme
//    m_enableDarkThemeAction = QSharedPointer<QAction>(new QAction{ /*QObject::tr("Choose Theme")*/ });
//    m_enableDarkThemeAction->setCheckable(true);


//    auto lightTheme = new QAction{ QObject::tr("Light") };
//    auto darkTheme = new QAction{ QObject::tr("Dark") };
//    auto useWindowsTheme = new QAction{ QObject::tr("Use Windows theme") };


//    m_enableDarkThemeActionGroup->addAction(lightTheme);
//    m_enableDarkThemeActionGroup->addAction(darkTheme);
//    m_enableDarkThemeActionGroup->addAction(useWindowsTheme);
//    darkTheme->setChecked(true);

//    m_enableDarkThemeAction->setActionGroup(m_enableDarkThemeActionGroup);

    // Connect this action to slot
//    QObject::connect(m_enableDarkThemeAction.get(), &QAction::toggled, [this]{ _q_ToggleTheme(); });

    // Add this action to menu
    // According to https://doc.qt.io/qt-5/qwidget.html#addAction, the ownership of action is not
    // transferred to this QWidget. So, we have to handle this pointer
//    m_darkThemeMenu->addAction(m_enableDarkThemeAction.get());

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
#ifdef Q_OS_WIN
        m_settingsManager->setCurrentTheme(SettingsManager::CurrentTheme::System);
#else
        m_settingsManager->setCurrentTheme(SettingsManager::CurrentTheme::Dark);
#endif
    }

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

//#include <QDebug>

void QDarkThemePlugin::QDarkThemePluginPrivate::_q_SetLightTheme()
{
//    qDebug() << "_q_SetLightTheme";
    // Change theme to default style
    m_mainWindow->setStyleSheet(m_settingsManager->getStringValue(SettingsManager::SETTING_DEFAULT_STYLE));
}

void QDarkThemePlugin::QDarkThemePluginPrivate::_q_SetDarkTheme()
{
//    qDebug() << "_q_SetDarkTheme";

    // Change theme to dark
    m_mainWindow->setStyleSheet(m_darkThemeStyleSheet);
}

void QDarkThemePlugin::QDarkThemePluginPrivate::_q_SetSystemTheme()
{
//    qDebug() << "_q_SetWindowsTheme;";
    m_systemThemeHelper->setEnabled(true);
    // Update settings value
    m_settingsManager->setCurrentTheme(SettingsManager::CurrentTheme::System);
}


