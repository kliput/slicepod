#include "precompiled.hpp"

#include "core/utils.hpp"
#include "db_model.hpp"
#include "core/sqlexception.hpp"

#include <cstdlib>
#include <iostream>
#include <taglib/tag.h>
#include <taglib/fileref.h>

#include <qxt/QxtCore/QxtLogger>

/**
 * @brief Creates global connection to SQLite database with given path.
 * @param db_name SQLite3 database file path
 */
void db_connect(const char* db_name)
{
	qx::QxSqlDatabase* db = qx::QxSqlDatabase::getSingleton();

	// Parameters to connect to database
	db->setDriverName("QSQLITE");
	db->setDatabaseName(db_name);
	db->setHostName("localhost");
	db->setUserName("root");
	db->setPassword("");

	// Only for debug purpose: assert if invalid offset detected fetching a relation
	qx::QxSqlDatabase::getSingleton()->setVerifyOffsetRelation(true);
}

/**
 * @brief Checks if SQL error is present. If it is - throws SQLException
 *	with QSqlError object. Otherwise does nothing.
 * @param error QSqlError object to check if it contains error.
 * @param info QString with information about circumstances in which error
 *	occurs
 */
inline void check_error(const QSqlError& error, const char* info = "")
{
	if (error.isValid()) {
		throw SQLException(error, info);
	}
}

/**
 * @brief Scans directory with given path for music files to create episode
 *  objects. Creates and automatically persists one Directory object and
 *	Episodes and associated start Fragments for each valid music file.
 * @param dir_path
 * @param podcast
 * @return Smart pointer to persisted Directory for given path or null smart
 *	pointer on failure.
 * @throw SQLException on persistence failure
 */
QSharedPointer<Directory> scan_dir(const char* dir_path,
										 QSharedPointer<Podcast> podcast)
{
	/* Procedure:
	 * - check path and create directory entity
	 * - scan directory and create episodes entities:
	 *  - filename <- file name
	 *  - episode_name <- mp3 tag title
	 *  - podcast_ptr <- try to compare with album name (TODO - other fun.)
	 *  - compare_data <- TODO
	 *  - metadata <- TODO
	 */

	// -- check path and create directory model --

	QDir qdir(dir_path);

	if (!qdir.exists()) {
		qxtLog->error("Directory ", qdir.path(),"DOES NOT exists");
		return directory_ptr(nullptr);
	}

	directory_ptr dir_model(new Directory(dir_path));
	check_error(qx::dao::save(dir_model), "saving first directory model");

	// -- scan files --
	QStringList filters;
	filters << "*.mp3";

	qdir.setNameFilters(filters);

	QFileInfoList files_list = qdir.entryInfoList();
	for (QFileInfo fi: files_list) {
		const char* file_name = QFile::encodeName(fi.absoluteFilePath());
		qxtLog->debug("reading file: ", file_name);

		if (!fi.isFile()) {
			qxtLog->debug("not a file: ", file_name);
			continue;
		}

		if (!fi.isReadable()) {
			qxtLog->info("file not readable: ", file_name);
			continue;
		}

		TagLib::FileRef f(file_name, false);

		if (f.isNull()) {
			qxtLog->warning("file cannot be read by TagLib: ", file_name);
			continue;
		}

		if (!f.tag()) {
			qxtLog->info("file tag cannot be read: ", file_name);
			continue;
		}

		QString title = f.tag()->title().toCString();
		if (title == "") title = fi.fileName();

		// -- adding episode to database --
		qxtLog->trace("adding episode: ", title);
		episode_ptr ep_model(new Episode(fi.fileName(), title,
										 dir_model, podcast));
		dir_model->episodes_list << ep_model;
	}

	check_error(qx::dao::update_with_all_relation(dir_model), "updating "
				"directory model with episodes");

	qxtLog->debug() << "adding start fragments to saved episodes:";
	for (const episode_ptr& e: dir_model->episodes_list) {
		qxtLog->debug("id: ", (qlonglong) e->id, ", name: ", e->episode_name);
		add_start_fragment(e);
	}

	return dir_model;
}

// TODO: check if already exists
fragment_ptr add_start_fragment(const episode_ptr& episode)
{
	fragment_ptr fragment(new Fragment(episode, 0));
	episode->start_fragment = fragment;
	check_error(qx::dao::update_with_relation(
					db_fields::episode::START_FRAGMENT, episode));
	return fragment;
}

