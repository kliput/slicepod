/****************************************************************************
* Slicepod
* Copyright (C) 2013 Jakub Liput <jakub.liput@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/ 

#include <QtCore>
#include <QtSql>

#include "databaseengine.hpp"

#include "directory.hpp"
#include "episode.hpp"
#include "fragment.hpp"
#include "fragmenttagmap.hpp"
#include "podcast.hpp"
#include "tag.hpp"
#include "playlist.hpp"
#include "fragmentplaylistmap.hpp"

template <class T>
QSqlError DatabaseEngine::fetchAll()
{
	// TODO: first check, if table exists?

	QSqlTableModel tableModel;

	tableModel.setTable(T::TABLE_NAME);

	if (!tableModel.select()) {
		qCritical("Cannot fetch rows from %s table, error: %s", T::TABLE_NAME,
				  qPrintable(tableModel.lastError().text()));
		return tableModel.lastError();
	} else {
		qDebug("Fetched %d rows from %s table.", tableModel.rowCount(), T::TABLE_NAME);

		auto& mp = map<T>();

		for (auto i = mp.begin(); i != mp.end(); ++i) {
			i.value()->stored_ = false;
		}
		mp.clear();

		for (int i=0; i < tableModel.rowCount(); ++i) {
			auto m = QSharedPointer<T>(new T(tableModel.record(i)));
			map<T>().insert(m->id(), m);
		}
		return QSqlError();
	}
}

bool DatabaseEngine::open(const QString& path)
{
	database_ = QSqlDatabase::addDatabase("QSQLITE");

	database_.setDatabaseName(path);
	database_.setHostName("localhost");
	database_.setUserName("root");
	database_.setPassword("");

	if (!database_.open())
	{
		qDebug("Error: could not connect to database: %s",
			   qPrintable(database_.lastError().text()));
		opened_ = false;
		return false;
	}

	{
		QSqlQuery pragmaForeign("PRAGMA foreign_keys = ON");

		if (!pragmaForeign.exec()) {
			qCritical("Cannot set PRAGMA foreign_keys to ON. Wrong relations will not "
				   "be detcted. SQL error: %s",
				   qPrintable(pragmaForeign.lastError().text()));
		}
	}

	{
		QSqlQuery pragmaSynchronous("PRAGMA synchronous = OFF");
		if (!pragmaSynchronous.exec()) {
			qWarning("Cannot set PRAGMA synchronous to OFF. Database operations will be slower but safer. "
				   "SQL error: %s",
				   qPrintable(pragmaSynchronous.lastError().text()));
		}
	}

	{
		QSqlQuery pragmaJournal("PRAGMA journal_mode = MEMORY");
		if (!pragmaJournal.exec()) {
			qWarning("Cannot set PRAGMA journal_mode to MEMORY. Database operations will be slower but safer. "
				   "SQL error: %s",
				   qPrintable(pragmaJournal.lastError().text()));
		}
	}

	return true;
}

DatabaseEngine::~DatabaseEngine()
{
	// TODO: maybe set all records stored_ to false?
}

void DatabaseEngine::fetchAllTables()
{
	// TODO: try/catch -> if cannot fetch try to recreate database or smthg
	fetchAll<Directory>();
	fetchAll<Podcast>();
	fetchAll<Episode>();
	fetchAll<Fragment>();
	fetchAll<FragmentTagMap>();
	fetchAll<Tag>();
	fetchAll<Playlist>();
	fetchAll<FragmentPlaylistMap>();
}

void DatabaseEngine::createAllTables()
{
	createTable<Directory>();
	createTable<Podcast>();
	createTable<Episode>();
	createTable<Fragment>();
	createTable<FragmentTagMap>();
	createTable<Tag>();
	createTable<Playlist>();
	createTable<FragmentPlaylistMap>();
}
