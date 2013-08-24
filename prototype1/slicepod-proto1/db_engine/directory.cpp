#include <QtCore>

#include "directory.hpp"
#include "episode.hpp"

// TODO: parametrize
const char *Directory::schemaString()
{
	return R"(Directories (
				id integer primary key,
				path text not null,
				metadata text
			)
	)";
}

Directory::Directory(QSqlRecord record, DatabaseEngine *engine) :
	BaseRecord<Directory>(record, engine),
	path_(record.value(db::directory::PATH).toString()),
	metadata_(record.value(db::directory::METADATA).toString())
{
}

Directory::Directory(const QString &path, DatabaseEngine *engine) :
	BaseRecord<Directory>(engine),
	path_(path)
{
}

QList<QSharedPointer<Episode> > Directory::getEpisodesList() const
{
	return engine_->refs<Directory, Episode>(id());
}

QList<QVariant> Directory::valuesList() const
{
	return QList<QVariant>() << path_ << metadata_;
}

const QStringList &Directory::columnsList() const
{
	using namespace db::directory;
	static auto cl = QStringList({PATH, METADATA});

	return cl;
}
