#-------------------------------------------------
#
# Project created by QtCreator 2013-07-08T12:35:24
#
#-------------------------------------------------

include(../slicepod-common.pri)

APPDIR = ../

TARGET = tst_slicepodtest
QT       += testlib
QT       -= gui
CONFIG += console

PRECOMPILED_HEADER = $$APPDIR/precompiled.hpp

SOURCES += tst_slicepodtest.cpp

INCLUDEPATH  += $$APPDIR/

SOURCES += $$APPDIR/db_model/directory.cpp\
	$$APPDIR/db_model/episode.cpp\
	$$APPDIR/db_model/playlist.cpp\
	$$APPDIR/db_model/podcast.cpp\
	$$APPDIR/db_model/fragment.cpp\
	$$APPDIR/db_model/tag.cpp \
	$$APPDIR/core/utils.cpp \
	$$APPDIR/core/vlcmanager.cpp \
	$$APPDIR/core/sqlexception.cpp

HEADERS  += $$APPDIR/db_model/directory.hpp\
	$$APPDIR/db_model/episode.hpp\
	$$APPDIR/db_model/playlist.hpp\
	$$APPDIR/db_model/podcast.hpp\
	$$APPDIR/db_model/fragment.hpp\
	$$APPDIR/db_model/tag.hpp \
	$$APPDIR/core/utils.hpp \
	$$APPDIR/precompiled.hpp \
	$$APPDIR/export.hpp \
	$$APPDIR/db_model.hpp \
	$$APPDIR/core/vlcmanager.hpp \
	$$APPDIR/db_model/db_constants.hpp\
	$$APPDIR/core/sqlexception.hpp
