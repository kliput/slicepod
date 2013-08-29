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
	FragmentTagMap(QSqlRecord record);
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

	FragmentTagMap(int fragmentId, int tagId);
};

#endif // FRAGMENTTAGMAP_HPP
