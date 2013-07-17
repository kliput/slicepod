#include "precompiled.hpp"

#include <stdexcept>
#include <taglib/fileref.h>

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

db::type::str Episode::full_path()
{
	// TODO path string buffered in instance

	if (this->directory->path.isNull()) {
		qx::dao::fetch_by_id_with_relation(DIRECTORY, *this);
	}

	return QString("%1/%2").arg(this->directory->path).arg(this->file_name);
}

/**
 * @brief Gets TagLib file informations from episode's file with "average" audio
 *  properties.
 * @return TagLib::FileRef object. See brief explaination.
 */
TagLib::FileRef Episode::file_info()
{
	// TODO store file_info in instance

	QString fpath = this->full_path();
	// read audio info
	TagLib::FileRef file_ref(fpath.toUtf8());
	if (!file_ref.isNull())
	{
		return file_ref;
	} else {
		// TODO: cannot read file error
		throw std::runtime_error("TODO cannot read file error");
		// return ?;
	}
}

/**
 * @brief Episode::audio_length
 * @return Episode's file audio length in seconds.
 */
int Episode::audio_length()
{
	auto file_ref = this->file_info();
	auto audio_p = file_ref.audioProperties();
	if (audio_p) {
		return audio_p->length();
	} else {
		throw std::runtime_error("TODO cannot read file audio properties");
		// return -1;
	}
}

