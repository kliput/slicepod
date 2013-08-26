#include "episode.hpp"

#include <QtCore>
#include <QDir>

#include <taglib/fileref.h>

#include "databaseengine.hpp"
#include "directory.hpp"
#include "podcast.hpp"
#include "fragment.hpp"

// TODO: parametrize
const char *Episode::schemaString()
{
	return R"(Episodes (
			id integer primary key,
			episode_name text,
			directory_id integer references Directories(id) on delete cascade on update cascade,
			file_name text,
			podcast_id integer references Podcasts(id) on delete cascade on update cascade,
			start_fragment_id integer references Fragments(id) on update cascade,
			compare_data text,
			metadata text
		)
	)";
}

Episode::Episode(QSqlRecord record, DatabaseEngine* engine) :
	BaseRecord<Episode>(record, engine),
	episodeName_(record.value(db::episode::EPISODE_NAME).toString()),
	directoryId_(record.value(db::episode::DIRECTORY).toInt()),
	fileName_(record.value(db::episode::FILE_NAME).toString()),
	podcastId_(record.value(db::episode::PODCAST).toInt()),
	// startFragmentId_
	compareData_(record.value(db::episode::COMPARE_DATA).toString()),
	metadata_(record.value(db::episode::METADATA).toString())
{
	QVariant startFragmentValue = record.value(db::episode::START_FRAGMENT);
	if (startFragmentValue.isNull() | !startFragmentValue.isValid()) {
		startFragmentId_ = -1;
	} else {
		startFragmentId_ = startFragmentValue.toInt();
	}
}

Episode::Episode(const QString& fileName, const QString& episodeName,
				 Podcast::ptr podcast, Directory::ptr directory, DatabaseEngine *engine) :
	BaseRecord<Episode>(engine),
	episodeName_(episodeName),
	fileName_(fileName)
{
	setPodcast(podcast);
	setDirectory(directory);
}

QSharedPointer<Directory> Episode::getDirectory() const
{
	return engine_->record<Directory>(directoryId_);
}

QSharedPointer<Podcast> Episode::getPodcast() const
{
	return engine_->record<Podcast>(podcastId_);
}

QSharedPointer<Fragment> Episode::getStartFragment() const
{
	return engine_->record<Fragment>(startFragmentId_);
}

void Episode::setDirectory(QSharedPointer<Directory> directory)
{
	if (directory) {
		fieldChange();
		directoryId_ = directory->id();
	}
}

void Episode::setPodcast(QSharedPointer<Podcast> podcast)
{
	if (podcast) {
		fieldChange();
		podcastId_ = podcast->id();
	}
}

void Episode::setStartFragment(QSharedPointer<Fragment> startFragment)
{
	if (startFragment) {
		fieldChange();
		startFragmentId_ = startFragment->id();
	}
}

QList<QSharedPointer<Fragment> > Episode::getFragmentsList() const
{
	return engine_->refs<Episode, Fragment>(id());
}

QString Episode::getFullPath() const
{
	return QDir::toNativeSeparators(QString("%1/%2").arg(getDirectory()->getPath(), getFileName()));
}

QList<QVariant> Episode::valuesList() const
{
	return QList<QVariant>() << episodeName_ << directoryId_ << fileName_
							 << podcastId_
							 << (startFragmentId_<0 ? QVariant(QVariant::Int) : startFragmentId_)
							 << compareData_ << metadata_;
}

const QStringList &Episode::columnsList() const
{
	using namespace db::episode;
	static auto cl = QStringList({EPISODE_NAME, DIRECTORY, FILE_NAME,
								  PODCAST, START_FRAGMENT,
								  COMPARE_DATA, METADATA});
	return cl;
}

/**
 * @brief Gets TagLib file informations from episode's file with "average" audio
 *  properties.
 * @return TagLib::FileRef object. See brief explaination.
 */
TagLib::FileRef Episode::fileInfo() const
{
	// TODO store file_info in instance

	QString fpath = this->getFullPath();
	// read audio info
    TagLib::FileRef file_ref(fpath.toUtf8().data());
	if (!file_ref.isNull())
	{
		return file_ref;
	} else {
		// TODO: cannot read file error
		throw QString("TODO cannot read file error");
		// return ?;
	}
}

/**
 * @brief Episode::audio_length
 * @return Episode's file audio length in seconds.
 */
int Episode::audioLength() const
{
	auto file_ref = this->fileInfo();
	auto audio_p = file_ref.audioProperties();
	if (audio_p) {
		return audio_p->length();
	} else {
		throw QString("TODO cannot read file audio properties");
		// return -1;
	}
}
