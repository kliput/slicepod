#ifndef _PODCAST_H_
#define _PODCAST_H_
#include "eclipse_ignore.hpp"

#include "entitytype.hpp"
#include "db_constants.hpp"

#include <QtCore>

class Episode;
class Directory;

class QX_SLICEPOD_DLL_EXPORT Podcast : public EntityType<Podcast>
{
public:
	// -- properties
	long id = 0;
	
	str name;
	str metadata;
	EntityType<Episode>::ptr_list episodes_list;

	// -- contructor, virtual destructor
	Podcast() {}
	Podcast(long _id): id(_id) {}
	Podcast(const str& _name):
		name(_name)
		{}
		
	virtual ~Podcast() {}

};

QX_REGISTER_HPP_QX_SLICEPOD(Podcast, qx::trait::no_base_class_defined, 0)

#endif // _PODCAST_H_
