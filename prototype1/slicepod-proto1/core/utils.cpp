#include "precompiled.hpp"

#include "core/utils.hpp"
#include "db_model.hpp"

#include <cstdlib>
#include <iostream>
#include <boost/format.hpp>
#include <taglib/tag.h>
#include <taglib/fileref.h>

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

void check(const QSqlError& error)
{
	if (error.isValid()) {
		qDebug() << QString("SQL error: %1").arg(error.text());
		exit(1);
	}
}

directory_ptr add_scan_dir(const char* dir_path)
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

	if (qdir.exists()) {
		qDebug() << QString("Directory %1 exists, listing files:").arg(qdir.path());
	} else {
		qDebug() << QString("Directory %1 DOES NOT exists").arg(qdir.path());
		return directory_ptr(0);
	}

	directory_ptr dir_model(new Directory(dir_path));
	qx::dao::save(dir_model);

	// -- dummy podcast TODO --

	podcast_ptr podcast_model(new Podcast("Sample Podcast"));
	qx::dao::save(podcast_model);

	// -- scan files --

	QStringList filters;
	filters << "*.mp3";

	qdir.setNameFilters(filters);

	QFileInfoList files_list = qdir.entryInfoList();
	for (QFileInfo fi: files_list) {
		const char* file_name = QFile::encodeName(fi.absoluteFilePath());
		LOG_DEBUG("reading file: " << file_name);
		
		
		if (!fi.isFile()) {
			LOG_DEBUG("not a file: " << file_name);
			continue;
		}

		if (!fi.isReadable()) {
			LOG_INFO("file not readable: " << file_name);
			continue;
		}

		std::cout << "fn1: " << file_name << std::endl;
		TagLib::FileRef f(file_name);
		std::cout << "fn2: " << file_name << std::endl;
		if (f.isNull()) {
			LOG_INFO("file cannot be read by TagLib: " << file_name);
			continue;
		}

		if (!f.tag()) {
			LOG_INFO("file tag cannot be read: " << file_name);
			continue;
		}

		std::string title = f.tag()->title().toCString();
		if (title == "") title = fi.fileName().toStdString();
		
		// TODO add podcast name (album name) if not exists in database
		
		// -- adding episode do database --
		LOG_INFO(boost::format(">> adding >> %1%") % title);
		episode_ptr ep_model(new Episode(fi.fileName(), title.c_str(), dir_model, podcast_model));
		dir_model->episodes_list.push_back(ep_model);
//		qx::dao::save(ep_model);
	}

	qx::dao::save_with_all_relation(dir_model);

	return directory_ptr(new Directory(dir_path));
}
