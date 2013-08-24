#include "tag.hpp"
#include "fragment.hpp" // because reference is used
#include "fragmenttagmap.hpp" // because of fragments list getter

class FragmentTagMap;

Tag::Tag(QSqlRecord record, DatabaseEngine *engine) :
	BaseRecord<Tag>(record, engine),
	name_(record.value(db::tag::NAME).toString()),
	metadata_(record.value(db::tag::METADATA).toString())
{
}

Tag::Tag(const QString &name) :
	BaseRecord<Tag>(),
	name_(name)
{
}

// TODO: parametrize
const char *Tag::schemaString()
{
	return R"(Tags (
			id integer primary key,
			name text not null unique,
			metadata text
		)
	)";
}

QList<QSharedPointer<Fragment> > Tag::getFragmentsList() const
{
	QList<QSharedPointer<FragmentTagMap>> ftl = engine_->list<FragmentTagMap>();

	QList<QSharedPointer<Fragment>> results;

	for (QSharedPointer<FragmentTagMap> ft: ftl) {
		if (ft->getTag()->id() == id()) {
			results << ft->getFragment();
		}
	}

	return results;
}

QList<QVariant> Tag::valuesList() const
{
	return QList<QVariant>() << name_ << metadata_;
}

const QStringList &Tag::columnsList() const
{
	using namespace db::tag;
	static auto cl = QStringList({NAME, METADATA});

	return cl;
}
