unix: QMAKE_CXX = g++-4.8

QMAKE_CXXFLAGS += -Wall -Wextra -Wno-unused-local-typedefs -std=c++11
QMAKE_CXXFLAGS_DEBUG += -gdwarf-3

TEMPLATE = app

QT += core sql

CONFIG -= app_bundle

# TODO: library path .pri file

win32: TAGLIB_PATH = G:\biblioteki\taglib-1.8 # EDIT HERE!
unix: TAGLIB_PATH = /usr

VLC_PATH = /usr
VLCQT_PATH = /home/kliput/Programowanie/lib/vlc-qt/ # EDIT HERE!

LIBS +=	-L$$TAGLIB_PATH/lib/ -ltag\
			-L$$VLCQT_PATH/lib/ -lvlc-qt\
			-L$$VLC_PATH/lib/ -lvlc

INCLUDEPATH += ./src \
				$$TAGLIB_PATH/include\	# TagLib
				$$VLCQT_PATH/include\ # VLC-Qt
				/usr/include

