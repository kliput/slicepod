#ifndef MAINCORE_HPP
#define MAINCORE_HPP

#include <QObject>
#include <QSortFilterProxyModel>
#include <QSettings>
#include <QMessageBox>

#include "librarymodel.hpp"
#include "settingsmanager.hpp"
#include "../db_model.hpp"
#include "vlcplayer.hpp"

class MainCore : public QObject
{
	Q_OBJECT
public:
	explicit MainCore(QObject *parent = 0);

	void loadDatabase();
	void addPodcastDirectory(const QString& path,
							 const EntityType<Podcast>::ptr &podcast);
	Directory::ptr scanDir(const char* dir_path, Podcast::ptr podcast
						   = Podcast::ptr());
	
// -- getters --
	LibraryModel *libraryModel() { return libraryModel_; }
	QSortFilterProxyModel *proxyModel() { return proxyModel_; }
	VLCPlayer *player() { return player_; }

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
	VLCPlayer *player_;
};

#endif // MAINCORE_HPP
