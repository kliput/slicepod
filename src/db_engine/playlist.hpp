#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include "baserecord.hpp"
#include <QString>

namespace db {
namespace playlist {
constexpr const char* TABLE_NAME = "Playlists";
constexpr const char* NAME = "name";
constexpr const char* METADATA = "metadata";
}
}

class Fragment;
class FragmentPlaylistMap;

class Playlist : public BaseRecord<Playlist>
{
public:
	static constexpr const char* TABLE_NAME = db::playlist::TABLE_NAME;
	static const char* schemaString();
	Playlist(QSqlRecord record, DatabaseEngine* engine = db::global_engine());
	Playlist(const QString& name = QString());
	virtual ~Playlist() {}

	inline const QString& getMetadata() const { return metadata_; }
	inline void setMetadata(const QString& metadata) { fieldChange(); metadata_ = metadata; }

	inline const QString& getName() const { return name_; }
	inline void setName(const QString& name) { fieldChange(); name_ = name; }

	QList<QSharedPointer<Fragment>> getFragmentsList() const;

	QList<QVariant> valuesList() const override;
	const QStringList& columnsList() const override;

	FragmentPlaylistMap bindFragment(const QSharedPointer<Fragment> &fragment);

private:
	QString name_;
	QString metadata_;
};

#endif // PLAYLIST_HPP
