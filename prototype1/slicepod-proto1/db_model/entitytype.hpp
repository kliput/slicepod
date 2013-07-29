#ifndef ENTITYTYPE_HPP
#define ENTITYTYPE_HPP

#include <QString>
#include <QSharedPointer>
#include <QList>


//! Struct provides types for database model classes.
template <typename T>
class EntityType
{
public:
	typedef QString str;
	typedef QSharedPointer<T> ptr;
	typedef QList<ptr> ptr_list;
};

#endif // ENTITYTYPE_HPP
