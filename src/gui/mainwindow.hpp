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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QSettings>

#include <QtGui>

#include "positionwidget.hpp"
#include "core/maincore.hpp"
#include "core/librarymodel.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(MainCore* core, QWidget* parent = nullptr);
	~MainWindow();

	void fillFragmentInfoView(const Fragment::ptr fragment = Fragment::ptr());
	void setSelectedFragmentPanelEnabled(bool state);

private:
	Ui::MainWindow *ui;
	MainCore *core_;

public slots:
	void showMessage(QMessageBox::Icon icon, const QString& title,
				   const QString& text);
	void loadDatabaseSuccess();
	void addDirectoryDialog();
	void updateItemInfoView(const QModelIndex& current,
										const QModelIndex& /*prev*/);
	void activateLibraryItem(const QModelIndex& index);
	void handlePlayButton();
	void handleAddFragmentButton();

	void handleFragmentCreated(int row);


};

#endif // MAINWINDOW_H
