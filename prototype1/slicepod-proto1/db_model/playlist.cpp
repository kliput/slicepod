#include "precompiled.hpp"

#include "db_model.hpp"

QX_REGISTER_CPP_QX_PODSLICER(Playlist)

namespace qx {
template <> void register_class(QxClass<Playlist>& t)
{
   t.id(&Playlist::id, "id");

   t.data(&Playlist::name, "name");
   t.data(&Playlist::metadata, "metadata");
   
//    t.relationOneToMany(&Playlist::playables_list, "playables_list", "playable_id");
   
}
}
