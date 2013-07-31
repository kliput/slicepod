#include <QtCore>
#include <QDebug>
#include <QtConcurrentRun>
#include <QxtLogger>

#include "maincore.hpp"
#include "sqlexception.hpp"
#include "utils.hpp"
#include "libraryitem.hpp"
#include "../db_model.hpp"

MainCore::MainCore(QObject *parent) :
	QObject(parent)
{
	libraryModel_ = new LibraryModel(this);
	proxyModel_ = new QSortFilterProxyModel(this);

	proxyModel_->setSourceModel(libraryModel_);
//	proxyModel_->setFilterKeyColumn(5);
}

void MainCore::loadDatabase()
{
	// -- check database --
	QString dbPath = settings_.dbPath();
	QFile dbFile(dbPath);
	if (!dbFile.exists()) {
		qxtLog->info() << "Database file does not exists, creating...";
		db_connect(dbPath); // creates file if does not exists
		db_create_tables(); // NOTICE: there are no errors here
		emit showMessage(QMessageBox::Information, tr("Database information"),
				  QString(tr("New database created.")));
	} else {
		qxtLog->debug() << "Connecting to database in file " << dbPath;
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
	auto dir = scan_dir(path.toUtf8(), podcast);

	QList<LibraryItem*> newItems;

	for (auto ep: dir->episodes_list) {
		for (auto fr: ep->fragments_list) {
			newItems << new LibraryItem(fr);
		}
	}

	libraryModel_->addItems(newItems);
}
