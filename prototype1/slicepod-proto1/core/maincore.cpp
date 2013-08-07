#include <QtCore>
#include <QDebug>
#include <QtConcurrentRun>

#include <taglib/taglib.h>
#include <taglib/fileref.h>

#include "maincore.hpp"
#include "sqlexception.hpp"
#include "utils.hpp"
#include "libraryitem.hpp"
#include "../db_model.hpp"
#include "sqlexception.hpp"
#include "musicplayer.hpp"

MainCore::MainCore(QObject *parent) :
	QObject(parent)
{
	libraryModel_ = new LibraryModel(this);
	proxyModel_ = new QSortFilterProxyModel(this);
	proxyModel_->setSourceModel(libraryModel_);

	player_ = new MusicPlayer(this);
}

void MainCore::loadDatabase()
{
	// -- check database --
	QString dbPath = settings_.dbPath();
	QFile dbFile(dbPath);
	if (!dbFile.exists()) {
		qDebug() << "Database file does not exists, creating...";
		db_connect(dbPath); // creates file if does not exists
		db_create_tables(); // NOTICE: there are no errors here
		emit showMessage(QMessageBox::Information, tr("Database information"),
				  QString(tr("New database created.")));
	} else {
		qDebug() << "Connecting to database in file " << dbPath;
		// TODO: validate, if not valid, ask user to recreate database
		db_connect(dbPath);
	}




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
								   const EntityType<Podcast>::ptr& podcast)
{
	try {
		auto dir = scanDir(path.toUtf8(), podcast);

		emit loadingProgress(tr("Updating media library..."), 100);

		QList<LibraryItem*> newItems;

		for (auto ep: dir->episodesList) {
			for (auto fr: ep->fragmentsList) {
				newItems << new LibraryItem(fr);
			}
		}

		libraryModel_->addItems(newItems);

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
Directory::ptr MainCore::scanDir(const char* dir_path, Podcast::ptr podcast)
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
		qFatal("Directory %s DOES NOT exists", qPrintable(qdir.path()));
		return Directory::ptr(nullptr);
	}

	Directory::ptr dir_model(new Directory(dir_path));
	check_error(qx::dao::save(dir_model), "saving first directory model");

	// -- scan files --
	QStringList filters;
	filters << "*.mp3";

	qdir.setNameFilters(filters);

	QFileInfoList files_list = qdir.entryInfoList();

	// TODO: make better progress informations

	emit loadingProgress(tr("Scanning files..."), 0);

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
		TagLib::FileRef f(file_path, false);

		if (f.isNull()) {
			qWarning() << "file cannot be read by TagLib: " << file_path;
			continue; // skip file
		}

		if (!f.tag()) {
			qWarning() << "file tag cannot be read: " << file_path;
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
		qDebug("adding episode to database: %s", qPrintable(title));
		Episode::ptr ep_model(new Episode(fi.fileName(), title,
										 dir_model, podcast));
		dir_model->episodesList << ep_model;
	}

	emit loadingProgress(tr("Saving episodes information to database..."), 33);

	check_error(qx::dao::update_with_all_relation(dir_model), "updating "
				"directory model with episodes");

	emit loadingProgress(tr("Creating and saving start fragments..."), 66);

	qDebug() << "adding start fragments to saved episodes:";
	for (const Episode::ptr& e: dir_model->episodesList) {

		qDebug("id: %ld, name: %s", (long) e->id, qPrintable(e->episodeName));

		create_start_fragment(e);
	}

	// TODO: relations should be static
	check_error(qx::dao::update_with_relation(
					QStringList()
					<< db::field::episode::START_FRAGMENT
					<< db::field::episode::FRAGMENTS_LIST,
					dir_model->episodesList));

	emit loadingProgress(tr("Podcast directory loading done!"), 100);

	return dir_model;
}
