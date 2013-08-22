#include "fragmentmarker.hpp"

FragmentMarker::FragmentMarker(LibraryItem* item) :
	item_(item)
{
	if (!item) throw 0; // TODO: handle error
}

bool FragmentMarker::hasEnd()
{
//	return end >= 0;
	return false;
}
