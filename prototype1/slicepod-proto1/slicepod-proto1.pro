#-------------------------------------------------
#
# Project created by QtCreator 2013-07-03T10:20:30
#
#-------------------------------------------------

include(slicepod-common.pri)

QT       += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = slicepod

TRANSLATIONS = translations/slicepod_pl.ts

SOURCES += main.cpp\
    core/utils.cpp \
    core/sqlexception.cpp \
	gui/mainwindow.cpp \
	core/librarymodel.cpp \
	core/libraryitem.cpp \
    core/settingsmanager.cpp \
    core/testing.cpp \
    core/maincore.cpp \
    gui/adddirectorydialog.cpp \
    gui/addingfilesdialog.cpp \
    gui/positionwidget.cpp \
    core/musicplayer.cpp \
    core/fragmentmarker.cpp \
    db_engine/tag.cpp \
    db_engine/podcast.cpp \
    db_engine/playlist.cpp \
    db_engine/fragmenttagmap.cpp \
    db_engine/fragmentplaylistmap.cpp \
    db_engine/fragment.cpp \
    db_engine/episode.cpp \
    db_engine/directory.cpp \
    db_engine/databaseengine.cpp \
    db_engine/baserecord.cpp

HEADERS  +=\
    core/utils.hpp \
    precompiled.hpp \
    core/sqlexception.hpp \
	gui/mainwindow.hpp \
	core/librarymodel.hpp \
	core/libraryitem.hpp \
    core/settingsmanager.hpp \
    core/testing.hpp \
	core/maincore.hpp \
    gui/adddirectorydialog.hpp \
    gui/addingfilesdialog.hpp \
    gui/positionwidget.hpp \
    core/musicplayer.hpp \
    core/fragmentmarker.hpp \
    db_engine/tag.hpp \
    db_engine/podcast.hpp \
    db_engine/playlist.hpp \
    db_engine/fragmenttagmap.hpp \
    db_engine/fragmentplaylistmap.hpp \
    db_engine/fragment.hpp \
    db_engine/episode.hpp \
    db_engine/directory.hpp \
    db_engine/databaseengine.hpp \
    db_engine/baserecord.hpp

FORMS    += gui/mainwindow.ui \
    gui/adddirectorydialog.ui \
    gui/addingfilesdialog.ui

RESOURCES += \
    slicepod.qrc

OTHER_FILES += \
	translations/slicepod_pl.qm
