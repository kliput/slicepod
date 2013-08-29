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

#include "sqlexception.hpp"

SQLException::SQLException(const QSqlError &qsql_error, const char *info)
	:
	  error_(qsql_error),
	  info_(info)
{
}

QSqlError SQLException::error() const
{
	return this->error_;
}

QString SQLException::text() const
{
	return QString("SQLError -> Database: %1; Driver: %2; Type: %3; Text: %4")
			.arg(error_.databaseText())
			.arg(error_.driverText())
			.arg(error_.type())
			.arg(error_.text())
			;
}
