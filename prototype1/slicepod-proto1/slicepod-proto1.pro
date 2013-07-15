#-------------------------------------------------
#
# Project created by QtCreator 2013-07-03T10:20:30
#
#-------------------------------------------------

QMAKE_CXX = g++-4.8
QMAKE_CXXFLAGS += -Wall -Wno-unused-local-typedefs -std=c++11 -gdwarf-2

QT       += core gui

CONFIG += console precompile_header qxt
PRECOMPILED_HEADER = ./precompiled.hpp
QXT		 += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = podslicer
TEMPLATE = app

QXORM_PATH = /home/kliput/Programowanie/slicepod/lib/QxOrm-1.2.5-gcc4.6/

SOURCES += main.cpp\
	mainwindow.cpp\
	db_model/directory.cpp\
	db_model/episode.cpp\
	db_model/playlist.cpp\
	db_model/podcast.cpp\
	db_model/fragment.cpp\
	db_model/tag.cpp \
    core/example_item.cpp \
    core/utils.cpp \
    core/library_model.cpp \
    core/library_item.cpp \
	core/vlcmanager.cpp \
    core/sqlexception.cpp

HEADERS  += mainwindow.h\
	db_model/directory.hpp\
	db_model/episode.hpp\
	db_model/playlist.hpp\
	db_model/podcast.hpp\
	db_model/fragment.hpp\
	db_model/tag.hpp \
    core/utils.hpp \
    core/playable.hpp \
    core/library_model.hpp \
    core/library_item.hpp \
    core/example_item.hpp \
    precompiled.hpp \
    export.hpp \
    db_model.hpp \
    core/vlcmanager.hpp \
    db_model/db_constants.hpp \
    core/sqlexception.hpp

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$QXORM_PATH/lib/ -lQxOrm\
					-lvlc\
					-ltag\
					-lboost_serialization

INCLUDEPATH += $$QXORM_PATH/include\
				/usr/include
DEPENDPATH += $$QXORM_PATH/include
