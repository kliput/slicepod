#include "precompiled.hpp"

#include "db_model.hpp"
#include <vlc/vlc.h>

QX_REGISTER_CPP_QX_PODSLICER(Episode)

namespace qx {
template <> void register_class(QxClass<Episode>& t)
{
	t.id(&Episode::id, "id");
	
	t.relationManyToOne(&Episode::directory, "directory_id");
	t.data(&Episode::file_name, "file_name");
	t.data(&Episode::episode_name, "episode_name");
	t.relationManyToOne(&Episode::podcast, "podcast_id");
	t.data(&Episode::compare_data, "compare_data");
	t.data(&Episode::metadata, "metadata");
	
	t.relationOneToMany(&Episode::slices_list, "slices_list", "episode_id");
}
}


void Episode::play()
{
	libvlc_instance_t *instance;
	libvlc_media_player_t *media_player;
	libvlc_media_t *media;

	// -- load slice data --
	qx::dao::fetch_by_id_with_relation("directory_id", *this);
	qDebug() << "=== fname " << this->file_name;
	qDebug() << "=== dir_path " << this->directory->path;
	
	QString q_path = QString("%1/%2")
		.arg(this->directory->path)
		.arg(this->file_name);
		
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
	libvlc_media_player_set_time(media_player, libvlc_time_t(0)*1000);

	// sleep for length of this episode (seconds) TODO
	sleep(30); // TODO

	// stop playing
	libvlc_media_player_stop(media_player);

	// free the media_player
	libvlc_media_player_release(media_player);

	libvlc_release(instance);
}

// TODO
QString Episode::name() const
{
	return episode_name;
}
