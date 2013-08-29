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

#include "directory.hpp"
#include "episode.hpp"

// TODO: parametrize
const char *Directory::schemaString()
{
	return R"(Directories (
				id integer primary key,
				path text not null,
				metadata text
			)
	)";
}

Directory::Directory(QSqlRecord record, DatabaseEngine *engine) :
	BaseRecord<Directory>(record, engine),
	path_(record.value(db::directory::PATH).toString()),
	metadata_(record.value(db::directory::METADATA).toString())
{
}

Directory::Directory(const QString &path, DatabaseEngine *engine) :
	BaseRecord<Directory>(engine),
	path_(path)
{
}

QList<QSharedPointer<Episode> > Directory::getEpisodesList() const
{
	return engine_->refs<Directory, Episode>(id());
}

QList<QVariant> Directory::valuesList() const
{
	return QList<QVariant>() << path_ << metadata_;
}

const QStringList &Directory::columnsList() const
{
	using namespace db::directory;
	static auto cl = QStringList({PATH, METADATA});

	return cl;
}
