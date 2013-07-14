#include "db_model.hpp"
#include <QtCore>

class QSqlError;


void db_connect(const char* db_name);
void check_error(const QSqlError& error, const char *info);
db::type::ptr<Directory> scan_dir(const char* dir_path,
										 QSharedPointer<Podcast> podcast);
db::type::ptr<Fragment> add_start_fragment(const db::type::ptr<Episode>& episode);
