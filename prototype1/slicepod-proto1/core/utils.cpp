#include "precompiled.hpp"

#include "core/utils.hpp"
#include "db_model.hpp"

#include <cstdlib>
#include <iostream>
#include <taglib/tag.h>
#include <taglib/fileref.h>

#include <qxt/QxtCore/QxtLogger>

void db_connect(const char* db_name)
{
	qx::QxSqlDatabase* db = qx::QxSqlDatabase::getSingleton();

	// Parameters to connect to database
	db->setDriverName("QSQLITE");
	db->setDatabaseName(db_name);
	db->setHostName("localhost");
	db->setUserName("root");
	db->setPassword("");

	// Only for debug purpose : assert if invalid offset detected fetching a relation
	qx::QxSqlDatabase::getSingleton()->setVerifyOffsetRelation(true);
}

inline void check_error(const QSqlError& error)
{
	if (error.isValid()) {
		qxtLog->error("SQL error: ", error.text());
	}
}


QSharedPointer<Directory> scan_dir(const char* dir_path,
										 QSharedPointer<Podcast> podcast)
{
	/*
	 * - check path and create directory entity
	 * - scan directory and create episodes entities:
	 *  - filename <- file name
	 *  - episode_name <- mp3 tag title
	 *  - podcast_ptr <- try to compare with album name (TODO)
	 *  - compare_data <- TODO
	 *  - metadata <- ?
	 */

	// -- check path and create directory model --

	QDir qdir(dir_path);

	if (!qdir.exists()) {
		qxtLog->error("Directory ", qdir.path(),"DOES NOT exists");
		return directory_ptr(nullptr);
	}

	directory_ptr dir_model(new Directory(dir_path));
	check_error(qx::dao::save(dir_model));

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

	check_error(qx::dao::save_with_all_relation(dir_model));

	qxtLog->debug() << "saved episodes:";
	for (const episode_ptr& e: dir_model->episodes_list) {
		qxtLog->debug("id: ", (qlonglong) e->id, ", name: ", e->episode_name);
		add_start_fragment(e);
	}

	return dir_model;
}

fragment_ptr add_start_fragment(const episode_ptr& episode)
{
	fragment_ptr fragment(new Fragment(episode, 0));
	episode->start_fragment = fragment;
	check_error(qx::dao::update_with_relation("full_fragment_id", episode));
	return fragment;
}

//episode_ptr create_episode(const QString& _file_name, const QString& _episode_name,
//						   const QSharedPointer<Directory>& _directory,
//						   const QSharedPointer<Podcast>& _podcast)
//{
//	auto episode = new Episode(_file_name, _episode_name, _directory, _podcast);
//	qx::dao::save(episode);
//	qxtLog->debug() << "saved episode with id: " << episode->id;
//	return episode_ptr(episode);
//}
