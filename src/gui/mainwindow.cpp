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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QtGui>
#include <QWidget>
#include <QFutureWatcher>
#include <QMetaType>

#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrentRun>
#else
#include <QtConcurrentRun>
#endif

#include "adddirectorydialog.hpp"

Q_DECLARE_METATYPE(QMessageBox::Icon)

// TODO: create LibraryManager class to manage library (move methods from here)
#include "core/utils.hpp"
#include "core/libraryinfo.hpp"
#include "core/settingsmanager.hpp"

#include "core/librarymodel.hpp"
#include <QSortFilterProxyModel>

MainWindow::MainWindow(MainCore* core, QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	core_(core)
{
	ui->setupUi(this);

	ui->libraryView->setModel(core_->getLibraryProxyModel());
	ui->libraryView->setVisible(true);

	ui->positionWidget->setMusicPlayer(core_->getMusicPlayer());

	// CONNECT
	qRegisterMetaType<QMessageBox::Icon>();

	connect(ui->actionAdd_directory, SIGNAL(triggered()),
			this, SLOT(addDirectoryDialog()));

	connect(core_, SIGNAL(loadDatabaseSuccess()),
					 this, SLOT(loadDatabaseSuccess()));

	connect(core_, SIGNAL(showMessage(QMessageBox::Icon,QString,QString)),
					 this, SLOT(showMessage(QMessageBox::Icon,QString,QString)));

	// -- library item info panel --
	connect(ui->libraryView->selectionModel(),
			SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
			this, SLOT(updateFragmentEditorView(QModelIndex,QModelIndex)));

//	connect(ui->editorEndTimeCheckBox, SIGNAL(toggled(bool)),
//			ui->editorEndTimeEdit, SLOT(setEnabled(bool)));

	// add new fragment

	connect(core_, SIGNAL(fragmentCreated(int)),
			this, SLOT(handleFragmentCreated(int)));
	connect(ui->addFragmentButton, SIGNAL(clicked()),
			this, SLOT(handleAddFragmentButton()));

	connect(ui->libraryView, SIGNAL(activated(QModelIndex)),
			this, SLOT(activateLibraryItem(QModelIndex)));

	// -- player controls --
	connect(ui->playButton, SIGNAL(clicked()),
			this, SLOT(handlePlayButton()));

//	connect(core_->player(), SIGNAL(positionChanged(int)),
//			ui->positionWidget, SLOT(setPlayerPosition(int)));

	// START ACTIONS
	ui->libraryView->setDisabled(true); // before loading from database
	QtConcurrent::run(core_, &MainCore::loadDatabase);

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::addDirectoryDialog()
{
	auto dialog = new AddDirectoryDialog(core_, this);
	dialog->show();
	dialog->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::updateFragmentEditorView(const QModelIndex& current,
									const QModelIndex& /*prev*/)
{
	auto fragment = core_->getLibraryModel()->getFragmentData(
				core_->getLibraryProxyModel()->mapToSource(current));

	ui->fragmentEditorWidget->setFragment(fragment);
}

void MainWindow::activateLibraryItem(const QModelIndex &index)
{
	Fragment::ptr fragment = core_->getLibraryModel()->getFragmentData(
				core_->getLibraryProxyModel()->mapToSource(index));

	if (core_->getMusicPlayer()->loadFragment(fragment)) {
		core_->getMusicPlayer()->scheduleTimeChange(fragment->getStart());
		core_->getMusicPlayer()->play();
		// TODO: emit data changed on played item to show play icon
	} else {
		qFatal("Cannot load media into player!");
		// TODO: handle
	}
}

void MainWindow::showMessage(QMessageBox::Icon icon, const QString& title,
						   const QString& text)
{
	QMessageBox(icon, title, text).exec();
}

void MainWindow::loadDatabaseSuccess()
{
	ui->libraryView->setDisabled(false);
	qDebug("Load database success");
}

void MainWindow::handlePlayButton()
{
	
}

void MainWindow::handleAddFragmentButton()
{
	QtConcurrent::run(core_, &MainCore::createFragment,
					  core_->getMusicPlayer()->getCurrentFragment()->getEpisode(),
					  core_->getMusicPlayer()->getPositionMs());
}

void MainWindow::handleFragmentCreated(int row)
{
	auto rowIndexes = core_->getLibraryModel()->createIndexesForRow(row);

	ui->libraryView->clearSelection();

	for (const auto& index: rowIndexes) {
		ui->libraryView->selectionModel()->setCurrentIndex(
					core_->getLibraryProxyModel()->mapFromSource(index),
					QItemSelectionModel::Select);
	}

}
