#ifndef _FRAGMENT_H_
#define _FRAGMENT_H_
#include "eclipse_ignore.hpp"

#include <core/playable.hpp>

class Episode;
class Tag;
class Playlist;

class QX_SLICEPOD_DLL_EXPORT Fragment: public Playable
{
public:
// -- properties
	long id = 0;
	
	QSharedPointer<Episode> episode;
	int start;
	int end;
	QString title;
	QString artist;
	QString metadata;
	
	QList<QSharedPointer<Tag>> tags_list;
	QList<QSharedPointer<Playlist>> playlists_list;
	
// -- contructor, virtual destructor
	Fragment() {}
	Fragment(const long& _id): id(_id) {}
	Fragment(const QSharedPointer<Episode>& _episode, int _start, int _end = -1):
		episode(_episode),
		start(_start),
		end(_end<0 ? _start : _end)
		{}

	virtual ~Fragment() {}
	
	/**
	 * Returns true if it is start fragment of its episode.
	 * TODO: test!
	 */
	bool is_start_fragment();

// -- playable --
	virtual void play() override;
//	virtual QString name() const override;

private:

	Fragment create_full_fragment(const QSharedPointer<Episode>& _episode);
	
};

QX_REGISTER_HPP_QX_SLICEPOD(Fragment, qx::trait::no_base_class_defined, 0);

typedef QSharedPointer<Fragment> fragment_ptr;
typedef QList<Fragment> fragments_list;

#endif // _FRAGMENT_H_
