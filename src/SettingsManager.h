#pragma once

#include <memory>
#include <QObject>
#include <QSettings>

namespace libqdark {

class SettingsManager final: public QObject
{
    Q_OBJECT

public:
    enum class CurrentTheme
    {
        Light = 0,
        Dark,
        System
    };

    explicit SettingsManager(QObject* parent = nullptr);

    virtual ~SettingsManager() override;

    SettingsManager(const SettingsManager&) = delete;
    SettingsManager(SettingsManager&&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;
    SettingsManager& operator=(SettingsManager&&) = delete;

    QString getStringValue(const QString& key) const;
    bool getBoolValue(const QString& key) const;
    int getIntValue(const QString& key) const;
    void updateValue(const QString& key, const QVariant& value) const;

    CurrentTheme getCurrentTheme() const;
    void setCurrentTheme(const CurrentTheme& currentTheme);

    static const QString SETTING_CURRENT_THEME;
    static const QString SETTING_DEFAULT_STYLE;
    static const QString SETTING_FIRST_START;
    static const QString SETTING_USE_OLD_THEME;

private:
    bool fileExists(const QString& path);

    QSettings* m_settings;
};

} // namespace libqdark
