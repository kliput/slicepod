#include "podcast.hpp"
#include "episode.hpp"

Podcast::Podcast(QSqlRecord record, DatabaseEngine *engine) :
	BaseRecord<Podcast>(record, engine),
	name_(record.value(db::podcast::NAME).toString()),
	metadata_(record.value(db::podcast::METADATA).toString())
{
}

Podcast::Podcast(const QString &name, DatabaseEngine *engine) :
	BaseRecord<Podcast>(engine),
	name_(name)
{
}

// TODO: parametrize
const char *Podcast::schemaString()
{
	return R"(Podcasts (
			id integer primary key,
			name text not null,
			metadata text
		)
	)";
}

QList<QSharedPointer<Episode> > Podcast::getEpisodesList() const
{
	return engine_->refs<Podcast, Episode>(id());
}

QList<QVariant> Podcast::valuesList() const
{
	return QList<QVariant>() << name_ << metadata_;
}

const QStringList &Podcast::columnsList() const
{
	using namespace db::podcast;
	static auto cl = QStringList({NAME, METADATA});
	return cl;
}
