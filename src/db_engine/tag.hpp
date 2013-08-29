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
	Tag(QSqlRecord record);
	Tag(const QString& name);
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
