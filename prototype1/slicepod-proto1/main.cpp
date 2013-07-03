#include "mainwindow.h"
#include <QApplication>

#include "precompiled.hpp"

#include <QtCore>
#include <QtGui>

#include "db_model.hpp"

#include <iostream>
#include <boost/format.hpp>

#define LOG_LEVEL 2
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

	// -- database connection --
	QFile::remove(DB_NAME);
	db_connect(DB_NAME);

	// -- create all tables in database --
	err = qx::dao::create_table<Episode>();
	err = qx::dao::create_table<Directory>();
	err = qx::dao::create_table<Podcast>();
	err = qx::dao::create_table<Slice>();
	err = qx::dao::create_table<Tag>();
	err = qx::dao::create_table<Playlist>();


	add_scan_dir("/media/Dane 1/muzyka/podcasty/Irish and Celtic Music Podcast");
//	add_scan_dir("/home/kliput/podcast");

	episode_ptr e1(new Episode(1));
	slice_ptr s1(new Slice(e1, 240, 250));
	qx::dao::save(s1);

	s1->play();
	e1->play();

	return app.exec();
}
