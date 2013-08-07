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

	startTime_ = QTime().addSecs(fragmentStartSec());
	endTime_ = QTime().addSecs(fragmentStartSec());
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

bool LibraryItem::isPlaying() const
{
	return false;
}

TagLib::Tag *LibraryItem::fileTags() const
{
	return fragmentPtr_->episode->fileInfo().tag();
}

QString LibraryItem::fileFullPath() const
{
	return fragmentPtr_->episode->fullPath();
}

//void LibraryItem::play() const
//{
//	libvlc_instance_t *instance;
//	libvlc_media_player_t *media_player;
//	libvlc_media_t *media;

//	// -- load fragment data --
//	qx::dao::fetch_by_id_with_relation("episode_id->directory_id", *this);
//	qDebug() << "=== fname " << this->episode->fileName;
//	qDebug() << "=== dir_path " << this->episode->directory->path;

//	QString q_path = QString("%1/%2")
//			.arg(this->episode->directory->path)
//			.arg(this->episode->fileName);

//	qDebug() << "=== full " << q_path;

//	// 	const char* file_path = q_path.toStdString().c_str();

//	std::cout << "=== char* " << q_path.toStdString().c_str() << std::endl;

//	// load the vlc engine
//	instance = libvlc_new(0, NULL);

//	// create a new item
//	media = libvlc_media_new_path(instance, q_path.toStdString().c_str());

//	if (media == NULL) {
//		qDebug() << "VLC error!";
//	}

//	// create a media play playing environment
//	media_player = libvlc_media_player_new_from_media(media);

//	// no need to keep the media now
//	libvlc_media_release(media);

//	// play the media_player
//	libvlc_media_player_play(media_player);
//	libvlc_media_player_set_time(media_player, libvlc_time_t(this->start)*1000);

//	// sleep for length of the this (seconds)
//	sleep(this->end - this->start);

//	// stop playing
//	libvlc_media_player_stop(media_player);

//	// free the media_player
//	libvlc_media_player_release(media_player);

//	libvlc_release(instance);

//}

QStringList LibraryItem::fragmentTagsList() const
{
	QStringList slist;
	for (Tag::ptr t: fragmentPtr_->tagsList) {
		slist << t->name;
	}
	return slist;
}

EntityType<Fragment>::ptr LibraryItem::fragmentPtr() const
{
	return fragmentPtr_;
}

