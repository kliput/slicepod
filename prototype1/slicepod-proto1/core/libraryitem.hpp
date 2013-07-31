#ifndef LIBRARYITEM_HPP
#define LIBRARYITEM_HPP

#include <QString>

#include "../db_model/db_constants.hpp"

class Fragment;

class LibraryItem
{
public:
	LibraryItem(const EntityType<Fragment>::ptr &fragment);

// -- getters --
	QString podcastName() const;
	QString episodeName() const;
	QString fragmentTitle() const;
	QString fragmentArtist() const;
	int fragmentStart() const;
	QStringList fragmentTagsList() const;

	EntityType<Fragment>::ptr fragmentPtr() const;

protected:
	EntityType<Fragment>::ptr fragmentPtr_;
};

#endif // LIBRARYITEM_HPP
