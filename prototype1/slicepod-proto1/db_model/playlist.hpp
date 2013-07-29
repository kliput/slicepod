#ifndef _PLAYLIST_H_
#define _PLAYLIST_H_
#include "eclipse_ignore.hpp"

#include "entitytype.hpp"
#include "db_constants.hpp"

class Fragment;

class QX_SLICEPOD_DLL_EXPORT Playlist : public EntityType<Playlist>
{
public:
// -- properties
	long id;
	
	str name;
	str metadata;

	EntityType<Fragment>::ptr_list fragments_list;
	
// -- contructor, virtual destructor
	Playlist(): id(0) {}
	Playlist(long _id): id(_id) {}
	Playlist(const str& _name):
		id(0),
		name(_name)
		{}
		
	virtual ~Playlist() {}

};

QX_REGISTER_HPP_QX_SLICEPOD(Playlist, qx::trait::no_base_class_defined, 0)

#endif // _PLAYLIST_H_
