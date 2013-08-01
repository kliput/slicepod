#include "adddirectorydialog.hpp"
#include "ui_adddirectorydialog.h"

#include <QFileDialog>

#include "../db_model.hpp"

AddDirectoryDialog::AddDirectoryDialog(MainCore *core, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddDirectoryDialog),
	core_(core)
{
	ui->setupUi(this);

	connect(ui->selectDirectoryButton, SIGNAL(clicked()),
			this, SLOT(selectDirectory()));

	connect(ui->buttonBox, SIGNAL(accepted()),
			this, SLOT(okButton()));

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
	EntityType<Podcast>::ptr podcast(nullptr);
	if (ui->assignToOneRadio->isChecked()) {
		// TODO: choose podcast from db
		podcast = EntityType<Podcast>::ptr(nullptr);
	} else if (ui->assignAutomaticRadio->isChecked()) {
		QtConcurrent::run(core_, &MainCore::addPodcastDirectory,
						  ui->directoryEdit->text(), podcast);
//		core_->addPodcastDirectory(ui->directoryEdit->text(), podcast);
	}

	this->close();

}

