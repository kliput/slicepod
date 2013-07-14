#include "precompiled.hpp"

#include "db_model.hpp"
using namespace db::field::episode;

QX_REGISTER_CPP_QX_SLICEPOD(Episode)

namespace qx {
template <> void register_class(QxClass<Episode>& t)
{
	t.id(&Episode::id, db::field::ID);
	
	t.relationManyToOne(&Episode::directory, DIRECTORY);
	t.data(&Episode::file_name, FILE_NAME);
	t.data(&Episode::episode_name, EPISODE_NAME);
	t.relationManyToOne(&Episode::podcast, PODCAST);
	t.relationOneToOne(&Episode::start_fragment, START_FRAGMENT);
	t.data(&Episode::compare_data, COMPARE_DATA);
	t.data(&Episode::metadata, METADATA);
	
	t.relationOneToMany(&Episode::fragments_list, FRAGMENTS_LIST,
						db::field::fragment::EPISODE);
}
}


Episode::Episode(const QString &_file_name, const QString &_episode_name,
				 const QSharedPointer<Directory> &_directory,
				 const QSharedPointer<Podcast> &_podcast)
	:
			  file_name(_file_name),
			  episode_name(_episode_name),
			  directory(_directory),
			  podcast(_podcast)
{
}
