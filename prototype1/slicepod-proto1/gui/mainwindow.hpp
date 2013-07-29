#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QSettings>

#include <QtGui>

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
	
private:
	Ui::MainWindow *ui;
	MainCore *core_;

public slots:
	void showMessage(QMessageBox::Icon icon, const QString& title,
				   const QString& text);
	void loadDatabaseSuccess();

	void addDirectoryDialog();
//	void setStatus(const QString &text);
};

#endif // MAINWINDOW_H
