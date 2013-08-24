QMAKE_CXX = g++-4.8
QMAKE_CXXFLAGS += -Wall -Wno-unused-local-typedefs -std=c++11

CONFIG(debug, debug|release) {
	QMAKE_CXXFLAGS -gdwarf-3
} else {

}

TEMPLATE = app

QT += core sql

CONFIG -= app_bundle

TAGLIB_PATH = /usr/

LIBS +=	-lQtMultimediaKit\
		-L$$TAGLIB_PATH/lib/ -ltag\

INCLUDEPATH += ./src \
				$$TAGLIB_PATH/include\	# TagLib
				/usr/include/QtMultimediaKit\	# QtMultimediaKit (part of QtMobility when using Qt4)
				/usr/include/QtMobility\		# QtMobility (when using Qt4)
				/usr/include

DEPENDPATH += 	$$TAGLIB_PATH/include
