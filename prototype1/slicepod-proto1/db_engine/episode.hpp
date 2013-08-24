#ifndef EPISODE_HPP
#define EPISODE_HPP

#include <QString>
#include <QList>

#include <taglib/fileref.h>

#include "baserecord.hpp"
#include "directory.hpp"
#include "podcast.hpp"

namespace db {

namespace episode {
constexpr const char* TABLE_NAME = "Episodes";
constexpr const char* FILE_NAME = "file_name";
constexpr const char* EPISODE_NAME = "episode_name";
constexpr const char* DIRECTORY = "directory_id";
constexpr const char* PODCAST = "podcast_id";
constexpr const char* START_FRAGMENT = "start_fragment_id";
constexpr const char* COMPARE_DATA = "compare_data";
constexpr const char* METADATA = "metadata";
}

}

class Fragment;

class Episode : public BaseRecord<Episode>
{
public:
	static constexpr const char* TABLE_NAME = db::episode::TABLE_NAME;
	static const char* schemaString();
	Episode(QSqlRecord record, DatabaseEngine* engine = db::global_engine());
	Episode(const QString& fileName, const QString& title, Podcast::ptr podcast,
			Directory::ptr directory, DatabaseEngine *engine = db::global_engine());
	virtual ~Episode() {}

	inline const QString& getEpisodeName() const { return episodeName_; }
	QSharedPointer<Directory> getDirectory() const;
	inline const QString& getFileName() const { return fileName_; }
	QSharedPointer<Podcast> getPodcast() const;
	QSharedPointer<Fragment> getStartFragment() const;
	inline const QString& getCompareData() const { return compareData_; }
	inline const QString& getMetadata() const { return metadata_; }

	inline void setEpisodeName(const QString& episodeName) { fieldChange(); episodeName_ = episodeName; }
	void setDirectory(QSharedPointer<Directory> getDirectory);
	inline void setFileName(const QString& fileName) { fieldChange(); fileName_ = fileName; }
	void setPodcast(QSharedPointer<Podcast> getPodcast);
	void setStartFragment(QSharedPointer<Fragment> getStartFragment);
	inline void setCompareData(const QString& compareData) { fieldChange(); compareData_ = compareData; }
	inline void setMtadata(const QString& metadata) { fieldChange(); metadata_ = metadata; }

	QList<QSharedPointer<Fragment>> getFragmentsList() const;

	QString getFullPath() const;

	QList<QVariant> valuesList() const override;
	const QStringList& columnsList() const override;

	template <class T> int referenceId() const;

	TagLib::FileRef fileInfo() const;
	int audioLength() const;

private :
	QString episodeName_;
	int directoryId_ = -1;
	QString fileName_;
	int podcastId_ = -1;
	int startFragmentId_ = -1;
	QString compareData_;
	QString metadata_;
};

template <> inline int Episode::referenceId<Directory>() const { return getDirectory()->id(); }
template <> inline int Episode::referenceId<Podcast>() const { return getPodcast()->id(); }


#endif // EPISODE_HPP
