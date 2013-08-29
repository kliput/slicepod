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

#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <QString>
#include "baserecord.hpp"

namespace db {
namespace directory {
constexpr const char* TABLE_NAME = "Directories";
constexpr const char* PATH = "path";
constexpr const char* METADATA = "metadata";
}
}

class Directory : public BaseRecord<Directory>
{
public:
	static constexpr const char* TABLE_NAME = db::directory::TABLE_NAME;
	static const char* schemaString();
	Directory(QSqlRecord record, DatabaseEngine* engine = db::global_engine());
	Directory(const QString& path, DatabaseEngine* engine = db::global_engine());
	virtual ~Directory() {}

	inline const QString& getPath() const { return path_; }
	inline void setPath(const QString& path) { fieldChange(); path_ = path; }

	inline const QString& getMetadata() const { return metadata_; }
	inline void setMetadata(const QString& metadata) { fieldChange(); metadata_ = metadata; }

	QList<QSharedPointer<Episode>> getEpisodesList() const;

	QList<QVariant> valuesList() const override;
	const QStringList& columnsList() const override;

private:
	QString path_;
	QString metadata_;
};

#endif // DIRECTORY_HPP
