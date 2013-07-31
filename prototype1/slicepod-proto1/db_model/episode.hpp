#ifndef _EPISODE_H_
#define _EPISODE_H_

#include "eclipse_ignore.hpp"

#include <taglib/fileref.h>

#include "entitytype.hpp"
#include "db_constants.hpp"

class Directory;
class Podcast;
class Fragment;

class QX_SLICEPOD_DLL_EXPORT Episode : public EntityType<Episode>
{
public:
// -- properties
	long id = 0;
	
	str fileName;
	str episodeName;
	EntityType<Directory>::ptr directory;
	EntityType<Podcast>::ptr podcast;
	EntityType<Fragment>::ptr startFragment;
	str compare_data;
	str metadata;
	
	EntityType<Fragment>::ptr_list fragmentsList;
	
// -- contructor, virtual destructor
	Episode() {}
	Episode(long _id): id(_id) {}
	Episode(const str& _file_name, const str& _episode_name,
			const EntityType<Directory>::ptr& _directory,
			const EntityType<Podcast>::ptr& _podcast);
		
	virtual ~Episode() {}

// -- utils
	str fullPath();
	TagLib::FileRef fileInfo();
	int audioLength();
};

QX_REGISTER_HPP_QX_SLICEPOD(Episode, qx::trait::no_base_class_defined, 0)

#endif // _EPISODE_H_
