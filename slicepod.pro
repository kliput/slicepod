#-------------------------------------------------
#
# Project created by QtCreator 2013-07-03T10:20:30
#
#-------------------------------------------------

CONFIG += debug

#CONFIG -= debug
#CONFIG += release

include(slicepod-common.pri)

QT       += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = slicepod

system(lrelease src/translations/slicepod_*.ts)
TRANSLATIONS = src/translations/slicepod_pl.ts

SOURCES +=\
	src/main.cpp\
	src/core/utils.cpp \
	src/core/sqlexception.cpp \
	src/gui/mainwindow.cpp \
	src/core/librarymodel.cpp \
	src/core/settingsmanager.cpp \
	src/core/testing.cpp \
	src/core/maincore.cpp \
	src/gui/adddirectorydialog.cpp \
	src/gui/addingfilesdialog.cpp \
	src/gui/positionwidget.cpp \
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
    src/core/libraryinfo.cpp \
    src/gui/imagesmanager.cpp \
    src/gui/fragmenteditorwidget.cpp

HEADERS  +=\
	src/core/utils.hpp \
	src/core/sqlexception.hpp \
	src/gui/mainwindow.hpp \
	src/core/librarymodel.hpp \
	src/core/libraryinfo.hpp \
	src/core/settingsmanager.hpp \
	src/core/testing.hpp \
	src/core/maincore.hpp \
	src/gui/adddirectorydialog.hpp \
	src/gui/addingfilesdialog.hpp \
	src/gui/positionwidget.hpp \
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
	src/db_engine/baserecord.hpp \
    src/gui/imagesmanager.hpp \
    src/gui/fragmenteditorwidget.hpp

FORMS +=\
	src/gui/mainwindow.ui \
	src/gui/adddirectorydialog.ui \
	src/gui/addingfilesdialog.ui \
    src/gui/fragmenteditorwidget.ui

RESOURCES += \
	src/slicepod.qrc

OTHER_FILES += \
	src/translations/slicepod_pl.qm


DESTDIR = build/
#release: DESTDIR = build/release
#debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
