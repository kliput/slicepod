#include <QtCore>
#include <QtGui>

#include "core/utils.hpp"
#include "core/sqlexception.hpp"

#include <taglib/tag.h>
#include <taglib/fileref.h>

/**
 * @brief Checks if SQL error is present. If it is - throws SQLException
 *	with QSqlError object. Otherwise does nothing.
 * @param error QSqlError object to check if it contains error.
 * @param info QString with information about circumstances in which error
 *	occurs
 */
void check_error(const QSqlError& error, const char* info)
{
	if (error.isValid()) {
		throw SQLException(error, info);
	}
}

QString taglib_qstring(const TagLib::String& s)
{
	return QString::fromUtf8(s.toCString(true));
}

QString generate_episode_name(TagLib::Tag *tag)
{
	// TODO: "structurize" specific podcast handlers

	QString title;

	QRegExp icmpAlbumRegExp("Irish.*Celtic.*Music.*Podcast",
								   Qt::CaseInsensitive);

	QString album = taglib_qstring(tag->album());

	if (icmpAlbumRegExp.exactMatch(album)) {
		// hooray! it's Marc's Gunn Irish And Celtic Music Podcast!
		// try to simplify title (for 133 or older episodes)
		QRegExp icmpTitleRegExp(".*(#\\d.*)");

		QString tmpTitle = taglib_qstring(tag->title());

		if (icmpTitleRegExp.exactMatch(tmpTitle)) {
			title = icmpTitleRegExp.cap(1);
		} else {

		}
	} else {
		// try to get track number
		uint track = tag->track();
		if (track > 0) {
			title = QString("%1").arg(track);
		} else {
			title = taglib_qstring(tag->title());
		}
	}

	return title;
}
