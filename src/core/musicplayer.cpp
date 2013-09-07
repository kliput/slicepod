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
#include "libraryinfo.hpp"

#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>

#include <vlc/libvlc.h>
#include <vlc/libvlc_media.h>

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

bool MusicPlayer::loadFragment(Fragment::ptr fragment)
{
	if (!fragment) {
		qFatal("MusicPlayer::loadMedia(item) -- item is null!");
		return false;
	}


	Episode::ptr ep = fragment->getEpisode();
	if (!currentFragment || vlcPlayer->state() == Vlc::Ended ||
			ep->id() != currentFragment->getEpisode()->id()) {

		VlcMedia* m = new VlcMedia(ep->getFullPath(), true, vlcInstance);

		if (!m->core()) {
			qFatal("MusicPlayer::loadMedia(...) -- cannot create VlcMedia");
			return false;
		}

		delete vlcPlayer->currentMedia();

		vlcPlayer->openOnly(m);

	}

	currentFragment = fragment;

	emit fragmentLoaded(currentFragment);

	qDebug("MusicPlayer loaded media with length: %d", getMediaLengthMs());

	return true;
}

void MusicPlayer::play()
{
	vlcPlayer->play();
}

// TODO: use media length changed notification to update length to accurate values
int MusicPlayer::getMediaLengthMs()
{
	if (currentFragment) {
		if (vlcPlayer->state() == Vlc::Playing) {
			int len = vlcPlayer->length();
			if (len > 0) {
				return len;
			}
		}

		return currentFragment->getEpisode()->getAudioLengthSec()*1000;
	} else {
		return 0;
	}

}

void MusicPlayer::scheduleTimeChange(const int& pos)
{	
	if (vlcPlayer->state() == Vlc::Playing) {
		vlcPlayer->setTime(pos);
	} else {
		if (scheduledTime < 0) {
			scheduledTime = pos;
			connect(vlcPlayer, SIGNAL(playing()), this, SLOT(changeTime()));
		} else {
			qDebug("Error: there is currently scheduled position change!");
		}
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
