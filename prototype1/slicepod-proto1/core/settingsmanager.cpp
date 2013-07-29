#include "settingsmanager.hpp"

#include "../db_model/db_constants.hpp"

SettingsManager::SettingsManager()
{
}

QString SettingsManager::dbPath()
{
	return settings_.value("db/path", DEF_DB_PATH).toString();
}

const QString SettingsManager::DEF_DB_PATH = db::DB_PATH;
