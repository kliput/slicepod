#include "precompiled.hpp"

#include <QtCore>
#include <QtGui>

#include "core/utils.hpp"
#include "core/sqlexception.hpp"
#include "db_model.hpp"
using namespace db::type;

#include <cstdlib>
#include <iostream>
#include <taglib/tag.h>
#include <taglib/fileref.h>

// -- DATABASE CORE SECTION --

/**
 * @brief Creates global connection to SQLite database with given path.
 * @param db_name SQLite3 database file path
 */
void db_connect(const QString& db_name)
{
	qx::QxSqlDatabase* dbs = qx::QxSqlDatabase::getSingleton();

	// Parameters to connect to database
	dbs->setDriverName("QSQLITE");
	dbs->setDatabaseName(db_name);
	dbs->setHostName("localhost");
	dbs->setUserName("root");
	dbs->setPassword("");

	// Only for debug purpose: assert if invalid offset detected fetching a relation
	qx::QxSqlDatabase::getSingleton()->setVerifyOffsetRelation(true);
}

/**
 * @brief db_check_schema - checks if schema of connected database is valid.
 * @return true if schema is valid, false otherwise
 */
bool db_check_schema()
{
	// TODO: implement
	return false;
}

void db_create_tables()
{
	// -- create all tables in database --
	// if there are any SQL errors - continue
	qx::dao::create_table<Episode>();
	qx::dao::create_table<Directory>();
	qx::dao::create_table<Podcast>();
	qx::dao::create_table<Fragment>();
	qx::dao::create_table<Tag>();
	qx::dao::create_table<Playlist>();
}

/**
 * @brief Checks if SQL error is present. If it is - throws SQLException
 *	with QSqlError object. Otherwise does nothing.
 * @param error QSqlError object to check if it contains error.
 * @param info QString with information about circumstances in which error
 *	occurs
 */
void check_error(const QSqlError& error, const char* info)
{
	if (error.isValid()) {
		throw SQLException(error, info);
	}
}

// -- MODEL FUNCTIONS SECTION --

Fragment::ptr create_start_fragment(const Episode::ptr &episode)
{
	// WARNING: it's only check for fetched start fragment
	if (episode->startFragment.isNull()) {
		Fragment::ptr fragment(new Fragment(episode, 15));
		episode->startFragment = fragment;
		episode->fragmentsList << fragment;
		return fragment;
	} else {
		return episode->startFragment;
	}
}


void set_enabled_for_each(QLayout *layout, bool state)
{
	for (int i=0; i < layout->count(); ++i) {
		if (QWidgetItem *item = dynamic_cast <QWidgetItem*>(layout->itemAt(i))) {
			item->widget()->setEnabled(state);
		}
	}
}
