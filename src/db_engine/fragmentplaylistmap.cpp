#include "fragmentplaylistmap.hpp"
#include "fragment.hpp"
#include "tag.hpp"

FragmentPlaylistMap::FragmentPlaylistMap(QSqlRecord record, DatabaseEngine *engine) :
	BaseRecord<FragmentPlaylistMap>(record, engine),
	fragmentId_(record.value(db::fragment_playlist_map::FRAGMENT).toInt()),
	playlistId_(record.value(db::fragment_playlist_map::PLAYLIST).toInt())
{
}

FragmentPlaylistMap::FragmentPlaylistMap(const QSharedPointer<Fragment>& fragment,
							   const QSharedPointer<Playlist>& playlist) :
	BaseRecord<FragmentPlaylistMap>()
{
	setFragment(fragment);
	setPlaylist(playlist);
}

FragmentPlaylistMap::FragmentPlaylistMap(DatabaseEngine *engine, int fragmentId, int playlistId) :
	BaseRecord<FragmentPlaylistMap>(engine),
	fragmentId_(fragmentId),
	playlistId_(playlistId)
{
}

// TODO: parametrize
const char *FragmentPlaylistMap::schemaString()
{
	return R"(FragmentPlaylistMap (
				id integer primary key,
				fragment_id not null references Fragments(id) on delete cascade on update cascade,
				playlist_id not null references Playlists(id) on delete cascade on update cascade
			)
	)";
}

QList<QVariant> FragmentPlaylistMap::valuesList() const
{
	return QList<QVariant>() << fragmentId_ << playlistId_;
}

const QStringList &FragmentPlaylistMap::columnsList() const
{
	using namespace db::fragment_playlist_map;
	static auto cl = QStringList({FRAGMENT, PLAYLIST});

	return cl;
}

QSharedPointer<Playlist> FragmentPlaylistMap::getPlaylist() const
{
	return engine_->record<Playlist>(playlistId_);
}

void FragmentPlaylistMap::setPlaylist(QSharedPointer<Playlist> playlist)
{
	if (playlist) {
		fieldChange();
		playlistId_ = playlist->id();
	}
}

QSharedPointer<Fragment> FragmentPlaylistMap::getFragment() const
{
	return engine_->record<Fragment>(fragmentId_);
}

void FragmentPlaylistMap::setFragment(QSharedPointer<Fragment> fragment)
{
	if (fragment) {
		fieldChange();
		fragmentId_ = fragment->id();
	}
}
