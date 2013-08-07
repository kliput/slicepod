#ifndef LIBRARYMODEL_HPP
#define LIBRARYMODEL_HPP

#include <QAbstractTableModel>
#include <QList>
#include <QImage>

class LibraryItem;

class LibraryModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	explicit LibraryModel(QObject *parent = 0);
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	virtual ~LibraryModel();

	const QList<LibraryItem*> libraryItems() { return libraryItems_; }

	void addItem(LibraryItem *item);
	void addItems(QList<LibraryItem*> itemsList);

	void loadFromDatabase();

	LibraryItem *libraryItemData(const QModelIndex &index);

signals:

public slots:

private:
	QList<LibraryItem*> libraryItems_;
	QImage playImage_;
};

#endif // LIBRARYMODEL_HPP
