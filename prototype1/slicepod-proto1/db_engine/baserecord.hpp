#ifndef ABSTRACTRECORD_HPP
#define ABSTRACTRECORD_HPP

#include <QList>
#include <QVariant>
#include <QSharedPointer>
#include <QMap>

#include "databaseengine.hpp" // includes db::global_engine()

namespace db {
constexpr const char* ID = "id";
}

class QSqlRecord;

template <class T>
class BaseRecord
{
public:
	BaseRecord(QSqlRecord record, DatabaseEngine* engine = db::global_engine());
	BaseRecord(DatabaseEngine* engine = db::global_engine());

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

protected:
	DatabaseEngine* engine_;
	bool dirty_ = false;

	//! Insert this record into database and if successful - returns pointer
	//! to associated record in database engine.
	QSharedPointer<T> insert();
	QSharedPointer<T> update();

	inline void fieldChange() { dirty_ = true; }

private:
	int id_ = -1;
	bool stored_ = false;
};


template <class T>
BaseRecord<T>::BaseRecord(QSqlRecord record, DatabaseEngine* engine) :
	engine_(engine),
	id_(record.value(db::ID).toInt()),
	stored_(true)
{
}

template <class T>
BaseRecord<T>::BaseRecord(DatabaseEngine *engine) :
	engine_(engine),
	id_(-1),
	stored_(false)
{
}

template <class T>
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
	QSharedPointer<T> db_record = engine_->insert<T>(columnsList(), valuesList());
	if (db_record) {
		id_ = db_record->id();
		dirty_ = false;
	}
	return db_record;
}

template <class T>
QSharedPointer<T> BaseRecord<T>::update()
{
	return engine_->update<T>(id(), columnsList(), valuesList());
}


#endif // ABSTRACTRECORD_HPP
