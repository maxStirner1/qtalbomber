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
           game/map.h \
           game/player.h
SOURCES += main.cpp \
           ui/gameui.cpp \
           ui/startui.cpp \
           ui/multiplayerui.cpp \
           game/map.cpp \
           game/player.cpp

