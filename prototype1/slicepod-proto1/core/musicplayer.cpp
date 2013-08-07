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

void MusicPlayer::emitPositionUpdate(qint64 positionMs)
{
	int n_pos = positionMs/1000;

	if (positionMs%1000 >= 500) {
		n_pos += 1;
	}

	qDebug() << "new position:" << positionMs << "->" << n_pos;

	emit positionChanged(n_pos);
}

void MusicPlayer::play()
{
	mediaPlayer_->play();
}

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

