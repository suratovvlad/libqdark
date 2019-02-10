#include "Windows10ThemeNotifier.h"

using namespace libqdark;

#include <string>
#include <QSettings>
#include <QWinEventNotifier>
//#include <QDebug>
#include <windows.h>

namespace
{
    using namespace std::string_literals;

    const auto mainRegKey = "HKEY_CURRENT_USER"s;
    const auto personalizationRegPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"s;
    const auto themeRegKey = "AppsUseLightTheme"s;
    const auto keyPath = mainRegKey + "\\"s + personalizationRegPath;

    std::wstring string_to_wstring(const std::string& text) {
        return std::wstring(text.begin(), text.end());
    }
}

Windows10ThemeNotifier::Windows10ThemeNotifier(QObject *parent)
    : QObject(parent)
    , m_winEventNotifier( new QWinEventNotifier{ parent } )
    , m_eventHandle(nullptr)
    , m_hKey(nullptr)
{
    connect(m_winEventNotifier.get(), &QWinEventNotifier::activated, this, &Windows10ThemeNotifier::handler);
    m_isOpened = false;
}

Windows10ThemeNotifier::~Windows10ThemeNotifier() = default;

void Windows10ThemeNotifier::open()
{
    if (m_isOpened)
    {
        return;
    }

    const DWORD dwFilter = REG_NOTIFY_CHANGE_NAME |
        REG_NOTIFY_CHANGE_ATTRIBUTES |
        REG_NOTIFY_CHANGE_LAST_SET |
        REG_NOTIFY_CHANGE_SECURITY;

    auto lErrorCode = RegOpenKeyEx(HKEY_CURRENT_USER, string_to_wstring(personalizationRegPath).c_str(), 0, KEY_NOTIFY, &m_hKey);
    if (lErrorCode != ERROR_SUCCESS)
    {
//        qDebug() << "Error in RegOpenKeyEx " << lErrorCode;
    }

    // Create an event.
    m_eventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_eventHandle == NULL)
    {
//        qDebug()<< "Error in CreateEvent " << GetLastError();
    }


    // Watch the registry key for a change of value.
    lErrorCode = RegNotifyChangeKeyValue(m_hKey,
                                       TRUE,
                                       dwFilter,
                                       m_eventHandle,
                                       TRUE);

    if (lErrorCode != ERROR_SUCCESS)
    {
//        qDebug()<< "Error in RegNotifyChangeKeyValue " << lErrorCode ;
    }

    // open the xl port and get the event handle
    m_winEventNotifier->setHandle(m_eventHandle);
    m_winEventNotifier->setEnabled(true);

    m_isOpened = true;
}

void Windows10ThemeNotifier::close()
{
    if (!m_isOpened)
    {
        return;
    }

    auto lErrorCode = RegCloseKey(m_hKey);
    if (lErrorCode != ERROR_SUCCESS)
    {
//        qDebug() << "Error in RegCloseKey.\n";
    }

    if (!CloseHandle(m_eventHandle))
    {
//        qDebug() << "Error in CloseHandle.\n";
    }


    m_isOpened = false;
}

void Windows10ThemeNotifier::setEnabled(bool enabled)
{
//    qDebug() << "\nsetEnabled.\n" << enabled;
    m_winEventNotifier->setEnabled(enabled);
}

void Windows10ThemeNotifier::checkCurrentThemeAndNotify()
{
    const auto appsUseLightTheme = QSettings{ keyPath.c_str(), QSettings::NativeFormat }.value(themeRegKey.c_str()).toBool();
//    qDebug() << appsUseLightTheme;

    if (appsUseLightTheme)
        emit signalLightTheme();
    else
        emit signalDarkTheme();
}

void Windows10ThemeNotifier::handler()
{
    if (m_isOpened && m_winEventNotifier->isEnabled())
    {
//        qDebug() << "\nChange has occurred.\n";
        checkCurrentThemeAndNotify();
    }
    else if (m_isOpened && !m_winEventNotifier->isEnabled())
    {
//        qDebug() << "\nCan't notify.\n";
    }
    else if (!m_isOpened)
    {
//        qDebug() << "\nClosed.\n";
    }

    close();

    if (m_winEventNotifier->isEnabled())
    {
        open();
    }
}
