#ifndef _FRAGMENT_H_
#define _FRAGMENT_H_
#include "eclipse_ignore.hpp"

#include "entitytype.hpp"
#include "db_constants.hpp"

class Episode;
class Tag;
class Playlist;

class QX_SLICEPOD_DLL_EXPORT Fragment : public EntityType<Fragment>
{
public:
// -- properties
	long id = 0;
	
	EntityType<Episode>::ptr episode;
	int start;
	int end;
	str title;
	str artist;
	str metadata;
	
	EntityType<Tag>::ptr_list tags_list;
	EntityType<Playlist>::ptr_list playlists_list;
	
// -- contructor, virtual destructor
	Fragment() {}
	Fragment(const long& _id): id(_id) {}
	Fragment(const EntityType<Episode>::ptr& _episode, int _start, int _end=-1);

	virtual ~Fragment() {}
	
	/**
	 * Returns true if it is start fragment of its episode.
	 */
	bool is_start_fragment();

// -- play --
	void play();

};

QX_REGISTER_HPP_QX_SLICEPOD(Fragment, qx::trait::no_base_class_defined, 0)

#endif // _FRAGMENT_H_
