#ifndef _EPISODE_H_
#define _EPISODE_H_

#include "eclipse_ignore.hpp"

#include "../core/playable.hpp"

class Directory;
class Podcast;
class Fragment;

class QX_SLICEPOD_DLL_EXPORT Episode
{
public:
// -- properties
	long id = 0;
	
	QString file_name;
	QString episode_name;
	QSharedPointer<Directory> directory;
	QSharedPointer<Podcast> podcast;
	QSharedPointer<Fragment> start_fragment;
	QString compare_data;
	QString metadata;
	
	QList<QSharedPointer<Fragment>> fragments_list;
	
// -- contructor, virtual destructor
	Episode() {}
	Episode(long _id): id(_id) {}
	Episode(const QString& _file_name, const QString& _episode_name,
			const QSharedPointer<Directory>& _directory,
			const QSharedPointer<Podcast>& _podcast):
		file_name(_file_name),
		episode_name(_episode_name),
		directory(_directory),
		podcast(_podcast)
		{}
		
	virtual ~Episode() {}

private:
//	QSharedPointer<Fragment> create_full_fragment();
};

QX_REGISTER_HPP_QX_SLICEPOD(Episode, qx::trait::no_base_class_defined, 0);

typedef QSharedPointer<Episode> episode_ptr;
typedef QList<Episode> episodes_list;

#endif // _EPISODE_H_
