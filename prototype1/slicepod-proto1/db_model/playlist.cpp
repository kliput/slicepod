#include "precompiled.hpp"

#include "db_model.hpp"

QX_REGISTER_CPP_QX_SLICEPOD(Playlist)
using namespace db_fields::playlist;

namespace qx {
template <> void register_class(QxClass<Playlist>& t)
{
	t.id(&Playlist::id, db_fields::ID);

	t.data(&Playlist::name, NAME);
	t.data(&Playlist::metadata, METADATA);

	t.relationManyToMany(&Playlist::fragments_list, FRAGMENTS_LIST,
						db_fields::playlist_fragment_map::TABLE_NAME,
						db_fields::playlist_fragment_map::PLAYLIST,
						db_fields::playlist_fragment_map::FRAGMENT);
}
}
