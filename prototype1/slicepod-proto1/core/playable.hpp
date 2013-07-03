#ifndef _PLAYABLE_H_
#define _PLAYABLE_H_

#include <QtCore>

class Playable
{
public:
	virtual ~Playable() {}
	
	// -- utils --
	
	// TODO sprawdzić nullptr dla zabawy
	virtual void play() = 0;
	virtual QString name() const = 0;
};

typedef QSharedPointer<Playable> playable_ptr;
typedef QList<Playable> playables_list;

#endif // _PLAYABLE_H_
