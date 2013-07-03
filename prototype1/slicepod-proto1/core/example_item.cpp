#include "core/example_item.hpp"

ExampleItem::ExampleItem(QString _text, LibraryItem* _parent) :
	LibraryItem(_parent),
	text(_text)
{
}

ExampleItem::~ExampleItem()
{
}

QString ExampleItem::info() const
{
	return text;
}
