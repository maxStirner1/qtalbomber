/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        ConnectionHandler.h
 *  Description: Header file for ConnectionHandler class.
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

#ifndef _CONNECTIONHANDLER_H
#define	_CONNECTIONHANDLER_H

#include "constants.h"
#include "game/Player.h"
#include <QtNetwork>

class ServerUI;

class ConnectionHandler : public QObject {
    Q_OBJECT

public:
    ConnectionHandler(QTcpSocket* socket, ServerUI* parent);
    Player* player();
    int playerKey();
    void sendMap();
    QTimer* timer();
    void sendPlayerInfo(int specificPlayer = -1);
    void notifyGameStart();
    void sendPlayerMove(int playerIdx, int position);

signals:
    void connectionEnded();
    void newPlayer(Player* player, int key, int position);
    void sendPlayerUpdate(int playerPosition);
    void playerMoved(int playerIdx, int newPosition);

public slots:
    void closeConnection();

private slots:
    void manageData();

private:
    void respondServerInfos();
    void respondJoinGame();
    bool isKeyValid();
    void comupteEvent(int event, int player);
    QTcpSocket* m_socket;
    ServerUI* m_parent;
    Player* m_player;
    int m_playerKey;
    int m_playerPosition;
    QTimer* m_timer;
    bool m_inGame;
};

#endif	/* _CONNECTIONHANDLER_H */

