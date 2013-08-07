#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QSettings>

#include <QtGui>

#include "positionwidget.hpp"
#include "../core/maincore.hpp"
#include "../core/librarymodel.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(MainCore *core, QWidget *parent = 0);
	~MainWindow();

	void fillItemInfoView(const LibraryItem *item);
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


};

#endif // MAINWINDOW_H
