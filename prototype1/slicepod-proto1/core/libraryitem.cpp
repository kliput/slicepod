#include "../precompiled.hpp"

#include <QtCore>

#include "libraryitem.hpp"
#include "../db_model/db_constants.hpp"
#include "../db_model/entitytype.hpp"
#include "../db_model/tag.hpp"
#include "../db_model/fragment.hpp"
#include "../core/sqlexception.hpp"
#include "utils.hpp"

#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

/**
 * @brief LibraryItem::LibraryItem
 * @param fragment
 * @throw SQLException on database access error
 */
LibraryItem::LibraryItem(const Fragment::ptr &fragment)
	:
	  fragmentPtr_(fragment)
{
	// -- complete data from database if needed

	QStringList relations;

	if (fragment->episode.isNull() || fragment->episode->podcast.isNull()) {
		relations << db::field::fragment::EPISODE;
		relations << QString("%1->%2").arg(db::field::fragment::EPISODE,
										   db::field::episode::PODCAST);
	}

	if (fragment->tagsList.isEmpty()) {
		relations << db::field::fragment::TAGS_LIST;
	}

	if (!relations.isEmpty()) {
		check_error(qx::dao::fetch_by_id_with_relation(relations, fragmentPtr_));
	}

	// ---

//	marker_ = FragmentMarker(this);

	startTime_ = QTime().addSecs(fragmentStartSec());
	endTime_ = QTime().addSecs(fragmentEndSec());
	episodeLengthTime_ = QTime().addSecs(episodeLengthSec());

	qDebug("init episode lenght time: %s", qPrintable(episodeLengthTime_.toString("HH:mm:ss")));
}

const QString &LibraryItem::podcastName() const
{
	return fragmentPtr_->episode->podcast->name;
}

const QString &LibraryItem::episodeName() const
{
	return fragmentPtr_->episode->episodeName;
}

const QString &LibraryItem::fragmentTitle() const
{
	return fragmentPtr_->title;
}

const QString &LibraryItem::fragmentArtist() const
{
	return fragmentPtr_->artist;
}

int LibraryItem::fragmentStartSec() const
{
	return fragmentPtr_->start;
}

int LibraryItem::fragmentEndSec() const
{
	return fragmentPtr_->end;
}

int LibraryItem::episodeLengthSec() const
{
	return fragmentPtr_->episode->audioLength(); // TODO check if it's fetched?
}

const QTime& LibraryItem::fragmentStartTime() const
{
	return startTime_;
}

const QTime& LibraryItem::fragmentEndTime() const
{
	return endTime_;
}

const QTime& LibraryItem::episodeLengthTime() const
{
	return episodeLengthTime_;
}

bool LibraryItem::hasEnd() const
{
	return fragmentPtr_->end >= 0;
}

TagLib::Tag *LibraryItem::fileTags() const
{
	return fragmentPtr_->episode->fileInfo().tag();
}

QString LibraryItem::fileFullPath() const
{
	return fragmentPtr_->episode->fullPath();
}

QStringList LibraryItem::fragmentTagsList() const
{
	QStringList slist;
	for (Tag::ptr t: fragmentPtr_->tagsList) {
		slist << t->name;
	}
	return slist;
}

bool LibraryItem::isStartFragment()
{
	return fragmentPtr_->isStartFragment();
}

EntityType<Fragment>::ptr LibraryItem::fragmentPtr() const
{
	return fragmentPtr_;
}

