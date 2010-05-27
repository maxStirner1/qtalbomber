TEMPLATE = app
TARGET = qtalbomber
DEPENDPATH += . ui game
INCLUDEPATH += . ui game
QT += network xml xmlpatterns

RESOURCES += qtalbomber.qrc

HEADERS += qtalbomber.h \
           ui/baseui.h \
           ui/gameui.h \
           ui/startui.h \
           ui/multiplayerui.h \
           ui/gamefield.h \
           game/map.h \
           game/player.h \
           handler/eventhandler.h \
           handler/multiplayerhandler.h
SOURCES += main.cpp \
           ui/gameui.cpp \
           ui/startui.cpp \
           ui/multiplayerui.cpp \
           ui/gamefield.cpp \
           game/map.cpp \
           game/player.cpp \
           handler/multiplayerhandler.cpp

