#include "fragmenttagmap.hpp"
#include "fragment.hpp"
#include "tag.hpp"

FragmentTagMap::FragmentTagMap(QSqlRecord record, DatabaseEngine *engine) :
	BaseRecord<FragmentTagMap>(record, engine),
	fragmentId_(record.value(db::fragment_tag_map::FRAGMENT).toInt()),
	tagId_(record.value(db::fragment_tag_map::TAG).toInt())
{
}

FragmentTagMap::FragmentTagMap(const QSharedPointer<Fragment>& fragment,
							   const QSharedPointer<Tag>& tag) :
	BaseRecord<FragmentTagMap>()
{
	setFragment(fragment);
	setTag(tag);
}

FragmentTagMap::FragmentTagMap(int fragmentId, int tagId, DatabaseEngine* engine) :
	BaseRecord<FragmentTagMap>(engine),
	fragmentId_(fragmentId),
	tagId_(tagId)
{
}

// TODO: parametrize
const char *FragmentTagMap::schemaString()
{
	return R"(FragmentTagMap (
				id integer primary key,
				fragment_id not null references Fragments(id) on delete cascade on update cascade,
				tag_id not null references Tags(id) on delete cascade on update cascade
			)
	)";
}

QList<QVariant> FragmentTagMap::valuesList() const
{
	return QList<QVariant>() << fragmentId_ << tagId_;
}

const QStringList &FragmentTagMap::columnsList() const
{
	using namespace db::fragment_tag_map;
	static auto cl = QStringList({FRAGMENT, TAG});

	return cl;
}

QSharedPointer<Tag> FragmentTagMap::getTag() const
{
	return engine_->record<Tag>(tagId_);
}

void FragmentTagMap::setTag(QSharedPointer<Tag> tag)
{
	if (tag) {
		fieldChange();
		tagId_ = tag->id();
	}
}

QSharedPointer<Fragment> FragmentTagMap::getFragment() const
{
	return engine_->record<Fragment>(fragmentId_);
}

void FragmentTagMap::setFragment(QSharedPointer<Fragment> fragment)
{
	if (fragment) {
		fieldChange();
		fragmentId_ = fragment->id();
	}
}
