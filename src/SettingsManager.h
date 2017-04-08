#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSharedPointer>
#include <QSettings>

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    explicit SettingsManager(QObject* parent = NULL);

    QString getStringValue(const QString& key) const;
    bool getBoolValue(const QString& key) const;
    void updateValue(const QString& key, const QVariant& value) const;

    static const QString SETTING_DARK_THEME_ENABLED;
    static const QString SETTING_DEFAULT_STYLE;
    static const QString SETTING_FIRST_START;

private:
    bool fileExists(const QString& path);
    QSharedPointer<QSettings> m_settings;
};

#endif // SETTINGSMANAGER_H
