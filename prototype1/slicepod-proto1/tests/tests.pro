#-------------------------------------------------
#
# Project created by QtCreator 2013-07-08T12:35:24
#
#-------------------------------------------------

APPDIR = ../

QMAKE_CXX = g++-4.8
QMAKE_CXXFLAGS += -Wall -Wno-unused-local-typedefs -std=c++11

QT       += core testlib

CONFIG += console precompile_header qxt
PRECOMPILED_HEADER = $$APPDIR/precompiled.hpp
QXT		 += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tst_slicepodtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

unix:!macx: LIBS += -L$$PWD/../../../../../lib/QxOrm-gcc4.8/lib/ -lQxOrm\
					-lvlc\
					-ltag\
					-lboost_serialization

INCLUDEPATH += $$PWD/../../../../../lib/QxOrm-gcc4.8/include\
				/usr/include
DEPENDPATH += $$PWD/../../../../../lib/QxOrm-gcc4.8/include

SOURCES += tst_slicepodtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH  += $$APPDIR/

SOURCES += $$APPDIR/db_model/directory.cpp\
	$$APPDIR/db_model/episode.cpp\
	$$APPDIR/db_model/playlist.cpp\
	$$APPDIR/db_model/podcast.cpp\
	$$APPDIR/db_model/fragment.cpp\
	$$APPDIR/db_model/tag.cpp \
	$$APPDIR/core/example_item.cpp \
	$$APPDIR/core/utils.cpp \
	$$APPDIR/core/library_model.cpp \
	$$APPDIR/core/library_item.cpp \
	$$APPDIR/core/vlcmanager.cpp \
	$$APPDIR/core/sqlexception.cpp

HEADERS  += $$APPDIR/db_model/directory.hpp\
	$$APPDIR/db_model/episode.hpp\
	$$APPDIR/db_model/playlist.hpp\
	$$APPDIR/db_model/podcast.hpp\
	$$APPDIR/db_model/fragment.hpp\
	$$APPDIR/db_model/tag.hpp \
	$$APPDIR/core/utils.hpp \
	$$APPDIR/core/playable.hpp \
	$$APPDIR/core/library_model.hpp \
	$$APPDIR/core/library_item.hpp \
	$$APPDIR/core/example_item.hpp \
	$$APPDIR/precompiled.hpp \
	$$APPDIR/export.hpp \
	$$APPDIR/db_model.hpp \
	$$APPDIR/core/vlcmanager.hpp \
	$$APPDIR/db_model/db_constants.hpp\
	$$APPDIR/core/sqlexception.hpp
