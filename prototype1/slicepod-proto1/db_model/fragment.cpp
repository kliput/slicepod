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
			  end(_end)
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

