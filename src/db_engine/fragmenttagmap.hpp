#ifndef FRAGMENTTAGMAP_HPP
#define FRAGMENTTAGMAP_HPP

#include "baserecord.hpp"
#include "fragment.hpp"

namespace db {
namespace fragment_tag_map {
constexpr const char* TABLE_NAME = "FragmentTagMap";
constexpr const char* TAG = "tag_id";
constexpr const char* FRAGMENT = "fragment_id";
}
}

class Tag;

class FragmentTagMap : public BaseRecord<FragmentTagMap>
{
public:
	static constexpr const char* TABLE_NAME = db::fragment_tag_map::TABLE_NAME;
	static const char* schemaString();
	FragmentTagMap(QSqlRecord record, DatabaseEngine* engine = db::global_engine());
	FragmentTagMap(const QSharedPointer<Fragment>& fragment,
				   const QSharedPointer<Tag>& tag);
	virtual ~FragmentTagMap() {}

	QSharedPointer<Fragment> getFragment() const;
	void setFragment(QSharedPointer<Fragment> fragment);

	QSharedPointer<Tag> getTag() const;
	void setTag(QSharedPointer<Tag> tag);

	QList<QVariant> valuesList() const override;
	const QStringList& columnsList() const override;

	friend FragmentTagMap Fragment::bindTag(const QSharedPointer<Tag> &tag);

private:
	int fragmentId_ = -1;
	int tagId_ = -1;

	FragmentTagMap(int fragmentId, int tagId, DatabaseEngine* engine = db::global_engine());
};

#endif // FRAGMENTTAGMAP_HPP
