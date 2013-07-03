#include "precompiled.hpp"

#include "db_model.hpp"
#include <vlc/vlc.h>

QX_REGISTER_CPP_QX_PODSLICER(Slice)

namespace qx {
template <> void register_class(QxClass<Slice>& t)
{
	t.id(&Slice::id, "id");
	
	t.relationManyToOne(&Slice::episode, "episode_id");
	t.data(&Slice::start, "start");
	t.data(&Slice::end, "end");
	t.data(&Slice::title, "title");
	t.data(&Slice::artist, "artist");
	t.data(&Slice::metadata, "metadata");
	
	t.relationManyToMany(&Slice::tags_list, "tags_list", "SliceTagMap", "slice_id", "tag_id");
}
}


void Slice::play()
{
	libvlc_instance_t *instance;
	libvlc_media_player_t *media_player;
	libvlc_media_t *media;

	// -- load slice data --
	qx::dao::fetch_by_id_with_relation("episode_id->directory_id", *this);
	qDebug() << "=== fname " << this->episode->file_name;
	qDebug() << "=== dir_path " << this->episode->directory->path;
	
	QString q_path = QString("%1/%2")
		.arg(this->episode->directory->path)
		.arg(this->episode->file_name);
		
	qDebug() << "=== full " << q_path;
	
// 	const char* file_path = q_path.toStdString().c_str();
	
	std::cout << "=== char* " << q_path.toStdString().c_str() << std::endl;
	
	// load the vlc engine
	instance = libvlc_new(0, NULL);

	// create a new item
	media = libvlc_media_new_path(instance, q_path.toStdString().c_str());

	if (media == NULL) {
		qDebug() << "VLC error!";
	}
	
	// create a media play playing environment
	media_player = libvlc_media_player_new_from_media(media);

	// no need to keep the media now
	libvlc_media_release(media);

	// play the media_player
	libvlc_media_player_play(media_player);
	libvlc_media_player_set_time(media_player, libvlc_time_t(this->start)*1000);

	// sleep for length of the this (seconds)
	sleep(this->end - this->start);

	// stop playing
	libvlc_media_player_stop(media_player);

	// free the media_player
	libvlc_media_player_release(media_player);

	libvlc_release(instance);
}

// TODO
QString Slice::name() const
{
	return title;
}