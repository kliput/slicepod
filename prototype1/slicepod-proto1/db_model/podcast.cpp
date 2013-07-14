#include "precompiled.hpp"

#include "db_model.hpp"
using namespace db::field::podcast;

QX_REGISTER_CPP_QX_SLICEPOD(Podcast)

namespace qx {
template <> void register_class(QxClass<Podcast>& t)
{
   t.id(&Podcast::id, db::field::ID);

   t.data(&Podcast::name, NAME);
   t.data(&Podcast::metadata, METADATA);
   
   t.relationOneToMany(&Podcast::episodes_list, EPISODES_LIST,
					   db::field::episode::PODCAST);
   
}
}

// -- LibraryItem

db::type::str Podcast::info() const
{
	return name;
}

