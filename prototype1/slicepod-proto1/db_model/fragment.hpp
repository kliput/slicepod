#ifndef _FRAGMENT_H_
#define _FRAGMENT_H_
#include "eclipse_ignore.hpp"

#include <core/playable.hpp>

#include "db_constants.hpp"

class Episode;
class Tag;
class Playlist;

class QX_SLICEPOD_DLL_EXPORT Fragment: public Playable
{
public:
// -- properties
	long id = 0;
	
	db::type::ptr<Episode> episode;
	int start;
	int end;
	db::type::str title;
	db::type::str artist;
	db::type::str metadata;
	
	db::type::ptr_list<Tag> tags_list;
	db::type::ptr_list<Playlist> playlists_list;
	
// -- contructor, virtual destructor
	Fragment() {}
	Fragment(const long& _id): id(_id) {}
	Fragment(const db::type::ptr<Episode>& _episode, int _start, int _end=-1);

	virtual ~Fragment() {}
	
	/**
	 * Returns true if it is start fragment of its episode.
	 * TODO: test!
	 */
	bool is_start_fragment();

// -- playable --
	virtual void play() override;
//	virtual str name() const override;

private:

//	Fragment create_full_fragment(const ptr<Episode>& _episode);
	
};

QX_REGISTER_HPP_QX_SLICEPOD(Fragment, qx::trait::no_base_class_defined, 0);

#endif // _FRAGMENT_H_
