#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include "eclipse_ignore.hpp"

#include "db_constants.hpp"

class Episode;

class QX_SLICEPOD_DLL_EXPORT Directory
{
public:
	// -- properties
	long id;
	
	db::type::str path;
	db::type::str metadata;
	db::type::ptr_list<Episode> episodes_list;
	
	// -- contructor, virtual destructor
	Directory(): id(0) {}
	Directory(long _id): id(_id) {}
	Directory(const db::type::str& _path):
		id(0),
		path(_path)
		{}
		
	virtual ~Directory() {}

};

QX_REGISTER_HPP_QX_SLICEPOD(Directory, qx::trait::no_base_class_defined, 0);

#endif // _DIRECTORY_H_
