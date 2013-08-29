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

#ifndef VLCMANAGER_H
#define VLCMANAGER_H

#include <QtCore>

#if QT_VERSION >= 0x050000
#include <QtMultimedia/QAudio>
#include <QtMultimedia/QMediaPlayer>
#else
#include <QtMultimediaKit/QAudio>
#include <QtMultimediaKit/QMediaPlayer>
#endif


#include "libraryitem.hpp"

constexpr int POSITION_UPDATE_FREQ = 1000;

class MusicPlayer : public QObject
{
	Q_OBJECT
public:
	explicit MusicPlayer(QObject *parent = nullptr);
	virtual ~MusicPlayer();

	bool loadMedia(LibraryItem *item);

	LibraryItem *currentItem() const { return currentItem_; }

private:
	QMediaPlayer *mediaPlayer_;

	LibraryItem *currentItem_ = nullptr;

private slots:
	//! Receive new position in ms and emit signal with rounded position in sec.
	void emitPositionUpdate(qint64 positionMs);

public slots:
	void play();
	//! Set position of loaded media in seconds
	void seek(int positionSec);

signals:
	//! Music position change in seconds
	void positionChanged(int);

};

#endif // VLCMANAGER_H
