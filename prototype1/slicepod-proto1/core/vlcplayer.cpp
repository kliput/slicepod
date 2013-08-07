#include <QtCore>

#include "vlcplayer.hpp"
#include "libraryitem.hpp"

#include <vlc/vlc.h>

#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

// --- global VLC handlers ---

void handle_wait_for_seek(const libvlc_event_t*, void* instance)
{
	VLCPlayer* player = static_cast<VLCPlayer*>(instance);
	if (!player) throw 0; // TODO!

	player->seek(player->currentItem_->fragmentStartSec());

	libvlc_event_detach(player->vlcMPEventManager_, libvlc_MediaPlayerPlaying,
						handle_wait_for_seek, instance);
}

// ---

VLCPlayer::VLCPlayer(QObject *parent) :
	QObject(parent)
{
	// load the vlc engine
	vlcInstance_ = libvlc_new(0, nullptr);
	vlcMediaPlayer_ = libvlc_media_player_new(vlcInstance_);

	vlcMPEventManager_ = libvlc_media_player_event_manager(vlcMediaPlayer_);

	positionTimer_ = new QTimer(this);
	positionTimer_->setInterval(POSITION_UPDATE_FREQ);

	connect(positionTimer_, SIGNAL(timeout()),
			this, SLOT(emitPositionUpdate()));

	// TODO: set callbacks

	// TODO: throw on null pointers
}

VLCPlayer::~VLCPlayer()
{
	libvlc_release(vlcInstance_);
	libvlc_media_player_release(vlcMediaPlayer_);
	libvlc_media_release(currentMedia_);
}

bool VLCPlayer::loadMedia(LibraryItem* item)
{
	if (!item) {
		qFatal("VLCPlayer::loadMedia(item) -- item is nullptr!");
		return false;
	}

	currentItem_ = item;
	currentMedia_ = libvlc_media_new_path(vlcInstance_,
										  qPrintable(item->fileFullPath()));

	if (!currentMedia_) {
		qFatal("VLCPlayer::loadMedia(...) -- cannot create vlc media");
		return false;
	}

	// previous media should be released by vlclib
	libvlc_media_player_set_media(vlcMediaPlayer_, currentMedia_);

	return true;
}

void VLCPlayer::emitPositionUpdate()
{
	emit positionChanged(libvlc_media_player_get_time(vlcMediaPlayer_)/1000);
}

void VLCPlayer::rewind()
{
	seek(0);
}

void VLCPlayer::play()
{
	// TODO: handle errors
	int status = libvlc_media_player_play(vlcMediaPlayer_);

	libvlc_event_attach(vlcMPEventManager_, libvlc_MediaPlayerPlaying,
						handle_wait_for_seek, this);

	positionTimer_->start();
}

void VLCPlayer::seek(int positionSec)
{
	libvlc_media_player_set_time(vlcMediaPlayer_, positionSec);
	emit positionChanged(positionSec);
}

