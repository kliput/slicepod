#include "settingsmanager.hpp"

SettingsManager::SettingsManager()
{
}

QString SettingsManager::dbPath()
{
	return settings_.value("db/path", DEF_DB_PATH).toString();
}

const QString SettingsManager::DEF_DB_PATH = "db.sqlite3";
