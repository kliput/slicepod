#ifndef LIBRARYITEM_HPP
#define LIBRARYITEM_HPP

#include <QString>
#include <QStringList>
#include <QTime>

#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

#include "../db_model/db_constants.hpp"

class Fragment;

class LibraryItem
{
public:
	LibraryItem(const EntityType<Fragment>::ptr &fragment);

	const QString &podcastName() const;
	const QString &episodeName() const;
	const QString &fragmentTitle() const;
	const QString &fragmentArtist() const;
	int fragmentStartSec() const;
	int fragmentEndSec() const;
	int episodeLengthSec() const;
	QStringList fragmentTagsList() const;

	EntityType<Fragment>::ptr fragmentPtr() const;

	const QTime &fragmentStartTime() const;
	const QTime &fragmentEndTime() const;
	const QTime &episodeLengthTime() const;

	bool hasEnd() const;
	bool isPlaying() const;

	TagLib::Tag *fileTags() const;
	QString fileFullPath() const;

protected:
	EntityType<Fragment>::ptr fragmentPtr_;

private:
	// NOTICE: these fields values should be updated
	// on model changes
	QTime startTime_;
	QTime endTime_;
	QTime episodeLengthTime_;

};

#endif // LIBRARYITEM_HPP
