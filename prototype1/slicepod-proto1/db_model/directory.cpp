#include "precompiled.hpp"

#include "db_model.hpp"
using namespace db_fields::directory;

QX_REGISTER_CPP_QX_SLICEPOD(Directory)

namespace qx {
template <> void register_class(QxClass<Directory>& t)
{
	t.id(&Directory::id, db_fields::ID);

	t.data(&Directory::path, PATH);
	t.data(&Directory::metadata, METADATA);

	t.relationOneToMany(&Directory::episodes_list, EPISODES_LIST,
						db_fields::episode::DIRECTORY);

}
}

