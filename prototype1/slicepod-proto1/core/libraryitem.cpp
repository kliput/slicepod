#include "../precompiled.hpp"

#include <QtCore>

#include "libraryitem.hpp"
#include "../db_model/db_constants.hpp"
#include "../db_model/entitytype.hpp"
#include "../db_model/tag.hpp"
#include "../core/sqlexception.hpp"
#include "utils.hpp"

class Tag;

LibraryItem::LibraryItem(const EntityType<Fragment>::ptr &fragment)
	:
	  fragment_ptr_(fragment)
{
	// TODO: complete _necessary_ fragment data from database
	check_error(qx::dao::fetch_by_id_with_relation("*->*->*", fragment_ptr_),
				"fetching *->*->* data for fragment");
}

QString LibraryItem::podcastName() const
{
	return fragment_ptr_->episode->podcast->name;
}

QString LibraryItem::episodeName() const
{
	return fragment_ptr_->episode->episode_name;
}

QString LibraryItem::fragmentTitle() const
{
	return fragment_ptr_->title;
}

QString LibraryItem::fragmentArtist() const
{
	return fragment_ptr_->artist;
}

int LibraryItem::fragmentStart() const
{
	return fragment_ptr_->start;
}

QStringList LibraryItem::fragmentTagsList() const
{
	QStringList slist;
	for (Tag::ptr t: fragment_ptr_->tags_list) {
		slist << t->name;
	}
	return slist;
}

EntityType<Fragment>::ptr LibraryItem::fragmentPtr() const
{
	return fragment_ptr_;
}

