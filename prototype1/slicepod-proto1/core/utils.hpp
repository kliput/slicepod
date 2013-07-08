#include "db_model.hpp"
#include <QtCore>

class QSqlError;


void db_connect(const char* db_name);
void check_error(const QSqlError& error);
QSharedPointer<Directory> scan_dir(const char* dir_path,
										 QSharedPointer<Podcast> podcast);
fragment_ptr add_start_fragment(const episode_ptr& episode);
