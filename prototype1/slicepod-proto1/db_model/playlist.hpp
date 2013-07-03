#ifndef _PLAYLIST_H_
#define _PLAYLIST_H_
#include "eclipse_ignore.hpp"

class Playable;

class QX_PODSLICER_DLL_EXPORT Playlist
{
public:
// -- properties
	long id;
	
	QString name;
	QString metadata;
// 	QList<QSharedPointer<Playable>> playables_list;
	
// -- contructor, virtual destructor
	Playlist(): id(0) {}
	Playlist(long _id): id(_id) {}
	Playlist(const QString& _name):
		id(0),
		name(_name)
		{}
		
	virtual ~Playlist() {}
};

QX_REGISTER_HPP_QX_PODSLICER(Playlist, qx::trait::no_base_class_defined, 0);

typedef QSharedPointer<Playlist> playlist_ptr;
typedef QList<Playlist> playlist_list;

#endif // _PLAYLIST_H_
