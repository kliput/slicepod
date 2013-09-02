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

#include "libraryinfo.hpp"

class VlcInstance;
class VlcMediaPlayer;

constexpr int POSITION_UPDATE_FREQ = 1000;

class MusicPlayer : public QObject
{
	Q_OBJECT
public:
	explicit MusicPlayer(QObject *parent = nullptr);
	virtual ~MusicPlayer();

	bool loadFragmentMedia(Fragment::ptr fragment);

	Fragment::ptr getCurrentFragment() { return currentFragment; }

	VlcMediaPlayer* getVlcPlayer() { return vlcPlayer; }
	void scheduleTimeChange(const int &pos);

private:
	VlcInstance* vlcInstance;
	VlcMediaPlayer* vlcPlayer;

	Fragment::ptr currentFragment;

	int scheduledTime = -1;

private slots:
	void changeTime();

public slots:
	void play();
//	! Set position of loaded media in seconds
//	void seek(int positionSec);

signals:
	//! Music position change in seconds
	void positionChanged(int);

};

#endif // VLCMANAGER_H
