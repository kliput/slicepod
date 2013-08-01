QMAKE_CXX = g++-4.8
#QMAKE_CXX = clang++
QMAKE_CXXFLAGS += -Wall -Wno-unused-local-typedefs -std=c++11 -gdwarf-3

TEMPLATE = app

QT += core

CONFIG += precompile_header
CONFIG -= app_bundle

DEFINES += _QX_SERIALIZE_BINARY_ENABLED
DEFINES += _QX_SERIALIZE_XML_ENABLED
DEFINES += _BUILDING_QX_SLICEPOD

QXORM_PATH = /home/kliput/Programowanie/lib/QxOrm-1.2.6.beta3-gcc4.8/
BOOST_PATH = /usr/
VLC_PATH = /usr/
TAGLIB_PATH = /usr/

LIBS += -L$$QXORM_PATH/lib/ -lQxOrm\
		-L$$VLC_PATH/lib/ -lvlc\
		-L$$TAGLIB_PATH/lib/ -ltag\
		-L$$BOOST_PATH/lib/ -lboost_serialization

INCLUDEPATH += $$QXORM_PATH/include\
				$$BOOST_PATH/include\
				$$VLC_PATH/include\
				$$TAGLIB_PATH/include\
				/usr/include

DEPENDPATH += $$QXORM_PATH/include\
				$$BOOST_PATH/include\
				$$VLC_PATH/include\
				$$TAGLIB_PATH/include
