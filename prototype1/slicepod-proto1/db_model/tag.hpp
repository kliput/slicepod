#ifndef _TAG_H_
#define _TAG_H_
#include "eclipse_ignore.hpp"

class Slice;

class QX_PODSLICER_DLL_EXPORT Tag
{
public:
// -- properties
	long id;
	
	QString name;
	QString metadata;
	QList<QSharedPointer<Slice>> slices_list;
	
// -- contructor, virtual destructor
	Tag(): id(0) {}
	Tag(long _id): id(_id) {}
	Tag(const QString& _name):
		id(0),
		name(_name)
		{}
		
	virtual ~Tag() {}
};

QX_REGISTER_HPP_QX_PODSLICER(Tag, qx::trait::no_base_class_defined, 0);

typedef QSharedPointer<Tag> tag_ptr;
typedef QList<Tag> tags_list;

#endif // _TAG_H_
