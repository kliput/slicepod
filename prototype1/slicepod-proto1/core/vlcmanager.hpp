#ifndef VLCMANAGER_H
#define VLCMANAGER_H

#include <QtCore>
#include <vlc/vlc.h>

class VLCManager
{
public:
	static VLCManager& instance();
	virtual ~VLCManager();

private:
	VLCManager();

	libvlc_instance_t* vlc_instance_;
};

#endif // VLCMANAGER_H
