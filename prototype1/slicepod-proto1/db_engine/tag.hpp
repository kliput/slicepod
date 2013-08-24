#ifndef TAG_HPP
#define TAG_HPP

#include "baserecord.hpp"
#include <QString>

namespace db {
namespace tag {
constexpr const char* TABLE_NAME = "Tags";
constexpr const char* NAME = "name";
constexpr const char* METADATA = "metadata";
}
}

class Fragment;

class Tag : public BaseRecord<Tag>
{
public:
	static constexpr const char* TABLE_NAME = db::tag::TABLE_NAME;
	static const char* schemaString();
	Tag(QSqlRecord record, DatabaseEngine* engine = db::global_engine());
	Tag(const QString& name = QString());
	virtual ~Tag() {}

	inline const QString& getMetadata() const { return metadata_; }
	inline void setMetadata(const QString& metadata) { fieldChange(); metadata_ = metadata; }

	inline const QString& getName() const { return name_; }
	inline void setName(const QString& name) { fieldChange(); name_ = name; }

	QList<QSharedPointer<Fragment>> getFragmentsList() const;

	QList<QVariant> valuesList() const override;
	const QStringList& columnsList() const override;

private:
	QString name_;
	QString metadata_;
};

#endif // TAG_HPP
