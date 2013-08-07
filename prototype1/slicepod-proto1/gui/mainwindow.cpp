#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QtGui>
#include <QWidget>
#include <QtConcurrentRun>
#include <QFutureWatcher>
#include <QMetaType>

#include "adddirectorydialog.hpp"

Q_DECLARE_METATYPE(QMessageBox::Icon)

#include <QxOrm.h>

// TODO: create LibraryManager class to manage library (move methods from here)
#include "../core/utils.hpp"
#include "../db_model.hpp"
#include "../core/libraryitem.hpp"
#include "../core/settingsmanager.hpp"

#include "../core/librarymodel.hpp"
#include <QSortFilterProxyModel>

MainWindow::MainWindow(MainCore *core, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	core_(core)
{
	ui->setupUi(this);
	positionWidget_ = new PositionWidget(ui->centralWidget);
	positionWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	ui->playerLayout->addWidget(positionWidget_);

	ui->libraryView->setModel(core_->proxyModel());
	ui->libraryView->setVisible(true);

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
	fillItemInfoView(nullptr);

	connect(ui->libraryView, SIGNAL(activated(QModelIndex)),
			this, SLOT(activateLibraryItem(QModelIndex)));

	// -- player controls --
	connect(ui->playButton, SIGNAL(clicked()),
			this, SLOT(handlePlayButton()));

	connect(core_->player(), SIGNAL(positionChanged(int)),
			positionWidget_, SLOT(setPlayerPosition(int)));

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

void MainWindow::fillItemInfoView(const LibraryItem* item)
{
	if (!item) {
		setSelectedFragmentPanelEnabled(false);
	} else {
		setSelectedFragmentPanelEnabled(true);

		ui->selectedPodcastLabel->setText(item->podcastName());
		ui->selectedEpisodeLabel->setText(item->episodeName());

		ui->startTimeEdit->setTimeRange(QTime(), item->episodeLengthTime());
		ui->startTimeEdit->setTime(item->fragmentStartTime());

		if (item->hasEnd()) {
			ui->endTimeEdit->setTimeRange(QTime(), item->episodeLengthTime());
			ui->endTimeEdit->setTime(item->fragmentEndTime());
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
	auto item = core_->libraryModel()->libraryItemData(current);

	fillItemInfoView(item);

}

void MainWindow::activateLibraryItem(const QModelIndex &index)
{
	auto item = core_->libraryModel()->libraryItemData(index);

	if (core_->player()->loadMedia(item)) {
		positionWidget_->setMediaLength(item->episodeLengthSec());
		core_->player()->seek(item->fragmentStartSec());
		core_->player()->play();
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
