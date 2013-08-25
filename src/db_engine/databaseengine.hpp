#ifndef DATABASEENGINE_HPP
#define DATABASEENGINE_HPP

#include <QtCore>
#include <QtSql>

class Directory;
class Podcast;
class Episode;
class Fragment;
class FragmentTagMap;
class Tag;
class Playlist;
class FragmentPlaylistMap;

class DatabaseEngine
{
public:
	~DatabaseEngine();
	bool open(const QString &path);

	template <class T> QSharedPointer<T> record(int id);
	template <class T> QList<QSharedPointer<T>> list();

	//! Return list of records of class B associated with 1-to-many relationship
	//! with class B. B class must implement A* reference() template method.
	template <class A, class B> QList<QSharedPointer<B>> refs(int recordId);

	void fetchAllTables();
	void createAllTables();

	template <class T> static bool createTable();
	template <class T> static bool dropTable();

	template <class T> QSqlError fetchAll();
	template <class T> QSharedPointer<T> fetchById(int id);
	template <class T> QSharedPointer<T> insert(const QStringList& columns,
								 const QList<QVariant>& values);
	template <class T> QSharedPointer<T> update(int id, const QStringList& columns,
								 const QList<QVariant>& values);
	template <class T> bool remove(int id);

	template <class T> QSharedPointer<T> insertRecord(int id, const QSqlRecord& record);

	template <class T> QList<QSharedPointer<T>> insertMultiple(QList<T> records);
	template <class T> bool updateMultiple(QList<QSharedPointer<T>> records);
	template <class T> QList<QSharedPointer<T>> fetchOnlyNew();

	inline bool isOpened() { return opened_; }

protected:
	template <class T> QMap<int, QSharedPointer<T>>& map();
	void insertToMap(const QSqlRecord &record);

private:
	QSqlDatabase database_;
	bool opened_ = false;
};

template <class T>
inline QMap<int, QSharedPointer<T>>& DatabaseEngine::map()
{
	static QMap<int, QSharedPointer<T>> mp;
	return mp;
}

template <class T>
inline QList<QSharedPointer<T> > DatabaseEngine::list()
{
	return map<T>().values();
}

template <class T>
inline QSharedPointer<T> DatabaseEngine::record(int id)
{
	return map<T>().value(id);
}

//! Inserts record class based on BaseRecord constructed with record to map
//! of T-records. If there is already record with id, it's smart pointer data
//! will be OVERWRITTEN. If there is no record with id, it will be created
//! with new shared pointer.
template <class T>
QSharedPointer<T> DatabaseEngine::insertRecord(int id, const QSqlRecord& record)
{
	if (map<T>().contains(id)) {
	   // if entry with id already exists, copy into smart pointer data
	   *map<T>()[id] = T(record, this);
	} else {
	   auto m = QSharedPointer<T>(new T(record, this));
	   map<T>().insert(m->id(), m);
	}
	return map<T>()[id];
}


template <class T>
QSharedPointer<T> DatabaseEngine::fetchById(int id)
{
	QSqlTableModel tableModel;

	tableModel.setTable(T::TABLE_NAME);
	tableModel.setFilter(QString("id=%1").arg(id));
	tableModel.select();

	if (tableModel.rowCount() == 1) {
	   qDebug("Fetched row with id=%d from %s table.", id, T::TABLE_NAME);
	   return insertRecord<T>(id, tableModel.record(0));
	} else {
	   qCritical("Fetching row with id=%d from %s table failed!", id, T::TABLE_NAME);
	   return QSharedPointer<T>();
	}
}



