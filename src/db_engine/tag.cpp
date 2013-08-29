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

#include "tag.hpp"
#include "fragment.hpp" // because reference is used
#include "fragmenttagmap.hpp" // because of fragments list getter

class FragmentTagMap;

Tag::Tag(QSqlRecord record) :
	BaseRecord<Tag>(record),
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
	QList<QSharedPointer<FragmentTagMap>> ftl = getEngine()->list<FragmentTagMap>();

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
