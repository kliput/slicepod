#include <QtCore>
#include <QxOrm.h>

#include "librarymodel.hpp"
#include "../core/libraryitem.hpp"
#include "../db_model.hpp"
#include "utils.hpp"

LibraryModel::LibraryModel(QObject *parent) :
	QAbstractTableModel(parent)
{
}

int LibraryModel::rowCount(const QModelIndex& /*parent*/) const
{
	return this->libraryItems_.size();
}

int LibraryModel::columnCount(const QModelIndex& /*parent*/) const
{
	return 6;
}

QVariant LibraryModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() > this->rowCount()
			|| index.column() >= this->columnCount()) {
		return QVariant();
	}


	if (role == Qt::DisplayRole) {
		const LibraryItem* item = libraryItems_[index.row()];

		switch (index.column()) {
		case 0:
			return item->podcastName();
		case 1:
			return item->episodeName();
		case 2:
			return item->fragmentTitle();
		case 3:
			return item->fragmentArtist();
		case 4:
			return QTime().addSecs(item->fragmentStart()).toString("HH:mm:ss");
		case 5:
			return item->fragmentTagsList().join(", ");
		default:
			return QVariant();
		}
	} else {
		return QVariant();
	}
}

QVariant LibraryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		switch (section) {
		case 0:
			return "Podcast";
		case 1:
			return "Episode";
		case 2:
			return "Title";
		case 3:
			return "Artist";
		case 4:
			return "Start";
		case 5:
			return "Tags";
		default:
			return QVariant();
		}
	} else {
		return QVariant();
	}
}


LibraryModel::~LibraryModel()
{
	qDeleteAll(this->libraryItems_);
	qDebug() << "Library model destructed";
}

void LibraryModel::addItem(LibraryItem *item)
{
	int first, last;
	first = last = libraryItems_.size();
	beginInsertRows(QModelIndex(), first, last);
	libraryItems_ << item;
	endInsertRows();
}

void LibraryModel::addItems(QList<LibraryItem *> itemsList)
{
	int first, last;
	first = libraryItems_.size();
	last = first + itemsList.size() - 1;

	beginInsertRows(QModelIndex(), first, last);
	libraryItems_ << itemsList;
	endInsertRows();
}

void LibraryModel::loadFromDatabase()
{
	Fragment::ptr_list fragments;

	static QStringList relations;
	if (relations.isEmpty()) {
		relations << (QStringList()
					  << db::field::fragment::EPISODE
					  << db::field::episode::PODCAST
					  ).join("->");
		relations << QString(db::field::fragment::TAGS_LIST);
	}

	check_error(qx::dao::fetch_all_with_relation(relations, fragments));

	QList<LibraryItem*> items;
	for (auto f: fragments) {
		items << new LibraryItem(f);
	}

	addItems(items);
}

