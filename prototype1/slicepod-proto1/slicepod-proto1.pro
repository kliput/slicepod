#-------------------------------------------------
#
# Project created by QtCreator 2013-07-03T10:20:30
#
#-------------------------------------------------

QMAKE_CXX = g++-4.7
QMAKE_CXXFLAGS += -std=c++0x

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = slicepod-proto1
TEMPLATE = app

SOURCES += main.cpp\
		mainwindow.cpp\
		db_model/directory.cpp\
		db_model/episode.cpp\
		db_model/playlist.cpp\
		db_model/podcast.cpp\
		db_model/slice.cpp\
		db_model/tag.cpp \
    core/example_item.cpp \
    core/utils.cpp \
    core/library_model.cpp \
    core/library_item.cpp

HEADERS  += mainwindow.h\
		db_model/directory.hpp\
		db_model/episode.hpp\
		db_model/playlist.hpp\
		db_model/podcast.hpp\
		db_model/slice.hpp\
		db_model/tag.hpp \
    core/utils.hpp \
    core/playable.hpp \
    core/library_model.hpp \
    core/library_item.hpp \
    core/example_item.hpp \
    precompiled.hpp \
    export.hpp \
    db_model.hpp

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$PWD/../../../../lib/QxOrm-gcc4.7/lib/ -lQxOrm\
					-lvlc\
					-ltag\
					-lboost_serialization

INCLUDEPATH += $$PWD/../../../../lib/QxOrm-gcc4.7/include\
				/usr/include
DEPENDPATH += $$PWD/../../../../lib/QxOrm-gcc4.7/include
