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
	t.data(&Episode::fileName, FILE_NAME);
	t.data(&Episode::episodeName, EPISODE_NAME);
	t.relationManyToOne(&Episode::podcast, PODCAST);
	t.relationOneToOne(&Episode::startFragment, START_FRAGMENT);
	t.data(&Episode::compare_data, COMPARE_DATA);
	t.data(&Episode::metadata, METADATA);
	
	t.relationOneToMany(&Episode::fragmentsList, FRAGMENTS_LIST,
						db::field::fragment::EPISODE);
}
}

Episode::Episode(const QString &_file_name, const QString &_episode_name,
				 const Directory::ptr &_directory,
				 const Podcast::ptr &_podcast)
	:
			  fileName(_file_name),
			  episodeName(_episode_name),
			  directory(_directory),
			  podcast(_podcast)
{
}

db::type::str Episode::fullPath()
{
	// TODO path string buffered in instance

	if (!this->directory ||
			this->directory->path.isNull()) {
		qx::dao::fetch_by_id_with_relation(DIRECTORY, *this);
	}

	return QString("%1/%2").arg(this->directory->path).arg(this->fileName);
}

/**
 * @brief Gets TagLib file informations from episode's file with "average" audio
 *  properties.
 * @return TagLib::FileRef object. See brief explaination.
 */
TagLib::FileRef Episode::fileInfo()
{
	// TODO store file_info in instance

	QString fpath = this->fullPath();
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
int Episode::audioLength()
{
	auto file_ref = this->fileInfo();
	auto audio_p = file_ref.audioProperties();
	if (audio_p) {
		return audio_p->length();
	} else {
		throw std::runtime_error("TODO cannot read file audio properties");
		// return -1;
	}
}

