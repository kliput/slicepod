#-------------------------------------------------
#
# Project created by QtCreator 2013-07-03T10:20:30
#
#-------------------------------------------------

include(slicepod-common.pri)

QT       += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = podslicer

PRECOMPILED_HEADER = precompiled.hpp

TRANSLATIONS = translations/slicepod_pl.ts

SOURCES += main.cpp\
	db_model/directory.cpp\
	db_model/episode.cpp\
	db_model/playlist.cpp\
	db_model/podcast.cpp\
	db_model/fragment.cpp\
	db_model/tag.cpp \
    core/utils.cpp \
	core/vlcmanager.cpp \
    core/sqlexception.cpp \
	gui/mainwindow.cpp \
	core/librarymodel.cpp \
	core/libraryitem.cpp \
    core/settingsmanager.cpp \
    core/testing.cpp \
    core/maincore.cpp \
    gui/adddirectorydialog.cpp

HEADERS  +=\
	db_model/directory.hpp\
	db_model/episode.hpp\
	db_model/playlist.hpp\
	db_model/podcast.hpp\
	db_model/fragment.hpp\
	db_model/tag.hpp \
    core/utils.hpp \
    precompiled.hpp \
    export.hpp \
    db_model.hpp \
    core/vlcmanager.hpp \
    db_model/db_constants.hpp \
    core/sqlexception.hpp \
	gui/mainwindow.hpp \
	core/librarymodel.hpp \
	core/libraryitem.hpp \
    core/settingsmanager.hpp \
    core/testing.hpp \
    core/maincore.hpp \
    db_model/entitytype.hpp \
    gui/adddirectorydialog.hpp

FORMS    += gui/mainwindow.ui \
    gui/adddirectorydialog.ui

RESOURCES += \
    slicepod.qrc

OTHER_FILES += \
	translations/slicepod_pl.qm
