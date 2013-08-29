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

#include "libraryitem.hpp"
#include "core/sqlexception.hpp"
#include "utils.hpp"
#include "db_engine/fragment.hpp"

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
//	marker_ = FragmentMarker(this);

	startTime_ = QTime().addSecs(fragmentStartSec());
	endTime_ = QTime().addSecs(fragmentEndSec());
	episodeLengthTime_ = QTime().addSecs(episodeLengthSec());

	qDebug("init episode lenght time: %s", qPrintable(episodeLengthTime_.toString("HH:mm:ss")));
}

const QString &LibraryItem::podcastName() const
{
	return fragmentPtr_->getEpisode()->getPodcast()->getName();
}

const QString &LibraryItem::episodeName() const
{
	return fragmentPtr_->getEpisode()->getEpisodeName();
}

const QString &LibraryItem::fragmentTitle() const
{
	return fragmentPtr_->getTitle();
}

const QString &LibraryItem::fragmentArtist() const
{
	return fragmentPtr_->getArtist();
}

int LibraryItem::fragmentStartSec() const
{
	return fragmentPtr_->getStart();
}

int LibraryItem::fragmentEndSec() const
{
	return fragmentPtr_->getEnd();
}

int LibraryItem::episodeLengthSec() const
{
	return fragmentPtr_->getEpisode()->audioLength(); // TODO check if it's fetched?
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
	return fragmentPtr_->getEnd() >= 0;
}

TagLib::Tag *LibraryItem::fileTags() const
{
	return fragmentPtr_->getEpisode()->fileInfo().tag();
}

QString LibraryItem::fileFullPath() const
{
	return fragmentPtr_->getEpisode()->getFullPath();
}

QStringList LibraryItem::fragmentTagsList() const
{
	QStringList slist;
	for (Tag::ptr t: fragmentPtr_->getTagsList()) {
		slist << t->getName();
	}
	return slist;
}

bool LibraryItem::isStartFragment()
{
	return fragmentPtr_->isStartFragment();
}

BaseRecord<Fragment>::ptr LibraryItem::getFragment() const
{
	return fragmentPtr_;
}

BaseRecord<Episode>::ptr LibraryItem::getEpisode() const
{
	fragmentPtr_->getEpisode();
}

BaseRecord<Fragment>::ptr_list LibraryItem::episodeFragmentsList() const
{
	return fragmentPtr_->getEpisode()->getFragmentsList();
}

