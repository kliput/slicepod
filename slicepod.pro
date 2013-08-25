#-------------------------------------------------
#
# Project created by QtCreator 2013-07-03T10:20:30
#
#-------------------------------------------------

# CONFIG += debug

CONFIG -= debug
CONFIG += release

include(slicepod-common.pri)

QT       += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = slicepod

SRCDIR = src/

system(lrelease src/translations/slicepod_*.ts)
TRANSLATIONS = $$SRCDIR/translations/slicepod_pl.ts

SOURCES +=\
	$$SRCDIR/main.cpp\
	$$SRCDIR/core/utils.cpp \
	$$SRCDIR/core/sqlexception.cpp \
	$$SRCDIR/gui/mainwindow.cpp \
	$$SRCDIR/core/librarymodel.cpp \
	$$SRCDIR/core/libraryitem.cpp \
	$$SRCDIR/core/settingsmanager.cpp \
	$$SRCDIR/core/testing.cpp \
	$$SRCDIR/core/maincore.cpp \
	$$SRCDIR/gui/adddirectorydialog.cpp \
	$$SRCDIR/gui/addingfilesdialog.cpp \
	$$SRCDIR/gui/positionwidget.cpp \
	$$SRCDIR/core/musicplayer.cpp \
	$$SRCDIR/core/fragmentmarker.cpp \
	$$SRCDIR/db_engine/tag.cpp \
	$$SRCDIR/db_engine/podcast.cpp \
	$$SRCDIR/db_engine/playlist.cpp \
	$$SRCDIR/db_engine/fragmenttagmap.cpp \
	$$SRCDIR/db_engine/fragmentplaylistmap.cpp \
	$$SRCDIR/db_engine/fragment.cpp \
	$$SRCDIR/db_engine/episode.cpp \
	$$SRCDIR/db_engine/directory.cpp \
	$$SRCDIR/db_engine/databaseengine.cpp \
	$$SRCDIR/db_engine/baserecord.cpp

HEADERS  +=\
	$$SRCDIR/core/utils.hpp \
	$$SRCDIR/core/sqlexception.hpp \
	$$SRCDIR/gui/mainwindow.hpp \
	$$SRCDIR/core/librarymodel.hpp \
	$$SRCDIR/core/libraryitem.hpp \
	$$SRCDIR/core/settingsmanager.hpp \
	$$SRCDIR/core/testing.hpp \
	$$SRCDIR/core/maincore.hpp \
	$$SRCDIR/gui/adddirectorydialog.hpp \
	$$SRCDIR/gui/addingfilesdialog.hpp \
	$$SRCDIR/gui/positionwidget.hpp \
	$$SRCDIR/core/musicplayer.hpp \
	$$SRCDIR/core/fragmentmarker.hpp \
	$$SRCDIR/db_engine/tag.hpp \
	$$SRCDIR/db_engine/podcast.hpp \
	$$SRCDIR/db_engine/playlist.hpp \
	$$SRCDIR/db_engine/fragmenttagmap.hpp \
	$$SRCDIR/db_engine/fragmentplaylistmap.hpp \
	$$SRCDIR/db_engine/fragment.hpp \
	$$SRCDIR/db_engine/episode.hpp \
	$$SRCDIR/db_engine/directory.hpp \
	$$SRCDIR/db_engine/databaseengine.hpp \
	$$SRCDIR/db_engine/baserecord.hpp

FORMS +=\
	$$SRCDIR/gui/mainwindow.ui \
	$$SRCDIR/gui/adddirectorydialog.ui \
	$$SRCDIR/gui/addingfilesdialog.ui

RESOURCES += \
	$$SRCDIR/slicepod.qrc

OTHER_FILES += \
	$$SRCDIR/translations/slicepod_pl.qm


release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
