#include "precompiled.hpp"
#include <vlc/vlc.h>

#include "db_model.hpp"
using namespace db::field::fragment;

QX_REGISTER_CPP_QX_SLICEPOD(Fragment)

namespace qx {
template <> void register_class(QxClass<Fragment>& t)
{
	t.id(&Fragment::id, db::field::ID);
	
	t.relationManyToOne(&Fragment::episode, EPISODE);
	t.data(&Fragment::start, START);
	t.data(&Fragment::end, END);
	t.data(&Fragment::title, TITLE);
	t.data(&Fragment::artist, ARTIST);
	t.data(&Fragment::metadata, METADATA);
	
	t.relationManyToMany(&Fragment::tagsList, TAGS_LIST,
						 db::field::fragment_tag_map::TABLE_NAME,
						 db::field::fragment_tag_map::FRAGMENT,
						 db::field::fragment_tag_map::TAG);

	t.relationManyToMany(&Fragment::playlistsList, PLAYLISTS_LIST,
						 db::field::playlist_fragment_map::TABLE_NAME,
						 db::field::playlist_fragment_map::FRAGMENT,
						 db::field::playlist_fragment_map::PLAYLIST);
}
}

Fragment::Fragment(const Episode::ptr &_episode, int _start,
				   int _end)
	:
			  episode(_episode),
			  start(_start),
			  end(_end<0 ? _start : _end)
{
}

bool Fragment::isStartFragment()
{
	// TODO: optimalize with one fetch
	if (!this->episode) {
		Fragment tmp_fragment(this->id);
		qx::dao::fetch_by_id_with_relation(db::field::fragment::EPISODE,
										   tmp_fragment);
		// complete this' episode field
		this->episode = tmp_fragment.episode;
	}
	if (!this->episode->startFragment) {
		qx::dao::fetch_by_id_with_relation(db::field::episode::START_FRAGMENT,
										   *this->episode);
	}
	// return whether start_fragment in db contains something or null
	return this->episode->startFragment->id == this->id;
}

void Fragment::play()
{
	libvlc_instance_t *instance;
	libvlc_media_player_t *media_player;
	libvlc_media_t *media;

	// -- load fragment data --
	qx::dao::fetch_by_id_with_relation("episode_id->directory_id", *this);
	qDebug() << "=== fname " << this->episode->fileName;
	qDebug() << "=== dir_path " << this->episode->directory->path;
	
	QString q_path = QString("%1/%2")
		.arg(this->episode->directory->path)
		.arg(this->episode->fileName);
		
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

