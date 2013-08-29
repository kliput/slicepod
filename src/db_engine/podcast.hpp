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

#ifndef PODCAST_HPP
#define PODCAST_HPP

#include "baserecord.hpp"

namespace db {
namespace podcast {
constexpr const char* TABLE_NAME = "Podcasts";
constexpr const char* NAME = "name";
constexpr const char* METADATA = "metadata";
}
}

class Episode;

class Podcast : public BaseRecord<Podcast>
{
public:
	static constexpr const char* TABLE_NAME = db::podcast::TABLE_NAME;
	static const char* schemaString();
	Podcast(QSqlRecord record, DatabaseEngine* engine = db::global_engine());
	Podcast(const QString& name, DatabaseEngine* engine = db::global_engine());
	virtual ~Podcast() {}

	inline const QString& getName() const { return name_; }
	inline void setName(const QString& name) { fieldChange(); name_ = name; }

	inline const QString& getMetadata() const { return metadata_; }
	inline void setMetadata(const QString& metadata) { fieldChange(); metadata_ = metadata; }

	QList<QSharedPointer<Episode>> getEpisodesList() const;

	QList<QVariant> valuesList() const override;
	const QStringList& columnsList() const override;

private:
	QString name_;
	QString metadata_;
};

#endif // PODCAST_HPP
