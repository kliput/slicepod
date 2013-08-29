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

#include "fragment.hpp"
#include "fragmenttagmap.hpp"
#include "tag.hpp"

Fragment::Fragment(QSqlRecord record, DatabaseEngine *engine) :
	BaseRecord<Fragment>(record, engine),
	episodeId_(record.value(db::fragment::EPISODE).toInt()),
	start_(record.value(db::fragment::START).toInt()),
	end_(record.value(db::fragment::END).toInt()),
	title_(record.value(db::fragment::TITLE).toString()),
	artist_(record.value(db::fragment::ARTIST).toString()),
	rating_(record.value(db::fragment::RATING).toInt()),
	metadata_(record.value(db::fragment::METADATA).toString())
{
}

Fragment::Fragment(DatabaseEngine *engine) :
	BaseRecord<Fragment>(engine)
{
}

Fragment::Fragment(QSharedPointer<Episode> episode,
				   int start, const QString &title, const QString &artist,
				   int end) :
	BaseRecord<Fragment>(),
	// episode
	start_(start),
	end_(end),
	title_(title),
	artist_(artist)
{
	setEpisode(episode);
}

void Fragment::setEpisode(QSharedPointer<Episode> episode)
{
	if (episode) {
		fieldChange();
		episodeId_ = episode->id();
	}
}

// TODO: parametrize
const char *Fragment::schemaString()
{
	return R"(Fragments (
				id integer primary key,
				episode_id integer references Episodes(id) on delete cascade on update cascade,
				start integer not null,
				end integer,
				title text,
				artist text,
				rating integer,
				metadata text
			)
	)";
}

QSharedPointer<Episode> Fragment::getEpisode() const
{
	return engine_->record<Episode>(episodeId_);
}

QList<QSharedPointer<Tag> > Fragment::getTagsList() const
{
	QList<QSharedPointer<FragmentTagMap>> ftl = engine_->list<FragmentTagMap>();

	QList<QSharedPointer<Tag>> results;

	for (QSharedPointer<FragmentTagMap> ft: ftl) {
		if (ft->getFragment()->id() == id()) {
			results << ft->getTag();
		}
	}

	return results;
}

FragmentTagMap Fragment::bindTag(const QSharedPointer<Tag> &tag)
{
	return FragmentTagMap(this->id(), tag->id(), engine_);
}

QList<QVariant> Fragment::valuesList() const
{
	return QList<QVariant>() << episodeId_ << start_ << end_ << title_
							 << artist_ << rating_ << metadata_;
}

const QStringList &Fragment::columnsList() const
{
	using namespace db::fragment;
	static auto cl = QStringList({EPISODE, START, END, TITLE, ARTIST, RATING, METADATA});
	return cl;
}


bool Fragment::isStartFragment() const
{
	// return whether start_fragment in db contains something or null
	return getEpisode()->getStartFragment()->id() == id();
}
