TEMPLATE = app
TARGET = qtalbomber
DEPENDPATH += . ui
INCLUDEPATH += . ui
QT += network xml xmlpatterns

RESOURCES += qtalbomber.qrc

HEADERS += qtalbomber.h \
           ui/baseui.h \
           ui/gameui.h \
           ui/startui.h \
           ui/multiplayerui.h
SOURCES += main.cpp \
           ui/gameui.cpp \
           ui/startui.cpp \
           ui/multiplayerui.cpp
