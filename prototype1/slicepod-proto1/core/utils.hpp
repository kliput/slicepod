#include "db_model.hpp"

#include <QtSql/QSqlError>
#include <QLayout>

class QSqlError;


void db_connect(const QString &db_name);
bool db_check_schema();
void db_create_tables();

void check_error(const QSqlError& error, const char* info = "");

Fragment::ptr create_start_fragment(const Episode::ptr& episode);
void db_create_tables();

void set_enabled_for_each(QLayout *layout, bool state);
