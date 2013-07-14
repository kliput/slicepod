#include "precompiled.hpp"

#include "db_model.hpp"
using namespace db::field::playlist;

QX_REGISTER_CPP_QX_SLICEPOD(Playlist)

namespace qx {
template <> void register_class(QxClass<Playlist>& t)
{
	t.id(&Playlist::id, db::field::ID);

	t.data(&Playlist::name, NAME);
	t.data(&Playlist::metadata, METADATA);

	t.relationManyToMany(&Playlist::fragments_list, FRAGMENTS_LIST,
						db::field::playlist_fragment_map::TABLE_NAME,
						db::field::playlist_fragment_map::PLAYLIST,
						db::field::playlist_fragment_map::FRAGMENT);
}
}
