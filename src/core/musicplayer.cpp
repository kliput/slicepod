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

#include "musicplayer.hpp"
#include "libraryitem.hpp"

#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>

#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

MusicPlayer::MusicPlayer(QObject *parent) :
	QObject(parent)
{
	vlcInstance = new VlcInstance(VlcCommon::args(), this);
	vlcPlayer = new VlcMediaPlayer(vlcInstance);

//	connect(mediaPlayer_, SIGNAL(positionChanged(qint64)),
//			this, SLOT(emitPositionUpdate(qint64)));
}

MusicPlayer::~MusicPlayer()
{
	// delete vlcInstances? TODO: check
	delete vlcPlayer->currentMedia();
}

bool MusicPlayer::loadMedia(LibraryItem* item)
{
	if (!item) {
		qFatal("MusicPlayer::loadMedia(item) -- item is null!");
		return false;
	}

	currentItem_ = item;

	VlcMedia* m = new VlcMedia(item->fileFullPath(), true, vlcInstance);

	if (!m->core()) {
		qFatal("MusicPlayer::loadMedia(...) -- cannot create VlcMedia");
		return false;
	}

	delete vlcPlayer->currentMedia();
	vlcPlayer->openOnly(m);

	return true;
}

///**
// * @brief MusicPlayer::emitPositionUpdate slot is used to catch signal with
// * position change in milliseconds and emit signal with time value rounded to
// * seconds.
// * @param positionMs position of media in milliseconds
// */
//void MusicPlayer::emitPositionUpdate(qint64 positionMs)
//{
//	int n_pos = positionMs/1000;

//	if (positionMs%1000 >= 500) {
//		n_pos += 1;
//	}

//	emit positionChanged(n_pos);
//}

void MusicPlayer::play()
{
	scheduleTimeChange(currentItem_->fragmentStartSec()*1000);
	vlcPlayer->play();
}

void MusicPlayer::scheduleTimeChange(const int& pos)
{
	if (scheduledTime < 0) {
		scheduledTime = pos;
		connect(vlcPlayer, SIGNAL(playing()), this, SLOT(changeTime()));
	} else {
		qDebug("Error: there is currently scheduled position change!");
	}
}

void MusicPlayer::changeTime()
{
	if (scheduledTime >= 0) {
		qDebug("Changing position to %d", scheduledTime);
		vlcPlayer->setTime(scheduledTime);
		scheduledTime = -1; // reset
		disconnect(vlcPlayer, SIGNAL(playing()), this, SLOT(changeTime()));
	} else {
		qDebug("Error: there is no currently scheduled position!");
	}
}
