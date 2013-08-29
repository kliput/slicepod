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
