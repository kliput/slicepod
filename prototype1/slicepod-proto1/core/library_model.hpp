#ifndef _LIBRARY_MODEL_H_
#define _LIBRARY_MODEL_H_

#include <QtCore>
#include <QtGui>

class LibraryItem;

class LibraryModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	LibraryModel(QObject* parent = 0);
	~LibraryModel();

	QVariant data(const QModelIndex &index, int role) const /*override*/;
	Qt::ItemFlags flags(const QModelIndex &index) const /*override*/;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role = Qt::DisplayRole) const /*override*/;
	QModelIndex index(int row, int column,
					const QModelIndex &parent = QModelIndex()) const /*override*/;
	QModelIndex parent(const QModelIndex &index) const /*override*/;
	int rowCount(const QModelIndex &parent = QModelIndex()) const /*override*/;
	int columnCount(const QModelIndex &parent = QModelIndex()) const /*override*/;
	
private:
	LibraryItem *root_item;
};

#endif // _LIBRARY_MODEL_H_
