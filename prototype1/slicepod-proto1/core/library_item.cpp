#include "core/playable.hpp"
#include "core/library_item.hpp"

#include <QtCore>

LibraryItem::LibraryItem(LibraryItem* _parent):
	parent_item(_parent)
{
}

LibraryItem::~LibraryItem()
{
	qDeleteAll(child_items);
}

void LibraryItem::append_child(LibraryItem* item)
{
	child_items.append(item);
}

LibraryItem* LibraryItem::child(int row)
{
	return child_items.value(row);
}

int LibraryItem::child_count() const
{
	return child_items.count();
}

// TODO
int LibraryItem::column_count() const
{
	return 1;
}

int LibraryItem::row() const
{
	if (parent_item) {
		return parent_item->child_items.indexOf(const_cast<LibraryItem*>(this));
	} else {
		return 0;
	}
}

LibraryItem* LibraryItem::parent()
{
	return parent_item;
}

// TODO nie zwraca uwagi na kolumnę
QVariant LibraryItem::data(int column) const
{
	return info();
}
