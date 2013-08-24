//#include "db_model.hpp"

class QSqlError;
class QString;

namespace TagLib {
class Tag;
class String;
}


//void db_connect(const QString &db_name);
//bool db_check_schema();
//void db_create_tables();
//void db_create_tables();

void check_error(const QSqlError& error, const char* info = "");

//Fragment::ptr create_start_fragment(const Episode::ptr& episode);

//! Convert TagLib::String to QString (UTF8)
QString taglib_qstring(const TagLib::String &s);

//! Try to generate episode name with rules specific for some podasts.
//! If cannot - return QString()
QString generate_episode_name(TagLib::Tag* tag);
