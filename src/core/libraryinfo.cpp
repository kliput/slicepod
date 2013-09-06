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

#include "libraryinfo.hpp"
#include "db_engine/fragment.hpp"
#include "db_engine/tag.hpp"

LibraryInfo::LibraryInfo(Fragment* fragment)
{
	update(fragment);
}

void LibraryInfo::update(Fragment* fragment)
{
	startTime_ = QTime().addMSecs(fragment->getStart());
	endTime_ = QTime().addMSecs(fragment->getEnd());
	episodeLengthTime_ = QTime().addSecs(fragment->getEpisode()->getAudioLengthSec());

	auto tl = fragment->getTagsList();
	QStringList tsl;

	tsl.reserve(tl.size());
	for (auto& t: fragment->getTagsList()) {
		tsl << t->getName();
	}

	tagsString_ = tsl.join(", ");
}
