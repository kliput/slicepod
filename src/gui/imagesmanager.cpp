#include "imagesmanager.hpp"

ImagesManager::ImagesManager() :
	playArrow(QImage(":/images/arrow-down-double-blue.png")),
	startArrow(QImage(":/images/arrow-up-green.png")),
	endArrow(QImage(":/images/arrow-up-red.png")),
	markerArrow(QImage(":/images/arrow-up-yellow.png"))
{
}

ImagesManager::~ImagesManager()
{
}
