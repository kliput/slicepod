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

#include "precompiled.hpp"

#include <QApplication>
#include <QtCore>
#include <QtGui>

#include "gui/mainwindow.hpp"
#include "core/utils.hpp"
#include "db_model.hpp"

#include "core/maincore.hpp"

const char* APP_NAME = "Slicepod";
const char* APP_VERSION = "0.0.1";
const char* ORG_NAME = "Jakub Liput";

QSqlError err;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(),
			QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&qtTranslator);

	QTranslator slicepodTranslator;
	slicepodTranslator.load(":/translations/slicepod_" + QLocale::system().name());
	a.installTranslator(&slicepodTranslator);

	QApplication::setApplicationName(APP_NAME);
	QApplication::setApplicationVersion(APP_VERSION);
	QApplication::setOrganizationName(ORG_NAME);

	MainCore c;
	MainWindow w(&c);

	w.show();

	return a.exec();
}
