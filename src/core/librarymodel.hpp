#ifndef LIBRARYMODEL_HPP
#define LIBRARYMODEL_HPP

#include <QAbstractTableModel>
#include <QList>
#include <QImage>
#include <QIcon>

class LibraryItem;
class MusicPlayer;

class LibraryModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	explicit LibraryModel(MusicPlayer* musicPlayer, QObject *parent = 0);
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

	enum Column {
		COL_PODCAST = 0,
		COL_EPISODE = 1,
		COL_TITLE = 2,
		COL_ARTIST = 3,
		COL_START = 4,
		COL_TAGS = 5
	};

	static constexpr const int COLUMN_COUNT = 6;

signals:

public slots:

private:
	QList<LibraryItem*> libraryItems_;
	MusicPlayer* player_;
	QImage playImage_;
	QIcon startFragmentIcon_;
};

#endif // LIBRARYMODEL_HPP
