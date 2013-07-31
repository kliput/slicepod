#include "precompiled.hpp"

#include "db_model.hpp"
using namespace db::field::tag;

QX_REGISTER_CPP_QX_SLICEPOD(Tag)

namespace qx {
template <> void register_class(QxClass<Tag>& t)
{
	t.id(&Tag::id, db::field::ID);

	t.data(&Tag::name, NAME);
	t.data(&Tag::metadata, METADATA);

	t.relationManyToMany(&Tag::fragmentsList, FRAGMENTS_LIST,
						 db::field::fragment_tag_map::TABLE_NAME,
						 db::field::fragment_tag_map::TAG,
						 db::field::fragment_tag_map::FRAGMENT);

}
}
