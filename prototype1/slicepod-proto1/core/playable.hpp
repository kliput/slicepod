#ifndef _PLAYABLE_H_
#define _PLAYABLE_H_

#include <QtCore>

class Playable
{
public:
	virtual ~Playable() {}
	
	// -- utils --
	
	virtual void play() = 0;
//	virtual QString name() const = 0;
};

#endif // _PLAYABLE_H_
