#-------------------------------------------------
#
# Project created by QtCreator 2013-08-31T21:46:28
#
#-------------------------------------------------

include(slicepod-common.pri)

QT       += sql testlib gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tst_teststest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tests/tst_teststest.cpp\
	src/core/utils.cpp \
	src/core/sqlexception.cpp \
	src/core/librarymodel.cpp \
	src/core/settingsmanager.cpp \
	src/core/testing.cpp \
	src/core/maincore.cpp \
	src/core/musicplayer.cpp \
	src/db_engine/tag.cpp \
	src/db_engine/podcast.cpp \
	src/db_engine/playlist.cpp \
	src/db_engine/fragmenttagmap.cpp \
	src/db_engine/fragmentplaylistmap.cpp \
	src/db_engine/fragment.cpp \
	src/db_engine/episode.cpp \
	src/db_engine/directory.cpp \
	src/db_engine/databaseengine.cpp \
	src/db_engine/baserecord.cpp \
	src/core/libraryinfo.cpp

HEADERS  +=\
	src/core/utils.hpp \
	src/core/sqlexception.hpp \
	src/core/librarymodel.hpp \
	src/core/libraryinfo.hpp \
	src/core/settingsmanager.hpp \
	src/core/testing.hpp \
	src/core/maincore.hpp \
	src/core/musicplayer.hpp \
	src/db_engine/tag.hpp \
	src/db_engine/podcast.hpp \
	src/db_engine/playlist.hpp \
	src/db_engine/fragmenttagmap.hpp \
	src/db_engine/fragmentplaylistmap.hpp \
	src/db_engine/fragment.hpp \
	src/db_engine/episode.hpp \
	src/db_engine/directory.hpp \
	src/db_engine/databaseengine.hpp \
	src/db_engine/baserecord.hpp

INCLUDEPATH += src/ \
			tests/
