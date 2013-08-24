#ifndef SETTINGSMANAGER_HPP
#define SETTINGSMANAGER_HPP

#include <QFile>
#include <QString>
#include <QSettings>

class SettingsManager
{
public:
	SettingsManager();

// -- getters --
	QString dbPath();

private:
	QSettings settings_;

// -- default values --
	static const QString DEF_DB_PATH;
};

#endif // SETTINGSMANAGER_HPP
