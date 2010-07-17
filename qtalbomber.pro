QT += xml
QT += xmlpatterns
TARGET = qtalbomber
CONFIG += qt
TEMPLATE = app
RESOURCES += qtalbomber.qrc
SOURCES += main.cpp \
    level.cpp \
    gamefield.cpp \
    bomb.cpp \
    flam.cpp \
    block.cpp \
    player.cpp \
    about.cpp \
    bonus.cpp
HEADERS += level.h \
    gamefield.h \
    bomb.h \
    constant.h \
    flam.h \
    block.h \
    player.h \
    about.h \
    bonus.h
