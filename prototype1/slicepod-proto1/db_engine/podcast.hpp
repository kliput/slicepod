#ifndef PODCAST_HPP
#define PODCAST_HPP

#include "baserecord.hpp"

namespace db {
namespace podcast {
constexpr const char* TABLE_NAME = "Podcasts";
constexpr const char* NAME = "name";
constexpr const char* METADATA = "metadata";
}
}

class Episode;

class Podcast : public BaseRecord<Podcast>
{
public:
	static constexpr const char* TABLE_NAME = db::podcast::TABLE_NAME;
	static const char* schemaString();
	Podcast(QSqlRecord record, DatabaseEngine* engine = db::global_engine());
	Podcast(const QString& name, DatabaseEngine* engine = db::global_engine());
	virtual ~Podcast() {}

	inline const QString& getName() const { return name_; }
	inline void setName(const QString& name) { fieldChange(); name_ = name; }

	inline const QString& getMetadata() const { return metadata_; }
	inline void setMetadata(const QString& metadata) { fieldChange(); metadata_ = metadata; }

	QList<QSharedPointer<Episode>> getEpisodesList() const;

	QList<QVariant> valuesList() const override;
	const QStringList& columnsList() const override;

private:
	QString name_;
	QString metadata_;
};

#endif // PODCAST_HPP
