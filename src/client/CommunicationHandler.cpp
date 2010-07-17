/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        CommunicationHandler.cpp
 *  Description: Manage client connection with server.
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

#include "client/CommunicationHandler.h"

CommunicationHandler::CommunicationHandler(QObject* parent) : QObject(parent) {
    m_socket = new QTcpSocket(this);
    m_playerKey = 0;
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(manageData()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
        SLOT(socketError(QAbstractSocket::SocketError)));
}

CommunicationHandler::CommunicationHandler(QString address, quint16 port,
    int action, int identifier, QObject* parent)
: QObject(parent) {
    m_socket = new QTcpSocket(this);
    
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(manageData()));

    setHost(address, port);
    setAction(action);
    beginTransaction(identifier);
}

void CommunicationHandler::setHost(QString address, quint16 port) {
    m_address = address;
    m_port = port;
}

void CommunicationHandler::setAction(int actionType) {
    m_action = actionType;
}

void CommunicationHandler::beginTransaction(int identifier) {
    m_inTransaction = true;
    m_identifier = identifier;

    if (m_socket->state() != QTcpSocket::ConnectedState) {
        m_socket->close();
        m_socket->disconnectFromHost();
    }

    m_socket->connectToHost(m_address, m_port);

    connect(m_socket, SIGNAL(connected()), this, SLOT(runAction()));
}

void CommunicationHandler::manageData() {
    qDebug() << "Recieving data from server";
    QByteArray socketData = m_socket->readAll();
    QDataStream in(&socketData, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_6);

    quint16 blockSize;
    in >> blockSize;

    if (!blockSize)
        return;

    quint8 actionType;
    in >> actionType;

    if (actionType == GAME_INFOS) {
        in >> m_serverInfos;
        gameInfosResponse();
    }
    else if (actionType == JOIN_GAME) {
        int playerPlace;
        in >> playerPlace >> m_playerKey >> m_playerNb;
        emit initPlayers(m_playerNb);
        joinGameResponse(playerPlace);
    }
    else if (actionType == GAME_MAP) {
        qDebug() << "Game map recieved";

        int width;
        int heigh;
        QList<int> collisionsTab;
        QList<int> unbreakableBlocks;
        QList<int> breakableBlocks;
        QList<int> bonusPositions;
        QList<int> playersPositions;

        in >> width >> heigh >> collisionsTab >> unbreakableBlocks;
        in >> breakableBlocks >> bonusPositions >> playersPositions;

        qDebug() << "Emitting retrieved value";
        emit newGameMap(width, heigh, collisionsTab, unbreakableBlocks,
            breakableBlocks, bonusPositions, playersPositions);
        emit socketFree();
    }
    else if (actionType == GAME_PLAYER || (int)actionType == GAME_PLAYERS) {
        qDebug() << "Game player(s) recieved";

        int loopTime = 2;
        if (actionType == GAME_PLAYERS)
            loopTime = m_playerNb;

        for (int i = 0; i < loopTime-1; i++) {
            Player* player = new Player;
            QString pseudo;
            QColor color;
            int position;
            int type;

            in >> pseudo >> color >> position >> type;

            qDebug() << "- Pseudo:" << pseudo;
            qDebug() << "- Color:" << color;
            qDebug() << "- Position:" << position;
            qDebug() << "- type:" << type;
            player->setPlayerPseudo(pseudo);
            player->setPlayerColor(color);
            player->setPlayerPosition(position);
            player->setPlayerType(type);

            emit playerUpdated(player);
        }
    }
    else if (actionType == GAME_START) {
        qDebug() << "Server notify game start";
        emit gameStart();
    }
    else if (actionType == PLAYER_MOVE) {
        int playerIdx;
        in >> playerIdx;
        int playerPos;
        in >> playerPos;

        emit rivalMoved(playerIdx, playerPos);
    }
}

void CommunicationHandler::runAction() {
    qDebug() << "Connected to server";
    disconnect(m_socket, SIGNAL(connected()), this, SLOT(runAction()));

    qDebug() << "Performing action" << m_action;

    switch (m_action) {
        case GAME_INFOS:
            gameInfosRequest();
            break;
        case JOIN_GAME:
            joinGameRequest();
            break;
        case GAME_MAP:
            gameMapRequest();
            break;
        case GAME_PLAYERS:
            gamePlayersRequest();
            break;
        default:
            return;
    }
}

void CommunicationHandler::gameInfosRequest() {
    qDebug() << "Requesting game infos";
    
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out << (quint16)0;
    out << (quint8)GAME_INFOS;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    m_socket->write(block);
}

void CommunicationHandler::gameInfosResponse() {
    qDebug() << "Server returned game infos";

    emit gameInfos(m_serverInfos);
    closeConnection();
    emit socketFree();
}

void CommunicationHandler::joinGameRequest() {
    qDebug() << "Sending join request";

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out << (quint16)0;
    out << (quint8)JOIN_GAME;
    out << m_player->playerPseudo();
    out << m_player->playerColor();
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    m_socket->write(block);
}

void CommunicationHandler::joinGameResponse(int position) {
    qDebug() << "Join game response recieved";
    if (position == PLAYER_REFUSED) {
        qDebug() << "Client refused!";          
        closeConnection();
        emit playerRefused();
        emit socketFree();
    }
    else {
        qDebug() << "Client accepted in game at" << position;
        emit gameJoined(position);
    }
}

void CommunicationHandler::gameMapRequest() {
    qDebug() << "Requesting game map";
    if (!m_playerKey) {
        qDebug() << "No key found";
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out << (quint16)0;
    out << (quint8)GAME_MAP;
    out << m_playerKey;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    m_socket->write(block);
}

void CommunicationHandler::gamePlayersRequest() {
    qDebug() << "Requesting game players";
    if (!m_playerKey) {
        qDebug() << "No key found";
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out << (quint16)0;
    out << (quint8)GAME_PLAYERS;
    out << m_playerKey;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    m_socket->write(block);
}

void CommunicationHandler::sendEvent(int event) {
    qDebug() << "Sending event to server";
    if (!m_playerKey) {
        qDebug() << "No key found";
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out << (quint16)0;
    out << (quint8)PLAYER_EVENT;
    out << m_playerKey;
    out << (quint8)event;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    m_socket->write(block);
}

bool CommunicationHandler::waitingForTransaction() {
    return m_inTransaction;
}

int CommunicationHandler::identifier() {
    return m_identifier;
}

void CommunicationHandler::closeConnection() {
    if (m_socket->isOpen()) {
        qDebug() << "Closing connection with server";
        m_socket->close();
        m_socket->disconnectFromHost();
    }
}

void CommunicationHandler::socketError(QAbstractSocket::SocketError) {
    qDebug() << "Communication error with server:" << m_socket->errorString();
    emit comError();
}

void CommunicationHandler::setPlayer(Player* playerInfos) {
    m_player = playerInfos;
}

Player* CommunicationHandler::player() {
    return m_player;
}

void CommunicationHandler::onDisconnected() {
    emit playerDisconnected();
}