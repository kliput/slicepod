#include "precompiled.hpp"

#include "db_model.hpp"

QX_REGISTER_CPP_QX_PODSLICER(Podcast)

namespace qx {
template <> void register_class(QxClass<Podcast>& t)
{
   t.id(&Podcast::id, "id");

   t.data(&Podcast::name, "name");
   t.data(&Podcast::metadata, "metadata");
   
   t.relationOneToMany(&Podcast::episodes_list, "episodes_list", "podcast_id");
   
}
}

// -- LibraryItem

QString Podcast::info() const
{
	return name;
}
