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

#include "podcast.hpp"
#include "episode.hpp"

Podcast::Podcast(QSqlRecord record, DatabaseEngine *engine) :
	BaseRecord<Podcast>(record, engine),
	name_(record.value(db::podcast::NAME).toString()),
	metadata_(record.value(db::podcast::METADATA).toString())
{
}

Podcast::Podcast(const QString &name, DatabaseEngine *engine) :
	BaseRecord<Podcast>(engine),
	name_(name)
{
}

// TODO: parametrize
const char *Podcast::schemaString()
{
	return R"(Podcasts (
			id integer primary key,
			name text not null,
			metadata text
		)
	)";
}

QList<QSharedPointer<Episode> > Podcast::getEpisodesList() const
{
	return engine_->refs<Podcast, Episode>(id());
}

QList<QVariant> Podcast::valuesList() const
{
	return QList<QVariant>() << name_ << metadata_;
}

const QStringList &Podcast::columnsList() const
{
	using namespace db::podcast;
	static auto cl = QStringList({NAME, METADATA});
	return cl;
}
