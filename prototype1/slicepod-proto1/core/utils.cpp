#include "precompiled.hpp"

#include "core/utils.hpp"
#include "core/sqlexception.hpp"
#include "db_model.hpp"
using namespace db::type;

#include <cstdlib>
#include <iostream>
#include <taglib/tag.h>
#include <taglib/fileref.h>

#include <qxt/QxtCore/QxtLogger>

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
inline void check_error(const QSqlError& error, const char* info)
{
	if (error.isValid()) {
		throw SQLException(error, info);
	}
}

// -- MODEL FUNCTIONS SECTION --

/**
 * @brief scan_dir Creates Directory entity in database and scans directory
 *  for supported audio files. Creates Episode for every readable audio file
 *  and start Fragment for it.
 * @param dir_path Path to directory on disk for scanning.
 * @param podcast If it's null smart pointer - associates (or creates if needed)
 *  Podcast entity with album name of file. If it's good smart pointer -
 *  assosciates every Episode with this Podcast.
 * @return Smart pointer for Directory entity associated with directory path.
 */
Directory::ptr scan_dir(const char* dir_path, Podcast::ptr podcast)
{
	/* Procedure:
	 * - check path and create directory entity
	 * - scan directory and create episodes entities:
	 *  - filename <- file name (without directory path)
	 *  - episode_name <- mp3 tag title
	 *  - podcast_ptr <- try to create for album name or associate
	 *  - compare_data <- TODO
	 *  - metadata <- TODO
	 */

	// -- check path and create directory model --

	QDir qdir(dir_path);

	if (!qdir.exists()) {
		qxtLog->error("Directory ", qdir.path(),"DOES NOT exists");
		return Directory::ptr(nullptr);
	}

	Directory::ptr dir_model(new Directory(dir_path));
	check_error(qx::dao::save(dir_model), "saving first directory model");

	// -- scan files --
	QStringList filters;
	filters << "*.mp3";

	qdir.setNameFilters(filters);

	QFileInfoList files_list = qdir.entryInfoList();
	for (const QFileInfo& fi: files_list) {
		const auto file_path = fi.absoluteFilePath().toUtf8();
		qxtLog->debug() << "reading file: " << file_path;

		if (!fi.isFile()) {
			qxtLog->debug() << "not a file: " << file_path;
			continue;
		}

		if (!fi.isReadable()) {
			qxtLog->warning() << "file not readable: " << file_path;
			continue;
		}

		// TODO: test UTF-8
		TagLib::FileRef f(file_path, false);

		if (f.isNull()) {
			qxtLog->warning() << "file cannot be read by TagLib: " << file_path;
			continue; // skip file
		}

		if (!f.tag()) {
			qxtLog->info() << "file tag cannot be read: " << file_path;
			continue; // skip file
		}

		QString title = QString::fromUtf8(f.tag()->title().toCString(true));
		if (title == "") title = fi.fileName();

		// -- if user not specified Podcast, try to get it from tags --
		if (podcast.isNull()) {

			QString album = QString::fromUtf8(f.tag()->album().toCString(true));

			qx::QxSqlQuery pod_query;
			// TODO: ignore case, etc.
			pod_query.where(db::field::podcast::NAME).isEqualTo(album);
			Podcast::ptr_list podcasts_list;
			check_error(qx::dao::fetch_by_query(pod_query, podcasts_list));

			if (podcasts_list.size() > 0) {
				// TODO behaviour on size > 1
				podcast = podcasts_list[0];
			} else {
				podcast = Podcast::ptr(new Podcast(album));
				check_error(qx::dao::save(podcast));
			}
		}

		// -- adding episode to database --
		qxtLog->trace("adding episode: ", title);
		Episode::ptr ep_model(new Episode(fi.fileName(), title,
										 dir_model, podcast));
		dir_model->episodesList << ep_model;
	}

	check_error(qx::dao::update_with_all_relation(dir_model), "updating "
				"directory model with episodes");

	qxtLog->debug() << "adding start fragments to saved episodes:";
	for (const Episode::ptr& e: dir_model->episodesList) {
		qxtLog->debug("id: ", (qlonglong) e->id, ", name: ", e->episodeName);
		add_start_fragment(e);
	}

	return dir_model;
}


// TODO: check if already exists
Fragment::ptr add_start_fragment(const Episode::ptr &episode)
{
	// WARNING: it's only check for fetched start fragment
	if (episode->startFragment.isNull()) {
		Fragment::ptr fragment(new Fragment(episode, 0));
		episode->startFragment = fragment;
		episode->fragmentsList << fragment;
		// TODO: relations should be static
		check_error(qx::dao::update_with_relation(
						QStringList()
						<< db::field::episode::START_FRAGMENT
						<< db::field::episode::FRAGMENTS_LIST
						, episode));
		return fragment;
	} else {
		return episode->startFragment;
	}
}

