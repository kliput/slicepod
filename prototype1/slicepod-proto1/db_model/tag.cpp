#include "precompiled.hpp"

#include "db_model.hpp"

QX_REGISTER_CPP_QX_SLICEPOD(Tag)
using namespace db_fields::tag;

namespace qx {
template <> void register_class(QxClass<Tag>& t)
{
	t.id(&Tag::id, db_fields::ID);

	t.data(&Tag::name, NAME);
	t.data(&Tag::metadata, METADATA);

	t.relationManyToMany(&Tag::fragments_list, FRAGMENTS_LIST,
						 db_fields::fragment_tag_map::TABLE_NAME,
						 db_fields::fragment_tag_map::TAG,
						 db_fields::fragment_tag_map::FRAGMENT);

}
}
