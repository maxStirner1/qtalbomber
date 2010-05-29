TEMPLATE = app
TARGET = qtalbomber
QT += network xml xmlpatterns

RESOURCES += qtalbomber.qrc

HEADERS += qtalbomber.h \
           gamefield.h \
           gamemap.h \
           player.h
SOURCES += main.cpp \
           gamefield.cpp \
           gamemap.cpp \
           player.cpp 

