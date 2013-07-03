#include "precompiled.hpp"

#include "db_model.hpp"

QX_REGISTER_CPP_QX_PODSLICER(Directory)

namespace qx {
template <> void register_class(QxClass<Directory>& t)
{
   t.id(&Directory::id, "id");

   t.data(&Directory::path, "path");
   t.data(&Directory::metadata, "metadata");
   
   t.relationOneToMany(&Directory::episodes_list, "episodes_list", "directory_id");
   
}
}
