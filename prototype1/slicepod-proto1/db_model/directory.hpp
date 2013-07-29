#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include "eclipse_ignore.hpp"

#include "entitytype.hpp"
#include "db_constants.hpp"

class Episode;

class QX_SLICEPOD_DLL_EXPORT Directory : public EntityType<Directory>
{
public:
	// -- properties
	long id;
	
	str path;
	str metadata;
	EntityType<Episode>::ptr_list episodes_list;
	
	// -- contructor, virtual destructor
	Directory(): id(0) {}
	Directory(long _id): id(_id) {}
	Directory(const str& _path):
		id(0),
		path(_path)
		{}
		
	virtual ~Directory() {}

};

QX_REGISTER_HPP_QX_SLICEPOD(Directory, qx::trait::no_base_class_defined, 0)

#endif // _DIRECTORY_H_
