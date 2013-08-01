#include "db_model.hpp"
#include <QtCore>

class QSqlError;


void db_connect(const QString &db_name);
bool db_check_schema();
void db_create_tables();

void check_error(const QSqlError& error, const char* info = "");

Fragment::ptr create_start_fragment(const Episode::ptr& episode);
void db_create_tables();
