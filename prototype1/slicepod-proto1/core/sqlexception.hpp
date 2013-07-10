#ifndef SQLEXCEPTION_HPP
#define SQLEXCEPTION_HPP

#include <QtSql/QSqlError>

class SQLException : public std::exception
{
public:
	SQLException(const QSqlError& qsql_error, const char* info);
	QSqlError error() const;
	QString text() const;

private:
	const QSqlError error_;
	const QString info_;
};

#endif // SQLEXCEPTION_HPP
