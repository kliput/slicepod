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
