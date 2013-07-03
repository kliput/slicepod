#ifndef _SLICE_H_
#define _SLICE_H_
#include "eclipse_ignore.hpp"

#include <core/playable.hpp>

class Episode;
class Tag;

class QX_PODSLICER_DLL_EXPORT Slice: public Playable
{
public:
// -- properties
	long id;
	
	QSharedPointer<Episode> episode;
	int start;
	int end;
	QString title;
	QString artist;
	QString metadata;
	
	QList<QSharedPointer<Tag>> tags_list;
	
// -- contructor, virtual destructor
	Slice(): id(0) {}
	Slice(const QSharedPointer<Episode>& _episode, int _start, int _end = -1):
		id(0),
		episode(_episode),
		start(_start),
		end(_end<0 ? _start : _end)
		{}
		
	virtual ~Slice() {}
	
	// -- playable --
	
	virtual void play();
	virtual QString name() const;
	
};

QX_REGISTER_HPP_QX_PODSLICER(Slice, qx::trait::no_base_class_defined, 0);

typedef QSharedPointer<Slice> slice_ptr;
typedef QList<Slice> slices_list;

#endif // _SLICE_H_
