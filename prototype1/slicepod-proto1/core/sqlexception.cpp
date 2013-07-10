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
