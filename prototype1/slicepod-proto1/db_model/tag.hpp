#ifndef _TAG_H_
#define _TAG_H_
#include "eclipse_ignore.hpp"

#include "db_constants.hpp"

class Fragment;

class QX_SLICEPOD_DLL_EXPORT Tag
{
public:
// -- properties
	long id;
	
	db::type::str name;
	db::type::str metadata;
	db::type::ptr_list<Fragment> fragments_list;
	
// -- contructor, virtual destructor
	Tag(): id(0) {}
	Tag(long _id): id(_id) {}
	Tag(const db::type::str& _name):
		id(0),
		name(_name)
		{}
		
	virtual ~Tag() {}
};

QX_REGISTER_HPP_QX_SLICEPOD(Tag, qx::trait::no_base_class_defined, 0);

#endif // _TAG_H_
