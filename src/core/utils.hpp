/****************************************************************************
* Slicepod
* Copyright (C) 2013 Jakub Liput <jakub.liput@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/ 

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
