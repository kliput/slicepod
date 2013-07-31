#ifndef MAINCORE_HPP
#define MAINCORE_HPP

#include <QObject>
#include <QSortFilterProxyModel>
#include <QSettings>
#include <QMessageBox>

#include "librarymodel.hpp"
#include "settingsmanager.hpp"
#include "../db_model.hpp"

class MainCore : public QObject
{
	Q_OBJECT
public:
	explicit MainCore(QObject *parent = 0);

	void loadDatabase();
	void addPodcastDirectory(const QString& path,
							 const EntityType<Podcast>::ptr &podcast);
	
// -- getters --
	LibraryModel *libraryModel() { return libraryModel_; }
	QSortFilterProxyModel *proxyModel() { return proxyModel_; }

signals:
	void showMessage(QMessageBox::Icon icon, const QString& title,
				   const QString& text);
	void loadDatabaseSuccess();



private:
	LibraryModel *libraryModel_;
	QSortFilterProxyModel *proxyModel_;
	SettingsManager settings_;
};

#endif // MAINCORE_HPP