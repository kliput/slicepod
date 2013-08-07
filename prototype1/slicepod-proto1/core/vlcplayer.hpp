#ifndef VLCMANAGER_H
#define VLCMANAGER_H

#include <QtCore>

#include <vlc/vlc.h>
#include "libraryitem.hpp"

constexpr int POSITION_UPDATE_FREQ = 1000;

class VLCPlayer : public QObject
{
	Q_OBJECT
public:
	explicit VLCPlayer(QObject *parent = nullptr);
	virtual ~VLCPlayer();

	bool loadMedia(LibraryItem *item);

	void seek(int positionSec);

	friend void handle_wait_for_seek(const libvlc_event_t*, void* instance);

private:
	libvlc_instance_t *vlcInstance_;
	libvlc_media_player_t *vlcMediaPlayer_;
	libvlc_event_manager_t *vlcMPEventManager_;

	LibraryItem *currentItem_ = nullptr;
	libvlc_media_t *currentMedia_ = nullptr;

	QTimer *positionTimer_;

private slots:
	void emitPositionUpdate();

public slots:
	void play();
	void rewind();

signals:
	void positionChanged(int);

};

#endif // VLCMANAGER_H
