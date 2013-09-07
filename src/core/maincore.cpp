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

#include <QtCore>
#include <QDebug>

#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrentRun>
#else
#include <QtConcurrentRun>
#endif

#include <taglib/taglib.h>
#include <taglib/fileref.h>

#include "maincore.hpp"
#include "sqlexception.hpp"
#include "utils.hpp"
#include "libraryinfo.hpp"
#include "db_engine/databaseengine.hpp"
#include "db_engine/directory.hpp"
#include "db_engine/episode.hpp"
#include "db_engine/fragment.hpp"
#include "sqlexception.hpp"
#include "musicplayer.hpp"

MainCore::MainCore(QObject *parent) :
	QObject(parent)
{
	player_ = new MusicPlayer(this);
	libraryModel_ = new LibraryModel(getDbEngine(), player_, this);
	proxyModel_ = new QSortFilterProxyModel(this);
	proxyModel_->setSortLocaleAware(true);
	proxyModel_->setDynamicSortFilter(true);

	proxyModel_->setSourceModel(libraryModel_);
}

void MainCore::loadDatabase()
{
	// -- check database --
	QString dbPath = settings_.dbPath();

	bool reCreate = !QFile(dbPath).exists();

	qDebug() << "Connecting to database in file " << dbPath;
	getDbEngine()->open(dbPath);

	if (reCreate) {
		qDebug() << "Database file does not exists, creating...";
		getDbEngine()->createAllTables();
		emit showMessage(QMessageBox::Information, tr("Database information"),
				  QString(tr("New database created.")));
	}

	getDbEngine()->fetchAllTables();

	try {
		libraryModel_->loadFromDatabase();
		emit loadDatabaseSuccess();
	} catch (const SQLException& e) {
		emit showMessage(QMessageBox::Critical, tr("Database error"),
				  QString(tr("Loading media library from database failed.\n"
							 "SQL error: %2")).arg(e.text()));
	}
}

void MainCore::addPodcastDirectory(const QString &path,
								   const Podcast::ptr& podcast)
{
	try {
		Directory::ptr dir = scanDir(path.toUtf8(), podcast);

		emit loadingProgress(tr("Updating media library..."), 100);

		QList<Fragment::ptr> newItems;

		for (const Episode::ptr& ep: dir->getEpisodesList()) {
			newItems << ep->getFragmentsList();
		}

		libraryModel_->addFragments(newItems);

		emit loadingFinished();

	} catch (const SQLException& e) {
		emit showMessage(QMessageBox::Critical, tr("Database error"),
						 QString(tr("Adding items failed.\n"
									"SQL error: %2")).arg(e.text()));
	}
}

/**
 * @brief scanDir Creates Directory entity in database and scans directory
 *  for supported audio files. Creates Episode for every readable audio file
 *  and start Fragment for it.
 * @param dir_path Path to directory on disk for scanning.
 * @param podcast If it's null smart pointer - associates (or creates if needed)
 *  Podcast entity with album name of file. If it's good smart pointer -
 *  assosciates every Episode with this Podcast.
 * @return Smart pointer for Directory entity associated with directory path.
 */
Directory::ptr MainCore::scanDir(const QString& dir_path, Podcast::ptr podcast)
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

	// if podcast not specified - generate for every file
	bool miscPodcasts = podcast.isNull();

	QDir qdir(dir_path);

	if (!qdir.exists()) {
		qFatal("Directory %s DOES NOT exists", qPrintable(qdir.path()));
		return Directory::ptr(nullptr);
	}

	Directory::ptr dir_model = Directory(dir_path).save();

	// -- scan files --
	QStringList filters;
	filters << "*.mp3";

	qdir.setNameFilters(filters);

	QFileInfoList files_list = qdir.entryInfoList();

	// TODO: make better progress informations

	emit loadingProgress(tr("Scanning files..."), 0);

	QList<Episode> episodesToAdd;

	for (const QFileInfo& fi: files_list) {

		const auto file_path = fi.absoluteFilePath().toUtf8();
		qDebug() << "reading file: " << file_path;


		if (!fi.isFile()) {
			qDebug() << "not a file: " << file_path;
			continue;
		}

		if (!fi.isReadable()) {
			qWarning() << "file not readable: " << file_path;
			continue;
		}

		// TODO: test UTF-8
		TagLib::FileRef f(file_path.data(), false);

		if (f.isNull()) {
			qWarning() << "file cannot be read by TagLib: " << file_path;
			continue; // skip file
		}

		if (!f.tag()) {
			qWarning() << "file tag cannot be read: " << file_path;
			continue; // skip file
		}

		QString title = generate_episode_name(f.tag());
		if (title.isEmpty()) title = fi.fileName();

		// -- if user not specified Podcast, try to get it from tags --
		if (miscPodcasts) {
			podcast.clear();

			// TODO: generate_podcast_name function

			QString album = taglib_qstring(f.tag()->album());

			// check if there is already Podcast of generated name in db
			for (const Podcast::ptr& pp: getDbEngine()->list<Podcast>()) {
				if (pp->getName() == album) {
					podcast = pp;
					break;
				}
			}

			if (podcast.isNull()) {
				podcast = Podcast(album).save();
			}
		}

		// -- adding episode to database --
		qDebug("adding episode to database: %s", qPrintable(title));

		episodesToAdd << Episode(fi.fileName(), title, podcast, dir_model);

	}

	emit loadingProgress(tr("Saving episodes information to database..."), 33);

//	check_error(qx::dao::update_with_all_relation(dir_model), "updating "
//				"directory model with episodes");

	Episode::ptr_list addedEpisodes = getDbEngine()->insertMultiple<Episode>(episodesToAdd);

	emit loadingProgress(tr("Creating and saving episodes starting points..."), 66);

	qDebug() << "adding start fragments to saved episodes";
	QList<Fragment> startFragmentsToAdd;
	for (const Episode::ptr& e: addedEpisodes) {
		startFragmentsToAdd << Fragment(e, 0, QString());
	}
	Fragment::ptr_list addedStartFragments = getDbEngine()->insertMultiple(startFragmentsToAdd);

	for (const Fragment::ptr fp: addedStartFragments) {
		fp->getEpisode()->setStartFragment(fp);
	}

	// update episodes info about start fragments
	getDbEngine()->updateMultiple<Episode>(addedEpisodes); // TODO: this can be done in background!

	emit loadingProgress(tr("Podcast directory loading done!"), 100);

	return dir_model;
}
