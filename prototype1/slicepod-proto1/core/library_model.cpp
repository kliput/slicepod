#include "core/library_model.hpp"
#include "core/example_item.hpp"

#include <QtCore>

LibraryModel::LibraryModel(QObject* parent):
	QAbstractItemModel(parent)
{
// 	QList<QVariant> root_data;
// 	root_data << tr("Episode/fragment title");
	root_item = new ExampleItem("Root");
	
	auto child1 = new ExampleItem("child1", root_item);
	root_item->append_child(child1);
	auto child2 = new ExampleItem("child2", root_item);
	root_item->append_child(child2);
	
	auto child11 = new ExampleItem("child11", child1);
	child1->append_child(child11);
	auto child12 = new ExampleItem("child12", child1);
	child1->append_child(child12);
	
	auto child21 = new ExampleItem("child21", child2);
	child2->append_child(child21);
	auto child22 = new ExampleItem("child22", child2);
	child2->append_child(child22);
	
}

LibraryModel::~LibraryModel()
{}

QVariant LibraryModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || role != Qt::DisplayRole) {
		return QVariant();
	}
	
	LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());
	
	return item->data(index.column());
}

Qt::ItemFlags LibraryModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return 0;
	}
	
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant LibraryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		return root_item->data(section);
	} else {
		return QVariant();
	}
// 	qDebug() << "hello header " << section << orientation << role;
// 	return QVariant("Nazwa");
}

QModelIndex LibraryModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent)) {
		return QModelIndex();
	}

	LibraryItem *parent_item;

	if (!parent.isValid()) {
		parent_item = root_item;
	} else {
		parent_item = static_cast<LibraryItem*>(parent.internalPointer());
	}

	LibraryItem *child_item = parent_item->child(row);
	if (child_item) {
		return createIndex(row, column, child_item);
	} else {
		return QModelIndex();
	}
}

QModelIndex LibraryModel::parent(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return QModelIndex();
	}

	LibraryItem *child_item = static_cast<LibraryItem*>(index.internalPointer());
	LibraryItem *parent_item = child_item->parent();

	if (parent_item == root_item) {
		return QModelIndex();
	}

	return createIndex(parent_item->row(), 0, parent_item);
}

int LibraryModel::rowCount(const QModelIndex &parent) const
{
	LibraryItem *parent_item;
	if (parent.column() > 0) {
		return 0;
	}

	if (!parent.isValid()) {
		parent_item = root_item;
	} else {
		parent_item = static_cast<LibraryItem*>(parent.internalPointer());
	}

	return parent_item->child_count();
}

int LibraryModel::columnCount(const QModelIndex &parent) const
{
// 	return 5;
	
	if (parent.isValid()) {
		return static_cast<LibraryItem*>(parent.internalPointer())->column_count();
	} else {
		return root_item->column_count();
	}
}
