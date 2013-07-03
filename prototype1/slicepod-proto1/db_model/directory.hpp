#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include "eclipse_ignore.hpp"

class Episode;

class QX_PODSLICER_DLL_EXPORT Directory
{
public:
// -- properties
	long id;
	
	QString path;
	QString metadata;
	QList<QSharedPointer<Episode>> episodes_list;
	
// -- contructor, virtual destructor
	Directory(): id(0) {}
	Directory(long _id): id(_id) {}
	Directory(const QString& _path):
		id(0),
		path(_path)
		{}
		
	virtual ~Directory() {}
};

QX_REGISTER_HPP_QX_PODSLICER(Directory, qx::trait::no_base_class_defined, 0);

typedef QSharedPointer<Directory> directory_ptr;
typedef QList<Directory> directories_list;

#endif // _DIRECTORY_H_
