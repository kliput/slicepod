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
