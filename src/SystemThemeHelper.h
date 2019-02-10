#pragma once

#include <memory>
#include <QObject>

namespace libqdark {

class SystemThemeHelper final : public QObject
{
    Q_OBJECT
public:
    explicit SystemThemeHelper(QObject* parent = nullptr);
    virtual ~SystemThemeHelper() override;

    void setEnabled(bool enabled);

signals:
    void signalLightTheme();
    void signalDarkTheme();

private:
    std::shared_ptr<QObject> m_themeNotifier;
};

} // namespace libqdark

