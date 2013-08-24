#ifndef FRAGMENTPLAYLISTMAP_HPP
#define FRAGMENTPLAYLISTMAP_HPP

#include "baserecord.hpp"
#include "fragment.hpp"
#include "playlist.hpp"

namespace db {
namespace fragment_playlist_map {
constexpr const char* TABLE_NAME = "FragmentPlaylistMap";
constexpr const char* PLAYLIST = "playlist_id";
constexpr const char* FRAGMENT = "fragment_id";
}
}

class Playlist;

class FragmentPlaylistMap : public BaseRecord<FragmentPlaylistMap>
{
public:
	static constexpr const char* TABLE_NAME = db::fragment_playlist_map::TABLE_NAME;
	static const char* schemaString();
	FragmentPlaylistMap(QSqlRecord record, DatabaseEngine* engine = db::global_engine());
	FragmentPlaylistMap(const QSharedPointer<Fragment>& fragment,
				   const QSharedPointer<Playlist>& playlist);
	virtual ~FragmentPlaylistMap() {}

	QSharedPointer<Fragment> getFragment() const;
	void setFragment(QSharedPointer<Fragment> fragment);

	QSharedPointer<Playlist> getPlaylist() const;
	void setPlaylist(QSharedPointer<Playlist> playlist);

	QList<QVariant> valuesList() const override;
	const QStringList& columnsList() const override;

	friend FragmentPlaylistMap Playlist::bindFragment(const QSharedPointer<Fragment> &fragment);

private:
	int fragmentId_ = -1;
	int playlistId_ = -1;

	FragmentPlaylistMap(DatabaseEngine *engine, int fragmentId, int playlistId);
};

#endif // FRAGMENTPLAYLISTMAP_HPP
