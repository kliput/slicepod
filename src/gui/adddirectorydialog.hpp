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
