#ifndef _PODCAST_H_
#define _PODCAST_H_
#include "eclipse_ignore.hpp"

#include "core/library_item.hpp"

#include "db_constants.hpp"

class Episode;
class Directory;

class QX_SLICEPOD_DLL_EXPORT Podcast : public LibraryItem
{
public:
	// -- properties
	long id = 0;
	
	db::type::str name;
	db::type::str metadata;
	db::type::ptr_list<Episode> episodes_list;

	// -- contructor, virtual destructor
	Podcast() {}
	Podcast(long _id): id(_id) {}
	Podcast(const db::type::str& _name):
		name(_name)
		{}
		
	virtual ~Podcast() {}
	
// -- LibraryItem
	
	virtual db::type::str info() const override;

};

QX_REGISTER_HPP_QX_SLICEPOD(Podcast, qx::trait::no_base_class_defined, 0);

#endif // _PODCAST_H_
