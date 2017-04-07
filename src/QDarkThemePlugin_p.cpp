#include "QDarkThemePlugin_p.h"

#include <QApplication>
#include <QFile>
#include <QPalette>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMenuBar>

#include "QDarkThemePlugin.h"

QDarkThemePluginPrivate::QDarkThemePluginPrivate(QDarkThemePlugin* lib)
    : q_ptr(lib)
    , m_isFirst(true)
    , m_isDark(false)
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
}

void QDarkThemePluginPrivate::initialize() const
{
    m_mainWindow = NULL;

    while (!m_mainWindow)
    {
        foreach (QWidget *widget, QApplication::topLevelWidgets()) {
            if (widget->objectName() == "MainWindow")
                m_mainWindow = qobject_cast<QMainWindow*>(widget);
        }

        QApplication::processEvents();
    }

    m_mainWindow->menuBar()->addMenu(m_darkThemeMenu.data());
}

void QDarkThemePluginPrivate::_q_ToggleTheme()
{
    if (m_isFirst) {
        m_defaultStyleSheet = m_mainWindow->styleSheet();
        m_isFirst = false;
    }

    if (!m_isDark) {
        m_mainWindow->setStyleSheet(m_darkThemeStyleSheet);
        m_isDark = true;
    } else {
        m_mainWindow->setStyleSheet(m_defaultStyleSheet);
        m_isDark = false;
    }
}
