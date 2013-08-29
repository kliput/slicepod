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

#ifndef LIBRARYMODEL_HPP
#define LIBRARYMODEL_HPP

#include <QAbstractTableModel>
#include <QList>
#include <QImage>
#include <QIcon>

class LibraryItem;
class MusicPlayer;
class DatabaseEngine;

class LibraryModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	explicit LibraryModel(DatabaseEngine *dbEngine, MusicPlayer* musicPlayer, QObject *parent = 0);
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	virtual ~LibraryModel();

	const QList<LibraryItem*> libraryItems() { return libraryItems_; }

	void addItem(LibraryItem *item);
	void addItems(QList<LibraryItem*> itemsList);

	void loadFromDatabase();

	LibraryItem *libraryItemData(const QModelIndex &index);

	enum Column {
		COL_PODCAST = 0,
		COL_EPISODE = 1,
		COL_TITLE = 2,
		COL_ARTIST = 3,
		COL_START = 4,
		COL_TAGS = 5
	};

	static constexpr const int COLUMN_COUNT = 6;

signals:

public slots:

private:
	QList<LibraryItem*> libraryItems_;
	DatabaseEngine* dbEngine_;
	MusicPlayer* player_;
	QImage playImage_;
	QIcon startFragmentIcon_;
};

#endif // LIBRARYMODEL_HPP
