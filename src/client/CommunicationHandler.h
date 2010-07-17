/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        CommunicationHandler.h
 *  Description: Header file for CommunicationHandler class.
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

#ifndef _COMMUNICATIONHANDLER_H
#define	_COMMUNICATIONHANDLER_H

#include "constants.h"
#include "game/Player.h"
#include "game/Map.h"
#include <QtNetwork>

class CommunicationHandler : public QObject {
    Q_OBJECT
    
public:
    CommunicationHandler(QObject* parent = 0);
    CommunicationHandler(QString address, quint16 port, int action, int identifier = -1, QObject* parent = 0);
    void setHost(QString address, quint16 port);
    void setAction(int actionType);
    int identifier();
    bool waitingForTransaction();
    void beginTransaction(int identifier = -1);
    void closeConnection();
    void setPlayer(Player* player);
    void setPlayerKey(int key);
    Player* player();
    void sendEvent(int event);

signals:
    void comError();
    void socketFree();
    void gameInfos(QStringList serverInfos);
    void gameJoined(int position);
    void newGameMap(int, int, QList<int>, QList<int>, QList<int>, QList<int>, QList<int>);
    void playerRefused();
    void playerDisconnected();
    void initPlayers(int playerNb);
    void playerUpdated(Player*);
    void gameStart();
    void rivalMoved(int playerIdx, int position);

public slots:
    void runAction();

private slots:
    void manageData();
    void socketError(QAbstractSocket::SocketError);
    void onDisconnected();

private:
    void gameInfosRequest();
    void gameInfosResponse();
    void joinGameRequest();
    void joinGameResponse(int place);
    void gameMapRequest();
    void gameMapResponse();
    void gamePlayersRequest();
    QTcpSocket* m_socket;
    QString m_address;
    quint16 m_port;
    int m_action;
    int m_identifier;
    bool m_inTransaction;
    Player* m_player;
    int m_playerKey;
    QStringList m_serverInfos;
    int m_playerNb;
};

#endif	/* _COMMUNICATIONHANDLER_H */

