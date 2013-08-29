#ifndef LIBRARYITEM_HPP
#define LIBRARYITEM_HPP

#include <QString>
#include <QStringList>
#include <QTime>
#include <QList>

#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

#include "db_engine/baserecord.hpp"

//#include "fragmentmarker.hpp"

class Fragment;

class LibraryItem
{
public:
	LibraryItem(const BaseRecord<Fragment>::ptr &fragment);

	const QString &podcastName() const;
	const QString &episodeName() const;
	const QString &fragmentTitle() const;
	const QString &fragmentArtist() const;
	int fragmentStartSec() const;
	int fragmentEndSec() const;
	int episodeLengthSec() const;
	QStringList fragmentTagsList() const;
	bool isStartFragment();

	BaseRecord<Fragment>::ptr_list episodeFragmentsList() const;

	BaseRecord<Fragment>::ptr getFragment() const;
	BaseRecord<Episode>::ptr getEpisode() const;

	const QTime &fragmentStartTime() const;
	const QTime &fragmentEndTime() const;
	const QTime &episodeLengthTime() const;

	bool hasEnd() const;

	TagLib::Tag *fileTags() const;
	QString fileFullPath() const;

//	QList<FragmentMarker>

protected:
	BaseRecord<Fragment>::ptr fragmentPtr_;

private:
	// NOTICE: these fields values should be updated
	// on model changes
	QTime startTime_;
	QTime endTime_;
	QTime episodeLengthTime_;

//	FragmentMarker marker_; // TODO

};

#endif // LIBRARYITEM_HPP
