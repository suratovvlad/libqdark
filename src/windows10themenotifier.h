#pragma once

#include <atomic>
#include <memory>

#include <QObject>
#include <windef.h>

class QWinEventNotifier;

namespace libqdark {

class Windows10ThemeNotifier final: public QObject
{
    Q_OBJECT
public:
    explicit Windows10ThemeNotifier(QObject *parent = nullptr);

    virtual ~Windows10ThemeNotifier() override;

    void open();
    void close();
    void setEnabled(bool enabled);

    void checkCurrentThemeAndNotify();

public slots:
   void handler();

signals:
   void signalLightTheme();
   void signalDarkTheme();

private:
    std::unique_ptr<QWinEventNotifier> m_winEventNotifier;
    HANDLE m_eventHandle;
    HKEY   m_hKey;
    std::atomic_bool m_isOpened;
};

} // namespace libqdark
