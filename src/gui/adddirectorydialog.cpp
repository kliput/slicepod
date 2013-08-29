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

#include "adddirectorydialog.hpp"
#include "ui_adddirectorydialog.h"

#include <QFileDialog>

#include "addingfilesdialog.hpp"

#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrentRun>
#else
#include <QtConcurrentRun>
#endif

AddDirectoryDialog::AddDirectoryDialog(MainCore *core, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddDirectoryDialog),
	core_(core)
{
	ui->setupUi(this);
	updateRadios();

	connect(ui->selectDirectoryButton, SIGNAL(clicked()),
			this, SLOT(selectDirectory()));

	connect(ui->buttonBox, SIGNAL(accepted()),
			this, SLOT(okButton()));

	connect(ui->assignToOneRadio, SIGNAL(clicked()),
			this, SLOT(updateRadios()));

	connect(ui->assignAutomaticRadio, SIGNAL(clicked()),
			this, SLOT(updateRadios()));
}

AddDirectoryDialog::~AddDirectoryDialog()
{
	delete ui;
}

void AddDirectoryDialog::selectDirectory()
{
	const QString path = QFileDialog::getExistingDirectory(
				this, tr("Open podcast's directory"),
				QString(),
				QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	ui->directoryEdit->setText(path);
}

void AddDirectoryDialog::okButton()
{
	BaseRecord<Podcast>::ptr podcast(nullptr);
	if (ui->assignToOneRadio->isChecked()) {
		// TODO: choose podcast from db
		podcast = BaseRecord<Podcast>::ptr(nullptr);
	} else if (ui->assignAutomaticRadio->isChecked()) {
		auto progressDialog = new AddingFilesDialog(this->parentWidget());
		progressDialog->setVisible(true);
		progressDialog->setAttribute(Qt::WA_DeleteOnClose);

		connect(core_, SIGNAL(loadingProgress(QString,int)),
				progressDialog, SLOT(updateProgress(QString,int)));
		connect(core_, SIGNAL(loadingFinished()),
				progressDialog, SLOT(close()));

		QtConcurrent::run(core_, &MainCore::addPodcastDirectory,
						  ui->directoryEdit->text(), podcast);
	}

	this->close();

}

void AddDirectoryDialog::updateRadios()
{
	if (ui->assignToOneRadio->isChecked()) {
		ui->podcastCombo->setDisabled(true);
		ui->podcastAddButton->setDisabled(true);
	} else if (ui->assignAutomaticRadio->isChecked()) {
		ui->podcastCombo->setDisabled(false);
		ui->podcastAddButton->setDisabled(false);
	}
}

