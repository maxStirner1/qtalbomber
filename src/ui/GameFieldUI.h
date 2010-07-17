/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        GameFieldUI.h
 *  Description: Header file for GameFieldUI class.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _GAMEFIELDUI_H
#define	_GAMEFIELDUI_H

#include "constants.h"
#include "client/CommunicationHandler.h"
#include "game/Map.h"
#include "game/Player.h"
#include <QtGui>
#include <QDebug>

class GameUI;

class GameFieldUI : public QWidget {
    Q_OBJECT
    
public:
    GameFieldUI(GameUI* parent = 0);
    QString layoutTitle();
    void setClientCom(CommunicationHandler* clientCom);
    int isInGame();
    QList<QGraphicsRectItem*> mapItems();

private slots:
    void onGameJoined(int position);
    void setMapInfo(int width, int height, QList<int> collisionsTab,
        QList<int> unbreakableBlocks, QList<int> breakableBlocks, 
        QList<int> bonusPositions, QList<int> playersPositions);
    void onPlayerRefused();
    void onPlayerDisconnected();
    void requestPlayerList();
    void onInitPlayers(int playerNb);
    void onPlayerUpdated(Player* player);
    void onGameStart();
    void displayCountdown();
    void moveRival(int playerIdx, int playerPosition);

private:
    void drawMap();
    void movePlayer(int position);
    void writeMapText(QString text, int size = 30, bool add = true, bool center = false);
    void removeMapText();
    void updatePlayerFace(Player* player);
    void keyPressEvent(QKeyEvent* event);
    int heightFromPos(int pos);
    int widthFromPos(int pos);
    GameUI* m_parent;
    CommunicationHandler* m_clientCom;
    Map* m_map;
    QGridLayout* m_mainPanel;
    QGraphicsScene* m_mapScene;
    QGraphicsView* m_mapView;
    QList<QGraphicsRectItem*> m_mapItems;
    int m_inGame;
    int m_playerPosition;
    QList<Player*> m_playerList;
    QList<QLabel*> m_playerDisplay;
    QVBoxLayout* m_mapRight;
    bool m_gameStarted;
    QGraphicsSimpleTextItem* m_mapText;
    QList<QString> m_startText;
    QTimer m_timer;
    int m_playerLocation;
};

#endif	/* _GAMEFIELDUI_H */


