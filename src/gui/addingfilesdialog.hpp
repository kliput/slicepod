#ifndef ADDINGFILESDIALOG_HPP
#define ADDINGFILESDIALOG_HPP

#include <QDialog>

namespace Ui {
class AddingFilesDialog;
}

class AddingFilesDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit AddingFilesDialog(QWidget *parent = 0);
	~AddingFilesDialog();
	
private:
	Ui::AddingFilesDialog *ui;

public slots:
	void updateProgress(const QString& message, const int percent);
};

#endif // ADDINGFILESDIALOG_HPP
