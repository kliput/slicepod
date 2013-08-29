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

#include "playlist.hpp"
#include "fragment.hpp" // because reference is used
#include "fragmentplaylistmap.hpp" // because of fragments list getter

class FragmentPlaylistMap;

Playlist::Playlist(QSqlRecord record, DatabaseEngine *engine) :
	BaseRecord<Playlist>(record, engine),
	name_(record.value(db::playlist::NAME).toString()),
	metadata_(record.value(db::playlist::METADATA).toString())
{
}

Playlist::Playlist(const QString &name) :
	BaseRecord<Playlist>(),
	name_(name)
{
}

// TODO: parametrize
const char *Playlist::schemaString()
{
	return R"(Playlists (
			id integer primary key,
			name text not null unique,
			metadata text
		)
	)";
}

QList<QSharedPointer<Fragment> > Playlist::getFragmentsList() const
{
	QList<QSharedPointer<FragmentPlaylistMap>> fpl = engine_->list<FragmentPlaylistMap>();

	QList<QSharedPointer<Fragment>> results;

	for (QSharedPointer<FragmentPlaylistMap> fp: fpl) {
		if (fp->getPlaylist()->id() == id()) {
			results << fp->getFragment();
		}
	}

	return results;
}

QList<QVariant> Playlist::valuesList() const
{
	return QList<QVariant>() << name_ << metadata_;
}

const QStringList &Playlist::columnsList() const
{
	using namespace db::playlist;
	static auto cl = QStringList({NAME, METADATA});

	return cl;
}

FragmentPlaylistMap Playlist::Playlist::bindFragment(const QSharedPointer<Fragment> &fragment)
{
	return FragmentPlaylistMap(engine_, this->id(), fragment->id());
}
