/* Copyright (C) 2013 Jakub Liput
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 */

#include "mainwindow.h"
#include <QApplication>

#include "precompiled.hpp"

#include <QtCore>
#include <QtGui>

#include <QxtLogger>

#include "db_model.hpp"

#include <iostream>
#include <boost/format.hpp>

#include "core/utils.hpp"

const char* DB_NAME = "db.sqlite";

QSqlError err;

int main(int argc, char *argv[])
{
//	QApplication a(argc, argv);
//	MainWindow w;
//	w.show();
	
//	return a.exec();

	// -- qt application --
	QApplication app(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	qxtLog->enableAllLogLevels();

	// -- database connection --
	QFile::remove(DB_NAME);
	db_connect(DB_NAME);

	// -- create all tables in database --
	err = qx::dao::create_table<Episode>();
	err = qx::dao::create_table<Directory>();
	err = qx::dao::create_table<Podcast>();
	err = qx::dao::create_table<Fragment>();
	err = qx::dao::create_table<Tag>();
	err = qx::dao::create_table<Playlist>();

	podcast_ptr icmp_podcast(new Podcast("Irish and Celtic Music Podcast"));
	qx::dao::save(icmp_podcast);

	directory_ptr icmp_dir = scan_dir("/media/Dane 1/muzyka/podcasty/"
						   "Irish and Celtic Music Podcast", icmp_podcast);

	episode_ptr e1(new Episode(1));
	fragment_ptr f1(new Fragment(e1, 240, 250));
	qx::dao::save(f1);

	f1->play();

	return app.exec();
}
