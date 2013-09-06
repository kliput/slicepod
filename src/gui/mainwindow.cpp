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

	ui->libraryView->setModel(core_->proxyModel());
	ui->libraryView->setVisible(true);

	ui->positionWidget->setMusicPlayer(core_->musicPlayer());

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
			this, SLOT(updateItemInfoView(QModelIndex,QModelIndex)));

	connect(ui->endTimeCheckBox, SIGNAL(toggled(bool)),
			ui->endTimeEdit, SLOT(setEnabled(bool)));

	// set disabled
	fillFragmentInfoView();

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

void MainWindow::fillFragmentInfoView(const Fragment::ptr fragment)
{
	if (!fragment) {
		setSelectedFragmentPanelEnabled(false);
	} else {
		setSelectedFragmentPanelEnabled(true);

		ui->selectedPodcastLabel->setText(fragment->getEpisode()->getPodcast()->getName());
		ui->selectedEpisodeLabel->setText(fragment->getEpisode()->getEpisodeName());

		// TODO: cached time
		ui->startTimeEdit->setTimeRange(QTime(), fragment->getLibraryInfo()->getFragmentEndTime());
		ui->startTimeEdit->setTime(fragment->getLibraryInfo()->getFragmentStartTime());

		if (fragment->hasEnd()) {
			ui->endTimeEdit->setTimeRange(QTime(), fragment->getLibraryInfo()->getEpisodeLengthTime());
			ui->endTimeEdit->setTime(fragment->getLibraryInfo()->getFragmentEndTime());
			ui->endTimeCheckBox->setChecked(true);
			ui->endTimeEdit->setEnabled(true);
		} else {
			ui->endTimeCheckBox->setChecked(false);
			ui->endTimeEdit->setEnabled(false);
		}
	}
}

void MainWindow::updateItemInfoView(const QModelIndex& current,
									const QModelIndex& /*prev*/)
{
	auto item = core_->libraryModel()->getFragmentData(current);

	fillFragmentInfoView(item);

}

void MainWindow::activateLibraryItem(const QModelIndex &index)
{
	Fragment::ptr fragment = core_->libraryModel()->getFragmentData(index);

	if (core_->musicPlayer()->loadFragment(fragment)) {
		ui->positionWidget->setCurrentFragment(fragment);
		core_->musicPlayer()->scheduleTimeChange(fragment->getStart());
		core_->musicPlayer()->play();
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

void MainWindow::setSelectedFragmentPanelEnabled(bool state)
{
	static QWidget* fragmentPanelElements[] = {
		ui->endTimeCheckBox,
		ui->endTimeEdit,
		ui->startTimeEdit,
		ui->startTimeLabel,
		ui->selectedEpisodeLabel,
		ui->selectedEpisodeStaticLabel,
		ui->selectedFragmentBox,
		ui->selectedPodcastLabel,
		ui->selectedPodcastStaticLabel,
		ui->selectedTagsStaticLabel
	};

	for (auto w: fragmentPanelElements) {
		w->setEnabled(state);
	}
}



void MainWindow::handlePlayButton()
{

}
