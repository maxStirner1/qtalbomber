TEMPLATE = app
TARGET = qtalbomber
DEPENDPATH += . 
INCLUDEPATH += . 
QT += network xml xmlpatterns

RESOURCES += qtalbomber.qrc

HEADERS += qtalbomber.h \
           baseui.h \
           gameui.h \
           startui.h \
           multiplayerui.h \
           gamefield.h \
           map.h \
           player.h \
           eventhandler.h \
           multiplayerhandler.h
SOURCES += main.cpp \
           gameui.cpp \
           startui.cpp \
           multiplayerui.cpp \
           gamefield.cpp \
           map.cpp \
           player.cpp \
           multiplayerhandler.cpp

