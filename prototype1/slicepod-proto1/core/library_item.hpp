#ifndef _LIBRARY_ITEM_H_
#define _LIBRARY_ITEM_H_

#include <QtCore>

class LibraryItem
{
public:
	LibraryItem(LibraryItem* _parent = 0);
	virtual ~LibraryItem();

	void append_child(LibraryItem *child);

	LibraryItem *child(int row);
	int child_count() const;
	int column_count() const;
 	QVariant data(int column) const;
	int row() const;
	LibraryItem *parent();
	
	virtual QString info() const = 0;

private:
	QList<LibraryItem*> child_items;
	LibraryItem *parent_item;
};

#endif // _LIBRARY_ITEM_H_
