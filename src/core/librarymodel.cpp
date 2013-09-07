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
#include <QIcon>
#include <QImage>

#include "librarymodel.hpp"
#include "core/libraryinfo.hpp"
#include "db_engine/databaseengine.hpp"
#include "db_engine/fragment.hpp"
#include "utils.hpp"
#include "musicplayer.hpp"

LibraryModel::LibraryModel(DatabaseEngine* dbEngine, MusicPlayer* musicPlayer, QObject* parent) :
	QAbstractTableModel(parent),
	dbEngine_(dbEngine),
	player_(musicPlayer)
{
	playImage_ = QImage(":/images/arrow-right-black.png");
	startFragmentIcon_ = QIcon::fromTheme("flag-green");

	//Q_ASSERT(!startFragmentIcon_.isNull()); // TODO: bundle icon theme
	Q_ASSERT(!playImage_.isNull());
}

int LibraryModel::rowCount(const QModelIndex& /*parent*/) const
{
	return getFragmentsList().size();
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


	const Fragment::ptr& fragment = getFragmentsList()[index.row()];

	if (role == Qt::DisplayRole) {
		switch (index.column()) {
		case COL_PODCAST:
			return fragment->getEpisode()->getPodcast()->getName();
		case COL_EPISODE:
			return fragment->getEpisode()->getEpisodeName();
		case COL_TITLE:
			if (fragment->isStartFragment()) {
				return tr("Beginning");
			} else {
				return fragment->getTitle();
			}
		case COL_ARTIST:
			return fragment->getArtist();
		case COL_START:
			return fragment->getLibraryInfo()->getFragmentStartTime().toString("HH:mm:ss");
		case COL_TAGS:
			return fragment->getLibraryInfo()->getTagsString();
		default:
			return QVariant();
		}
		// TODO: play icon
//	} else if (role == Qt::DecorationRole && index.column() == 0
//			   && player_ && player_->currentItem() == item) {
//		// TODO: react on column order changes
//		return playImage_;
	} else if (role == Qt::DecorationRole && index.column() == COL_TITLE
			   && fragment->isStartFragment()) {
		return startFragmentIcon_;
	} else {
		return QVariant();
	}
}

//! Gets LibraryItem* associated with given QModelIndex. Does range checking.
Fragment::ptr LibraryModel::getFragmentData(const QModelIndex& index)
{
	if (!index.isValid() || index.row() > this->rowCount()
			|| index.column() >= this->columnCount()
			|| index.row() >= getFragmentsList().size() || index.row() < 0) {
		qFatal("Tried to access invalid index from LibraryModel: %d, %d",
			   index.row(), index.column());

		return Fragment::ptr();
	}

	return getFragmentsList()[index.row()];
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
	qDebug() << "Library model destructed";
}

void LibraryModel::addFragment(Fragment::ptr fragment)
{
	int first, last;
	first = last = getFragmentsList().size();
	beginInsertRows(QModelIndex(), first, last);
	fragmentsList_ << fragment;
	endInsertRows();
}

//! Add items (rows) to list model.
void LibraryModel::addFragments(QList<Fragment::ptr> fragmentsList)
{
	if (fragmentsList.size() == 0) {
		return;
	}

	int first, last;
	first = this->getFragmentsList().size(); // last element index + 1
	// prevent first > last if libraryItems_.size() == 0 TODO
//	last = fragmentsList.size() == 0 ? first : first + fragmentsList.size() - 1;
	last = first + fragmentsList.size() - 1;

	beginInsertRows(QModelIndex(), first, last);
	fragmentsList_ << fragmentsList;
	endInsertRows();
}

void LibraryModel::loadFromDatabase()
{
	addFragments(dbEngine_->template list<Fragment>());
}

