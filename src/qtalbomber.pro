TEMPLATE = app
TARGET = qtalbomber
DEPENDPATH += . client server ui
INCLUDEPATH += . ui client server
QT += network xml xmlpatterns

RESOURCES += qtalmedia.qrc

# Input
HEADERS += constants.h \
           client/AddServerDialog.h \
           client/CommunicationHandler.h \
           server/ConnectionHandler.h \
           server/GameMap.h \
           game/Map.h \
           game/Player.h \
           ui/ClientUI.h \
           ui/GameUI.h \
           ui/ServerUI.h \
           ui/StartUI.h \
           ui/GameFieldUI.h 
SOURCES += main.cpp \
           client/AddServerDialog.cpp \
           client/CommunicationHandler.cpp \
           server/ConnectionHandler.cpp \
           server/GameMap.cpp \
           game/Map.cpp \
           game/Player.cpp \
           ui/ClientUI.cpp \
           ui/GameUI.cpp \
           ui/ServerUI.cpp \
           ui/StartUI.cpp \
           ui/GameFieldUI.cpp

