#include "precompiled.hpp"

#include "db_model.hpp"
#include <vlc/vlc.h>

QX_REGISTER_CPP_QX_SLICEPOD(Episode)
using namespace db_fields::episode;

namespace qx {
template <> void register_class(QxClass<Episode>& t)
{
	t.id(&Episode::id, db_fields::ID);
	
	t.relationManyToOne(&Episode::directory, DIRECTORY);
	t.data(&Episode::file_name, FILE_NAME);
	t.data(&Episode::episode_name, EPISODE_NAME);
	t.relationManyToOne(&Episode::podcast, PODCAST);
	t.relationOneToOne(&Episode::start_fragment, START_FRAGMENT);
	t.data(&Episode::compare_data, COMPARE_DATA);
	t.data(&Episode::metadata, METADATA);
	
	t.relationOneToMany(&Episode::fragments_list, FRAGMENTS_LIST,
						db_fields::fragment::EPISODE);
}
}

