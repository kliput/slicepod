#ifndef MAINCORE_HPP
#define MAINCORE_HPP

#include <QObject>
#include <QSortFilterProxyModel>
#include <QSettings>
#include <QMessageBox>

#include "librarymodel.hpp"
#include "settingsmanager.hpp"
#include "db_engine/databaseengine.hpp"
#include "db_engine/podcast.hpp"
#include "db_engine/directory.hpp"
#include "musicplayer.hpp"

class MainCore : public QObject
{
	Q_OBJECT
public:
	explicit MainCore(QObject *parent = 0);

	void loadDatabase();
	void addPodcastDirectory(const QString& path,
							 const Podcast::ptr &podcast);
	Directory::ptr scanDir(const QString &dir_path, Podcast::ptr podcast
						   = Podcast::ptr());
	
// -- getters --
	LibraryModel *libraryModel() { return libraryModel_; }
	QSortFilterProxyModel *proxyModel() { return proxyModel_; }
	MusicPlayer *player() { return player_; }

signals:
	void showMessage(QMessageBox::Icon icon, const QString& title,
				   const QString& text);
	void loadDatabaseSuccess();
	void loadingProgress(const QString& message, const int percent);
	void loadingFinished();

private:
	LibraryModel *libraryModel_;
	QSortFilterProxyModel *proxyModel_;
	SettingsManager settings_;
	MusicPlayer *player_;
	DatabaseEngine *db_;
};

#endif // MAINCORE_HPP