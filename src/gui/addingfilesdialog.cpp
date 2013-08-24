#include "addingfilesdialog.hpp"
#include "ui_addingfilesdialog.h"

AddingFilesDialog::AddingFilesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddingFilesDialog)
{
	ui->setupUi(this);
}

AddingFilesDialog::~AddingFilesDialog()
{
	delete ui;
}

//! Updates main label and progress widgets. If message is QString() - doesn't
//! update main label.
void AddingFilesDialog::updateProgress(const QString &message, const int percent)
{
	if (!message.isNull()) {
		ui->messageLabel->setText(message);
	}
	ui->progressBar->setValue(percent);
}