template <class T>
QList<QSharedPointer<T>> DatabaseEngine::fetchOnlyNew()
{
	QSqlTableModel tableModel;

	tableModel.setTable(T::TABLE_NAME);

	if (!tableModel.select()) {
		qCritical("Cannot fetch rows from %s table, error: %s", T::TABLE_NAME,
				 qPrintable(tableModel.lastError().text()));
		return QList<QSharedPointer<T>>();
	} else {
		QList<QSharedPointer<T>> nRecords;

		auto& mp = map<T>();

		for (int i=0; i < tableModel.rowCount(); ++i) {
			// add record to cache map if it's not there already (get ID)
			// NOTICE: "id" is not constant here from db:: namespace
			if (!mp.contains(tableModel.record(i).value("id").toInt())) {
				auto m = QSharedPointer<T>(new T(tableModel.record(i), this));
				map<T>().insert(m->id(), m);
				nRecords << m;
			}
		}


		qDebug("Selected %d rows from %s table. Saved %d of them to cache.",
			   tableModel.rowCount(), T::TABLE_NAME, nRecords.size());

		return nRecords;
	}
}

template <class T>
QSharedPointer<T> DatabaseEngine::insert(const QStringList& columns,
								 const QList<QVariant>& values)
{
	Q_ASSERT(values.size() == columns.size());

	QStringList sl;
	sl.reserve(columns.size());
	for (int i=0; i<columns.size(); ++i) sl << QString('?');

	QString queryStr = QString("INSERT INTO %1 (%2) VALUES (%3)").arg(
				T::TABLE_NAME, columns.join(","), sl.join(","));
	QSqlQuery query;
	query.prepare(queryStr);
	for (auto v: values) {
		query.addBindValue(v);
	}

	bool status = query.exec();

	QSharedPointer<T> p;

	if (status) {
		int n_id = query.lastInsertId().toInt();

		qDebug("Inserted record to %s with id: %d", T::TABLE_NAME,
			   n_id);

		p = fetchById<T>(n_id);

		if (!p) { // inserted, but cannot fetch
			qCritical("Previously inserted record with id=%d to table %s "
				   "cannot be fetched. Please check database readibiliy.",
				   n_id, T::TABLE_NAME);
		}

	} else {
		qCritical("Inserting record to table %s failed: %s", T::TABLE_NAME,
				   qPrintable(query.lastError().text()));
	}

	return p;
}

template <class T>
QList<QSharedPointer<T>> DatabaseEngine::insertMultiple(QList<T> records)
{
	Q_ASSERT(!records.empty());

	const QStringList& columns = records[0].columnsList();

	QVector<QVariantList> multiValues(columns.size());
	for (int i=0; i<multiValues.size(); ++i) {
		multiValues[i] = QVariantList();
		for (T& r: records) {
			multiValues[i].append(r.valuesList()[i]);
		}
	}

	QStringList sl;
	sl.reserve(columns.size());
	for (int i=0; i<columns.size(); ++i) sl << QString('?');

	QString queryStr = QString("INSERT INTO %1 (%2) VALUES (%3)").arg(
				T::TABLE_NAME, columns.join(","), sl.join(","));
	QSqlQuery query;
	query.prepare(queryStr);
	for (auto mv: multiValues) {
		query.addBindValue(mv);
	}

	database_.transaction();
	bool status = query.execBatch();
	database_.commit();

	if (status) {

		auto output = fetchOnlyNew<T>();
		if (output.size() != records.size()) {
			qWarning("Warning: only %d/%d records was inserted on inserting "
					 "to %s table!", output.size(), records.size(), T::TABLE_NAME);
		} else {
			qDebug("Inserted multiple records to %s", T::TABLE_NAME);
		}

		return output;

	} else {
		qCritical("Inserting multiple records to table %s failed: %s", T::TABLE_NAME,
				   qPrintable(query.lastError().text()));

		return QList<QSharedPointer<T>>();
	}
}

