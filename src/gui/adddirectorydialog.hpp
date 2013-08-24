#ifndef ADDDIRECTORYDIALOG_HPP
#define ADDDIRECTORYDIALOG_HPP

#include <QDialog>

#include "../core/maincore.hpp"

namespace Ui {
class AddDirectoryDialog;
}

class AddDirectoryDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit AddDirectoryDialog(MainCore *core, QWidget *parent = 0);
	~AddDirectoryDialog();
	
private:
	Ui::AddDirectoryDialog *ui;
	MainCore *core_;

public slots:
	void selectDirectory();
	void okButton();
	void updateRadios();
};

#endif // ADDDIRECTORYDIALOG_HPP
