#include "SystemThemeHelper.h"

using namespace libqdark;

#ifdef Q_OS_WIN
#include "Windows10ThemeNotifier.h"
#endif

SystemThemeHelper::SystemThemeHelper(QObject* parent)
    : QObject(parent)
    , m_themeNotifier(nullptr)
{
#ifdef Q_OS_WIN
    m_themeNotifier = std::make_shared<Windows10ThemeNotifier>();
    if (m_themeNotifier)
    {
        auto win10ThemeNotifier = std::dynamic_pointer_cast<Windows10ThemeNotifier>(m_themeNotifier);

        connect(win10ThemeNotifier.get(), &Windows10ThemeNotifier::signalLightTheme, this, &SystemThemeHelper::signalLightTheme);
        connect(win10ThemeNotifier.get(), &Windows10ThemeNotifier::signalDarkTheme, this, &SystemThemeHelper::signalDarkTheme);
    }
#endif
}

SystemThemeHelper::~SystemThemeHelper()
{
#ifdef Q_OS_WIN
    if (m_themeNotifier)
    {
        auto win10ThemeNotifier = std::dynamic_pointer_cast<Windows10ThemeNotifier>(m_themeNotifier);
        win10ThemeNotifier->close();
    }
#endif
}

void SystemThemeHelper::setEnabled(bool enabled)
{
#ifdef Q_OS_WIN
    if (m_themeNotifier)
    {
        auto win10ThemeNotifier = std::dynamic_pointer_cast<Windows10ThemeNotifier>(m_themeNotifier);

        win10ThemeNotifier->checkCurrentThemeAndNotify();

        if (enabled)
        {
            win10ThemeNotifier->open();
            win10ThemeNotifier->setEnabled(true);
        }
        else
        {
            win10ThemeNotifier->setEnabled(false);
            win10ThemeNotifier->close();
        }
    }
#endif
}
