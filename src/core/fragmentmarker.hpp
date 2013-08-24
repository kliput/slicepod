#ifndef FRAGMENTMARKER_HPP
#define FRAGMENTMARKER_HPP

class LibraryItem;

class FragmentMarker
{
public:
	FragmentMarker() {}
	FragmentMarker(LibraryItem* item);
	bool hasEnd();
private:
	const LibraryItem* item_ = nullptr;
};

#endif // FRAGMENTMARKER_HPP
