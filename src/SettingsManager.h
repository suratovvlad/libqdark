#pragma once

#include <memory>
#include <QObject>
#include <QSettings>

namespace libqdark {

class SettingsManager final: public QObject
{
    Q_OBJECT

public:
    explicit SettingsManager(QObject* parent = nullptr);

    virtual ~SettingsManager() override;

    SettingsManager(const SettingsManager&) = delete;
    SettingsManager(SettingsManager&&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;
    SettingsManager& operator=(SettingsManager&&) = delete;

    QString getStringValue(const QString& key) const;
    bool getBoolValue(const QString& key) const;
    void updateValue(const QString& key, const QVariant& value) const;

    static const QString SETTING_DARK_THEME_ENABLED;
    static const QString SETTING_DEFAULT_STYLE;
    static const QString SETTING_FIRST_START;

private:
    bool fileExists(const QString& path);

    std::unique_ptr<QSettings> m_settings;
};

} // namespace libqdark
