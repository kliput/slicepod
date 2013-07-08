#include "vlcmanager.hpp"

#include <vlc/vlc.h>

VLCManager::VLCManager():
	vlc_instance_(libvlc_new(0, NULL))
{
}

VLCManager::~VLCManager()
{
	libvlc_release(this->vlc_instance_);
}

VLCManager& VLCManager::instance()
{
	static VLCManager manager;
	return manager;
}
