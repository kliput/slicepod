#include "../precompiled.hpp"

#include <QtCore>

#include "libraryitem.hpp"
#include "../db_model/db_constants.hpp"
#include "../db_model/entitytype.hpp"
#include "../db_model/tag.hpp"
#include "../db_model/fragment.hpp"
#include "../core/sqlexception.hpp"
#include "utils.hpp"

/**
 * @brief LibraryItem::LibraryItem
 * @param fragment
 * @throw SQLException on database access error
 */
LibraryItem::LibraryItem(const Fragment::ptr &fragment)
	:
	  fragmentPtr_(fragment)
{


	QStringList relations;

	if (fragment->episode.isNull() || fragment->episode->podcast.isNull()) {
		relations << db::field::fragment::EPISODE;
		relations << QString("%1->%2").arg(db::field::fragment::EPISODE,
										   db::field::episode::PODCAST);
	}

	if (fragment->tags_list.isEmpty()) {
		relations << db::field::fragment::TAGS_LIST;
	}

	if (!relations.isEmpty()) {
		check_error(qx::dao::fetch_by_id_with_relation(relations, fragmentPtr_));
	}

}

QString LibraryItem::podcastName() const
{
	return fragmentPtr_->episode->podcast->name;
}

QString LibraryItem::episodeName() const
{
	return fragmentPtr_->episode->episode_name;
}

QString LibraryItem::fragmentTitle() const
{
	return fragmentPtr_->title;
}

QString LibraryItem::fragmentArtist() const
{
	return fragmentPtr_->artist;
}

int LibraryItem::fragmentStart() const
{
	return fragmentPtr_->start;
}

QStringList LibraryItem::fragmentTagsList() const
{
	QStringList slist;
	for (Tag::ptr t: fragmentPtr_->tags_list) {
		slist << t->name;
	}
	return slist;
}

EntityType<Fragment>::ptr LibraryItem::fragmentPtr() const
{
	return fragmentPtr_;
}

