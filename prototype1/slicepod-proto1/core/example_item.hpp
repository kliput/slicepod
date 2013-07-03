#ifndef _EXAMPLE_ITEM_H_
#define _EXAMPLE_ITEM_H_

#include "core/library_item.hpp"

class ExampleItem : public LibraryItem
{
public:
	ExampleItem(QString _text = "Example", LibraryItem* _parent = 0);
	virtual ~ExampleItem();
	
	virtual QString info() const;
private:
	QString text;
};

#endif // _EXAMPLE_ITEM_H_
