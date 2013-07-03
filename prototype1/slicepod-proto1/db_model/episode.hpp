#ifndef _EPISODE_H_
#define _EPISODE_H_

#include "eclipse_ignore.hpp"

#include "../core/playable.hpp"

class Directory;
class Podcast;
class Slice;

class QX_PODSLICER_DLL_EXPORT Episode: public Playable
{
public:
// -- properties
	long id;
	
	QString file_name;
	QString episode_name;
	QSharedPointer<Directory> directory;
	QSharedPointer<Podcast> podcast;
	QString compare_data;
	QString metadata;
	
	QList<QSharedPointer<Slice>> slices_list;
	
// -- contructor, virtual destructor
	Episode(): id(0) {}
	Episode(long _id): id(_id) {}
	Episode(const QString& _file_name, const QString& _episode_name,
			const QSharedPointer<Directory>& _directory,
			const QSharedPointer<Podcast>& _podcast):
		id(0),
		file_name(_file_name),
		episode_name(_episode_name),
		directory(_directory),
		podcast(_podcast)
		{}
		
	virtual ~Episode() {}
	
	// -- playable --
	
	virtual void play();
	virtual QString name() const;
};

QX_REGISTER_HPP_QX_PODSLICER(Episode, qx::trait::no_base_class_defined, 0);

typedef QSharedPointer<Episode> episode_ptr;
typedef QList<Episode> episodes_list;

#endif // _EPISODE_H_
