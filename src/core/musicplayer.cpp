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

#include <QtMultimediaKit/QMediaContent>

#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

MusicPlayer::MusicPlayer(QObject *parent) :
	QObject(parent)
{
	mediaPlayer_ = new QMediaPlayer(this);

	connect(mediaPlayer_, SIGNAL(positionChanged(qint64)),
			this, SLOT(emitPositionUpdate(qint64)));
}

MusicPlayer::~MusicPlayer()
{
}

bool MusicPlayer::loadMedia(LibraryItem* item)
{
	if (!item) {
		qFatal("MusicPlayer::loadMedia(item) -- item is null!");
		return false;
	}

	currentItem_ = item;

	QMediaContent content(QUrl::fromLocalFile(item->fileFullPath()));

	if (content.isNull()) {
		qFatal("MusicPlayer::loadMedia(...) -- cannot create MediaContent");
		return false;
	}

	mediaPlayer_->setMedia(content);

	return true;
}

/**
 * @brief MusicPlayer::emitPositionUpdate slot is used to catch signal with
 * position change in milliseconds and emit signal with time value rounded to
 * seconds.
 * @param positionMs position of media in milliseconds
 */
void MusicPlayer::emitPositionUpdate(qint64 positionMs)
{
	int n_pos = positionMs/1000;

	if (positionMs%1000 >= 500) {
		n_pos += 1;
	}

	emit positionChanged(n_pos);
}

void MusicPlayer::play()
{
	mediaPlayer_->play();
}

/**
 * @brief MusicPlayer::seek blocking method used to change position in currently
 * loaded media. It can be used even when media is not played. If media position
 * cannot be changed for 2 seconds, it gives error TODO
 * @param positionSec new position of media in seconds.
 */
void MusicPlayer::seek(int positionSec)
{
	qint64 positionMs = positionSec*1000;

	if (mediaPlayer_->isSeekable()) {
		mediaPlayer_->setPosition(positionMs);
	} else {
		qDebug() << "Media player is not seekable - waiting...";

		QEventLoop loop;
		QTimer timer;
		timer.setSingleShot(true);
		timer.setInterval(2000);
		connect(mediaPlayer_, SIGNAL(seekableChanged(bool)),
				&loop, SLOT(quit()));
		connect(&timer, SIGNAL(timeout()),
				&loop, SLOT(quit()));
		loop.exec();

		if (mediaPlayer_->isSeekable()) {
			mediaPlayer_->setPosition(positionMs);
		} else {
			qFatal("Timeout on waiting for seekable media player");
			// TODO error
		}
	}
}

