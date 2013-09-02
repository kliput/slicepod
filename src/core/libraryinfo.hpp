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

#ifndef LIBRARYITEM_HPP
#define LIBRARYITEM_HPP

#include <QTime>
#include <QString>

#include "db_engine/fragment.hpp"

class Fragment;

/**
 * @brief The LibraryInfo class is used for caching some information needed by
 * media library, originally provided by database model classes.
 */
class LibraryInfo
{
public:
	LibraryInfo(Fragment *fragment);

	inline const QTime& getFragmentStartTime() const { return startTime_; }
	inline const QTime& getFragmentEndTime() const { return endTime_; }
	inline const QTime& getEpisodeLengthTime() const { return episodeLengthTime_; }
	inline const QString& getTagsString() const { return tagsString_; }

	void update(Fragment *fragment);

private:
	QTime startTime_;
	QTime endTime_;
	QTime episodeLengthTime_;
	QString tagsString_;

};

#endif // LIBRARYITEM_HPP
