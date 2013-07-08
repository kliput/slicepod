#include "precompiled.hpp"

#include "db_model.hpp"

QX_REGISTER_CPP_QX_SLICEPOD(Podcast)
using namespace db_fields::podcast;

namespace qx {
template <> void register_class(QxClass<Podcast>& t)
{
   t.id(&Podcast::id, db_fields::ID);

   t.data(&Podcast::name, NAME);
   t.data(&Podcast::metadata, METADATA);
   
   t.relationOneToMany(&Podcast::episodes_list, EPISODES_LIST,
					   db_fields::episode::PODCAST);
   
}
}

// -- LibraryItem

QString Podcast::info() const
{
	return name;
}

