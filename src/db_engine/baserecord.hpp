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

#ifndef ABSTRACTRECORD_HPP
#define ABSTRACTRECORD_HPP

#include <QList>
#include <QVariant>
#include <QSharedPointer>
#include <QMap>

#include "databaseengine.hpp"

namespace db {
constexpr const char* ID = "id";
}

class QSqlRecord;

/**
 * @brief The BaseRecord template class is used as base for Slicepod data model
 * record classes. By record class "B" it should be inherited as:
 * public BaseRecord<B>. This version of class uses global database engine
 * instance for convenince. However, it has getEngine() method, if in future
 * multiple instances of DatabaseEngine will be needed.
 */
template <class T>
class BaseRecord
{
public:
	BaseRecord(QSqlRecord record);
	BaseRecord();

	typedef QSharedPointer<T> ptr;
	typedef QString str;
	typedef QList<ptr> ptr_list;

	inline int id() const { return id_; }

	QSharedPointer<T> save();
	inline bool isStored() const { return stored_; }
	inline bool isDirty() const { return dirty_; }

	friend class DatabaseEngine;

	virtual QList<QVariant> valuesList() const = 0;
	virtual const QStringList& columnsList() const = 0;

	inline DatabaseEngine* getEngine() const { return DatabaseEngine::getInstance(); }

protected:
	bool dirty_ = false;

	//! Insert this record into database and if successful - returns pointer
	//! to associated record in database engine.
	QSharedPointer<T> insert();
	QSharedPointer<T> update();

	//! This method should be invoked on every record object's field change
	inline void fieldChange() { dirty_ = true; }

private:
	int id_ = -1;
	bool stored_ = false;
};


template <class T>
BaseRecord<T>::BaseRecord(QSqlRecord record) :
	id_(record.value(db::ID).toInt()),
	stored_(true)
{
}

template <class T>
BaseRecord<T>::BaseRecord() :
	id_(-1),
	stored_(false)
{
}

template <class T>
/**
 * @brief BaseRecord<T>::save if record's object has valid id it updates record
 * in database. Otherwise, it tries to insert new record.
 * @return shared pointer to saved record's object in database engine
 */
QSharedPointer<T> BaseRecord<T>::save()
{
	if (id_ >= 0) {
		return update();
	} else {
		return insert();
	}
}

template <class T>
QSharedPointer<T> BaseRecord<T>::insert()
{
	QSharedPointer<T> db_record = getEngine()->template insert<T>(columnsList(), valuesList());
	if (db_record) {
		id_ = db_record->id();
		dirty_ = false;
	}
	return db_record;
}

template <class T>
QSharedPointer<T> BaseRecord<T>::update()
{
	return getEngine()->template update<T>(id(), columnsList(), valuesList());
}


#endif // ABSTRACTRECORD_HPP
