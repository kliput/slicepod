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
