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

#include "core/libraryinfo.hpp"

Fragment::Fragment(QSqlRecord record) :
	BaseRecord<Fragment>(record),
	episodeId_(record.value(db::fragment::EPISODE).toInt()),
	start_(record.value(db::fragment::START).toInt()),
	end_(record.value(db::fragment::END).toInt()),
	title_(record.value(db::fragment::TITLE).toString()),
	artist_(record.value(db::fragment::ARTIST).toString()),
	rating_(record.value(db::fragment::RATING).toInt()),
	metadata_(record.value(db::fragment::METADATA).toString())
{
}

Fragment::Fragment() :
	BaseRecord<Fragment>()
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

Fragment::~Fragment()
{
	delete libraryInfo_;
}

void Fragment::setEpisode(QSharedPointer<Episode> episode)
{
	if (episode) {
		fieldChange();
		episodeId_ = episode->id();
	}
}

const QString &Fragment::schemaString()
{
	using namespace db::fragment;
	static QString schema = QString("%1 ("
				"%2 integer primary key,"
				"%3 integer references %4(%5) on delete cascade on update cascade,"
				"%6 integer not null,"
				"%7 integer,"
				"%8 text,"
				"%9 text,"
				"%10 integer,"
				"%11 text)"
			).arg(TABLE_NAME,
				  db::ID,
				  EPISODE, db::episode::TABLE_NAME, db::ID,
				  START,
				  END,
				  TITLE).arg(
				ARTIST,
				RATING,
				METADATA);

	return schema;
}

QSharedPointer<Episode> Fragment::getEpisode() const
{
	return getEngine()->record<Episode>(episodeId_);
}

QList<QSharedPointer<Tag> > Fragment::getTagsList() const
{
	QList<QSharedPointer<FragmentTagMap>> ftl = getEngine()->list<FragmentTagMap>();

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
	return FragmentTagMap(this->id(), tag->id());
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

LibraryInfo* Fragment::getLibraryInfo()
{
	if (!libraryInfo_) {
		libraryInfo_ = new LibraryInfo(this);
	}

	return libraryInfo_;
}

void Fragment::fieldChange()
{
	BaseRecord<Fragment>::fieldChange();
	if (libraryInfo_) {
		libraryInfo_->update(this);
	}
}

bool Fragment::isStartFragment() const
{
	// return whether start_fragment in db contains something or null
	return getEpisode()->getStartFragment()->id() == id();
}
