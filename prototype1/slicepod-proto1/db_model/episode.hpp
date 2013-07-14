#ifndef _EPISODE_H_
#define _EPISODE_H_

#include "eclipse_ignore.hpp"

#include "db_constants.hpp"

class Directory;
class Podcast;
class Fragment;

class QX_SLICEPOD_DLL_EXPORT Episode
{
public:
// -- properties
	long id = 0;
	
	db::type::str file_name;
	db::type::str episode_name;
	db::type::ptr<Directory> directory;
	db::type::ptr<Podcast> podcast;
	db::type::ptr<Fragment> start_fragment;
	db::type::str compare_data;
	db::type::str metadata;
	
	db::type::ptr_list<Fragment> fragments_list;
	
// -- contructor, virtual destructor
	Episode() {}
	Episode(long _id): id(_id) {}
	Episode(const db::type::str& _file_name, const db::type::str& _episode_name,
			const db::type::ptr<Directory>& _directory,
			const db::type::ptr<Podcast>& _podcast);
		
	virtual ~Episode() {}

private:
//	QSharedPointer<Fragment> create_full_fragment();
};

QX_REGISTER_HPP_QX_SLICEPOD(Episode, qx::trait::no_base_class_defined, 0);

#endif // _EPISODE_H_
