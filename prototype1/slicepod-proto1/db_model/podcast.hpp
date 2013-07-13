#ifndef _PODCAST_H_
#define _PODCAST_H_
#include "eclipse_ignore.hpp"

#include "core/library_item.hpp"


class Episode;
class Directory;

class QX_SLICEPOD_DLL_EXPORT Podcast : public LibraryItem
{
public:
	// -- properties
	long id = 0;
	
	QString name;
	QString metadata;
	QList<QSharedPointer<Episode>> episodes_list;

	// -- contructor, virtual destructor
	Podcast() {}
	Podcast(long _id): id(_id) {}
	Podcast(const QString& _name):
		name(_name)
		{}
		
	virtual ~Podcast() {}
	
// -- LibraryItem
	
	virtual QString info() const override;

};

QX_REGISTER_HPP_QX_SLICEPOD(Podcast, qx::trait::no_base_class_defined, 0);

typedef QSharedPointer<Podcast> podcast_ptr;
typedef QList<Podcast> podcasts_list;

#endif // _PODCAST_H_