template <class T>
inline QSharedPointer<T> DatabaseEngine::update(int id, const QStringList& columns,
								 const QList<QVariant>& values)
{
	Q_ASSERT(values.size() == columns.size());

	QStringList sl;
	sl.reserve(columns.size());
	for (int i=0; i<columns.size(); ++i) sl << QString('?');

	QString settingStr = columns.join("=?,") + QString("=?");

	QString queryStr = QString("UPDATE %1 SET %2 WHERE id=%3").arg(
				T::TABLE_NAME, settingStr).arg(id);

	QSqlQuery query;
	query.prepare(queryStr);
	for (auto v: values) {
		query.addBindValue(v);
	}

	qDebug("Update query to be executed: %s", qPrintable(query.lastQuery()));

	bool status = query.exec();

	QSharedPointer<T> p;

	if (status) {
		qDebug("Record with id=%d in table %s updated successfully.", id,
			   T::TABLE_NAME);

		p = fetchById<T>(id); // TODO: do in-memory only
	} else {
		qCritical("Error: Updating record with id=%d in table %s failed: %s. "
				  "Reverting record in cache.", id, T::TABLE_NAME,
				  qPrintable(query.lastError().text()));
		fetchById<T>(id);
	}

	return p;
}

template <class T>
bool DatabaseEngine::updateMultiple(QList<QSharedPointer<T>> records)
{
	Q_ASSERT(!records.empty());

	const QStringList& columns = records[0]->columnsList();

	for (const auto& r: records) {
		Q_ASSERT(r->isStored());
	}

	QVector<QVariantList> multiValues(columns.size() + 1);
	for (int i=0; i<columns.size(); ++i) {
		multiValues[i].reserve(records.size());
		for (const auto& r: records) {
			multiValues[i].append(r->valuesList()[i]);
		}
	}
	{ // add id's to values lists
		QVariantList& idsList = multiValues[columns.size()];
		idsList.reserve(records.size());
		for (const auto& r: records) {
			idsList.append(r->id());
		}
	}

	QStringList sl;
	sl.reserve(columns.size());
	for (int i=0; i<columns.size(); ++i) sl << QString('?');

	QString settingStr = columns.join("=?,") + QString("=?");

	QString queryStr = QString("UPDATE %1 SET %2 WHERE id=?").arg(
		T::TABLE_NAME, settingStr);

	QSqlQuery query;
	query.prepare(queryStr);
	for (auto mv: multiValues) {
		query.addBindValue(mv);
	}

	database_.transaction();
	bool status = query.execBatch();
	database_.commit();

	if (status) {
		qDebug("Updated multiple records in %s", T::TABLE_NAME);
		return true;
	} else {
		qCritical("Updating multiple records to table %s failed: %s", T::TABLE_NAME,
				  qPrintable(query.lastError().text()));
		return false;
	}
}

template <class A, class B>
QList<QSharedPointer<B>> DatabaseEngine::refs(int recordId)
{
	QList<QSharedPointer<B>> resultsList;

	for (QSharedPointer<B> const m: this->list<B>())
	{
		// filtering...
		if (m->template referenceId<A>() == recordId) {
			resultsList << m;
		}
	}

	return resultsList;
}


template <class T>
bool DatabaseEngine::createTable()
{
	auto queryStr = QString("CREATE TABLE IF NOT EXISTS %1").arg(T::schemaString());
	QSqlQuery query(queryStr);
	if (!query.exec()) {
		qDebug("Cannot create %s table: %s", T::TABLE_NAME, qPrintable(query.lastError().text()));
		return false;
	} else {
		return true;
	}
}

template <class T>
bool DatabaseEngine::dropTable()
{
	qWarning("Warning: %s table will be dropped!", T::TABLE_NAME);
	auto queryStr = QString("DROP TABLE IF EXISTS %1").arg(T::TABLE_NAME);
	QSqlQuery query(queryStr);
	if (!query.exec()) {
		qDebug("Cannot drop %s table: %s", T::TABLE_NAME, qPrintable(query.lastError().text()));
		return false;
	} else {
		return true;
	}
}


namespace db {
//// --- GET GLOBAL INSTANCE (SINGLETON) ---
DatabaseEngine* global_engine();
}


#endif // DATABASEENGINE_HPP
