/****************************************************************************
* Slicepod
* Copyright (C) 2013 Jakub Liput <jakub.liput@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/ 

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
