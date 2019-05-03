#include "SettingsManager.h"
#include <QStandardPaths>
#include <QDir>

using namespace libqdark;

const QString SettingsManager::SETTING_CURRENT_THEME = "CurrentTheme";
const QString SettingsManager::SETTING_DEFAULT_STYLE = "DefaultStyle";
const QString SettingsManager::SETTING_FIRST_START = "FirstAppStart";

SettingsManager::SettingsManager(QObject* parent)
    : QObject(parent)
{
    auto userPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    userPath += QDir::separator();
    userPath += "qdarktheme.conf";
    userPath = QDir::toNativeSeparators(userPath);
    m_settings = new QSettings{ userPath, QSettings::IniFormat };

    if (!fileExists(userPath)) {
        setCurrentTheme(CurrentTheme::Light);
        updateValue(SETTING_DEFAULT_STYLE, QString(""));
        updateValue(SETTING_FIRST_START, true);
    } else {
        getIntValue(SETTING_CURRENT_THEME);
        getStringValue(SETTING_DEFAULT_STYLE);
        getBoolValue(SETTING_FIRST_START);
    }
}

SettingsManager::~SettingsManager() {
    m_settings->sync();
    m_settings->deleteLater();
};

QString SettingsManager::getStringValue(const QString& key) const
{
    return m_settings->value(key).toString();
}

bool SettingsManager::getBoolValue(const QString &key) const
{
    return m_settings->value(key).toBool();
}

int SettingsManager::getIntValue(const QString &key) const
{
   return m_settings->value(key).toInt();
}

void SettingsManager::updateValue(const QString& key, const QVariant& value) const
{
    m_settings->setValue(key, value);
    m_settings->sync();
}

SettingsManager::CurrentTheme SettingsManager::getCurrentTheme() const
{
    return static_cast<CurrentTheme>(getIntValue(SETTING_CURRENT_THEME));
}

void SettingsManager::setCurrentTheme(const SettingsManager::CurrentTheme &currentTheme)
{
    updateValue(SETTING_CURRENT_THEME, static_cast<int>(currentTheme));
}

bool SettingsManager::fileExists(const QString& path)
{
    auto check_file = QFileInfo{path};
    // check if file exists and if yes: Is it really a file and no directory?
    return check_file.exists() && check_file.isFile();
}
