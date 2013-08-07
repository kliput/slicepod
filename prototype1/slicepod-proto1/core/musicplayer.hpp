#ifndef VLCMANAGER_H
#define VLCMANAGER_H

#include <QtCore>

#include <QtMultimediaKit/QAudio>
#include <QtMultimediaKit/QMediaPlayer>
#include "libraryitem.hpp"

constexpr int POSITION_UPDATE_FREQ = 1000;

class MusicPlayer : public QObject
{
	Q_OBJECT
public:
	explicit MusicPlayer(QObject *parent = nullptr);
	virtual ~MusicPlayer();

	bool loadMedia(LibraryItem *item);


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
