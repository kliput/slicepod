#include <QtCore>
#include <QIcon>
#include <QImage>

#include "librarymodel.hpp"
#include "core/libraryitem.hpp"
#include "db_engine/databaseengine.hpp"
#include "db_engine/fragment.hpp"
#include "utils.hpp"
#include "musicplayer.hpp"

LibraryModel::LibraryModel(MusicPlayer *musicPlayer, QObject *parent) :
	QAbstractTableModel(parent),
	player_(musicPlayer)
{
	playImage_ = QImage(":/images/arrow-right-black.png");
	startFragmentIcon_ = QIcon::fromTheme("flag-green");

	//Q_ASSERT(!startFragmentIcon_.isNull()); // TODO: bundle icon theme
	Q_ASSERT(!playImage_.isNull());
}

int LibraryModel::rowCount(const QModelIndex& /*parent*/) const
{
	return this->libraryItems_.size();
}

int LibraryModel::columnCount(const QModelIndex& /*parent*/) const
{
	return COLUMN_COUNT;
}

QVariant LibraryModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() > this->rowCount()
			|| index.column() >= this->columnCount()) {
		return QVariant();
	}


	LibraryItem* const item = libraryItems_[index.row()];

	if (role == Qt::DisplayRole) {
		switch (index.column()) {
		case COL_PODCAST:
			return item->podcastName();
		case COL_EPISODE:
			return item->episodeName();
		case COL_TITLE:
			if (item->isStartFragment()) {
				return tr("Beginning");
			} else {
				return item->fragmentTitle();
			}
		case COL_ARTIST:
			return item->fragmentArtist();
		case COL_START:
			return QTime().addSecs(item->fragmentStartSec()).toString("HH:mm:ss");
		case COL_TAGS:
			return item->fragmentTagsList().join(", ");
		default:
			return QVariant();
		}
		// TODO: play icon
//	} else if (role == Qt::DecorationRole && index.column() == 0
//			   && player_ && player_->currentItem() == item) {
//		// TODO: react on column order changes
//		return playImage_;
	} else if (role == Qt::DecorationRole && index.column() == COL_TITLE
			   && item->isStartFragment()) {
		return startFragmentIcon_;
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
		case COL_PODCAST:
			return tr("Podcast");
		case COL_EPISODE:
			return tr("Episode");
		case COL_TITLE:
			return tr("Title");
		case COL_ARTIST:
			return tr("Artist");
		case COL_START:
			return tr("Start");
		case COL_TAGS:
			return tr("Tags");
		default:
			return QVariant();
		}
//	} else if (role == Qt::DecorationRole && section == 0) {

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

//! Add items (rows) to list model.
void LibraryModel::addItems(QList<LibraryItem *> itemsList)
{
	int first, last;
	first = libraryItems_.size();
	// prevent first > last if libraryItems_.size() < 0
	last = itemsList.size() == 0 ? first : first + itemsList.size() - 1;

	beginInsertRows(QModelIndex(), first, last);
	libraryItems_ << itemsList;
	endInsertRows();
}

//! Creates list of LibraryItems constructed with all fragments from database
//! engine cache. Global DatabaseEngine instance is used.
//! NOTICE: Fragments must be previously loaded into engine's cache.
void LibraryModel::loadFromDatabase()
{
	QList<LibraryItem*> items;
	for (const Fragment::ptr& f: db::global_engine()->list<Fragment>()) {
		items << new LibraryItem(f);
	}

	addItems(items);
}

