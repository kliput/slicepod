unix: QMAKE_CXX = g++-4.8

QMAKE_CXXFLAGS += -Wall -Wextra -Wno-unused-local-typedefs -std=c++11
QMAKE_CXXFLAGS_DEBUG += -gdwarf-3

TEMPLATE = app

QT += core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += multimedia

CONFIG -= app_bundle

win32: TAGLIB_PATH = G:\biblioteki\taglib-1.8 # EDIT HERE!
#unix: TAGLIB_PATH = /usr

LIBS +=	-L$$TAGLIB_PATH/lib/ -ltag\
#unix: LIBS += -lQtMultimediaKit

INCLUDEPATH += ./src \
				$$TAGLIB_PATH/include\	# TagLib
				/usr/include/QtMultimediaKit\	# QtMultimediaKit (part of QtMobility when using Qt4)
				/usr/include/QtMobility\		# QtMobility (when using Qt4)
				/usr/include

DEPENDPATH += 	$$TAGLIB_PATH/include
