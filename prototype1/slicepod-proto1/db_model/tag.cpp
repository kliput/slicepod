#include "precompiled.hpp"

#include "db_model.hpp"

QX_REGISTER_CPP_QX_PODSLICER(Tag)

namespace qx {
template <> void register_class(QxClass<Tag>& t)
{
	t.id(&Tag::id, "id");

	t.data(&Tag::name, "name");
	t.data(&Tag::metadata, "metadata");

	t.relationManyToMany(&Tag::slices_list, "slices_list", "SliceTagMap", "tag_id", "slice_id");

}
}
