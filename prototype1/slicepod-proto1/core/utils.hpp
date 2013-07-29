#include "db_model.hpp"
#include <QtCore>

class QSqlError;


void db_connect(const QString &db_name);
bool db_check_schema();
void db_create_tables();

void check_error(const QSqlError& error, const char* info = "");

/**
 * @brief Scans directory with given path for music files to create episode
 *  objects. Creates and automatically persists one Directory object and
 *	Episodes and associated start Fragments for each valid music file.
 * @param dir_path
 * @param podcast Optionally set the same provided podcast for all new episodes.
 * @return Smart pointer to persisted Directory for given path or null smart
 *	pointer on failure.
 * @throw SQLException on persistence failure
 */
Directory::ptr scan_dir(const char* dir_path,
								  Podcast::ptr podcast
								  = Podcast::ptr());

//db::type::Directory::ptr scan_dir_auto(const char* dir_path);

Fragment::ptr add_start_fragment(const Episode::ptr& episode);
void db_create_tables();
