#ifndef FRAGMENT_HPP
#define FRAGMENT_HPP

#include "baserecord.hpp"
#include "episode.hpp"
#include "tag.hpp"

#include <QString>

namespace db {
namespace fragment {
constexpr const char* TABLE_NAME = "Fragments";
constexpr const char* EPISODE = "episode_id";
constexpr const char* START = "start";
constexpr const char* END = "end";
constexpr const char* TITLE = "title";
constexpr const char* ARTIST = "artist";
constexpr const char* RATING = "rating";
constexpr const char* METADATA = "metadata";
}
}

class Tag;

class Fragment : public BaseRecord<Fragment>
{
public:
	static constexpr const char* TABLE_NAME = db::fragment::TABLE_NAME;
	static const char* schemaString();
	Fragment(QSqlRecord record, DatabaseEngine* engine = db::global_engine());
	Fragment(DatabaseEngine *engine = db::global_engine());
	Fragment(QSharedPointer<Episode> episode,
			 int start, const QString& title, const QString& artist = QString(),
			 int end=-1);
	virtual ~Fragment() {}

	void setEpisode(QSharedPointer<Episode> episode);
	QSharedPointer<Episode> getEpisode() const;

	inline const int& getStart() const { return start_; }
	inline void setStart(const int& start) { fieldChange(); start_ = start; }

	inline const int& getEnd() const { return end_; }
	inline void setEnd(const int& end) { fieldChange(); end_ = end; }

	inline const QString& getTitle() const { return title_; }
	inline void setTitle(const QString& title) { fieldChange(); title_ = title; }

	inline const QString& getArtist() const { return artist_; }
	inline void setArtist(const QString& artist) { fieldChange(); artist_ = artist; }

	inline const QString& getMetadata() const { return metadata_; }
	inline void setMetadata(const QString& metadata) { fieldChange(); metadata_ = metadata; }

	inline const int& getRating() const { return rating_; }
	inline void setRating(const int& rating) { fieldChange(); rating_ = rating; }

	QList<QSharedPointer<Tag>> getTagsList() const;

	FragmentTagMap bindTag(const QSharedPointer<Tag>& tag);

	bool isStartFragment() const;

	QList<QVariant> valuesList() const;
	const QStringList& columnsList() const;

	template <class T> int referenceId() const;

private:
	int episodeId_ = -1;
	int start_ = -1;
	int end_ = -1;
	QString title_;
	QString artist_;
	int rating_ = -1;
	QString metadata_;
};

template <> inline int Fragment::referenceId<Episode>() const { return getEpisode()->id(); }

#endif // FRAGMENT_HPP
