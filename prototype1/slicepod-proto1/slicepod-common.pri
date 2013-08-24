QMAKE_CXX = g++-4.8
QMAKE_CXXFLAGS += -Wall -Wno-unused-local-typedefs -std=c++11 -gdwarf-3

TEMPLATE = app

QT += core sql

CONFIG += precompile_header
CONFIG -= app_bundle

TAGLIB_PATH = /usr/

LIBS +=	-lQtMultimediaKit\
		-L$$TAGLIB_PATH/lib/ -ltag\

INCLUDEPATH +=	$$TAGLIB_PATH/include\	# TagLib
				/usr/include/QtMultimediaKit\	# QtMultimediaKit (part of QtMobility when using Qt4)
				/usr/include/QtMobility\		# QtMobility (when using Qt4)
				/usr/include

DEPENDPATH += 	$$VLC_PATH/include\
				$$TAGLIB_PATH/include
