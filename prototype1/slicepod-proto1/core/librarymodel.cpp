#include <QtCore>
#include <QxOrm.h>

#include "librarymodel.hpp"
#include "../core/libraryitem.hpp"
#include "../db_model.hpp"
#include "utils.hpp"

LibraryModel::LibraryModel(QObject *parent) :
	QAbstractTableModel(parent)
{
	playImage_ = QImage(":/images/arrow-right-black.png");
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


	const LibraryItem* item = libraryItems_[index.row()];

	if (role == Qt::DisplayRole) {
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
			return QTime().addSecs(item->fragmentStartSec()).toString("HH:mm:ss");
		case 5:
			return item->fragmentTagsList().join(", ");
		default:
			return QVariant();
		}
	} else if (role == Qt::DecorationRole && index.column() == 0
			   && item->isPlaying()) {
		// TODO: react on column order changes
		return playImage_;
	} else {
		return QVariant();
	}
}

//! Gets LibraryItem* associated with given QModelIndex. Does range checking.
LibraryItem* LibraryModel::libraryItemData(const QModelIndex& index)
{
	if (!index.isValid() || index.row() > this->rowCount()
			|| index.column() >= this->columnCount()
			|| index.row() >= libraryItems_.size() || index.row() < 0) {
		qFatal("Tried to access invalid index from LibraryModel: %d, %d",
			   index.row(), index.column());
		return nullptr;
	}

	return libraryItems_[index.row()];
}

QVariant LibraryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		switch (section) {
		case 0:
			return tr("Podcast");
		case 1:
			return tr("Episode");
		case 2:
			return tr("Title");
		case 3:
			return tr("Artist");
		case 4:
			return tr("Start");
		case 5:
			return tr("Tags");
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

