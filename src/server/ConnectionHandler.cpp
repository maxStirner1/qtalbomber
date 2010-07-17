/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        ConnectionHandler.cpp
 *  Description: Manage server connection with client.
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

#include "server/ConnectionHandler.h"
#include "ui/ServerUI.h"

ConnectionHandler::ConnectionHandler(QTcpSocket* socket, ServerUI* parent)
: QObject(parent), m_socket(socket), m_parent(parent) {
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(manageData()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(closeConnection()));

    m_timer = new QTimer(this);
    m_inGame = false;
}

void ConnectionHandler::manageData() {
    qDebug() << "Recieving data from client";
    QByteArray socketData = m_socket->readAll();
    QDataStream in(&socketData, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_6);

    quint16 blockSize;
    in >> blockSize;

    if (!blockSize)
        return;

    quint8 actionType;
    in >> actionType;

    if ((int)actionType == GAME_INFOS) {
        respondServerInfos();
    }
    else if ((int)actionType == JOIN_GAME) {
        m_player = new Player;

        if ((m_parent->freePlayerSlots() > 0) && !m_parent->gameStarted()) {
            QString pseudo;
            in >> pseudo;
            m_player->setPlayerPseudo(pseudo);
            qDebug() << "- Pseudo:" << pseudo;

            QColor color;
            in >> color;
            m_player->setPlayerColor(color);
            qDebug() << "- Color:" << color;
        }

        respondJoinGame();
    }
    else if ((int)actionType == GAME_MAP) {
        if (!m_inGame) {
            qDebug() << "Timer stopped";
            m_inGame = true;
            m_timer->stop();
            m_timer->setSingleShot(false);
            emit sendPlayerUpdate(m_playerPosition);

            if (!m_parent->freePlayerSlots())
                QTimer::singleShot(1000, m_parent, SLOT(notifyAllGameStart()));
        }
        qDebug() << "Game map requested";
        in >> m_playerKey;
        if (!isKeyValid())
            return;

        sendMap();
    }
    else if ((int)actionType == GAME_PLAYERS) {
        in >> m_playerKey;
        if (!isKeyValid())
            return;

        sendPlayerInfo();
    }
    else if ((int)actionType == GAME_PLAYER) {
        in >> m_playerKey;
        if (!isKeyValid())
            return;

        int playerId;
        in >> playerId;
        
        sendPlayerInfo(playerId);
    }
    else if (actionType == PLAYER_EVENT) {
        in >> m_playerKey;
        if (!isKeyValid())
            return;

        quint8 event;
        in >> event;

        int playerPos = m_parent->playerKeys().indexOf(m_playerKey);

        if (playerPos == -1)
            return;

        comupteEvent(event, playerPos);
    }
}

void ConnectionHandler::respondServerInfos() {
    qDebug() << "Sending server infos to client";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out << (quint16)0;
    out << (quint8)GAME_INFOS;
    out << m_parent->serverInfos();
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    m_socket->write(block);
}

void ConnectionHandler::respondJoinGame() {
    qDebug() << "Responding to join game request";

    m_playerKey = qrand();

    m_playerPosition = PLAYER_REFUSED;
    if ((m_parent->freePlayerSlots() > 0) && !m_parent->gameStarted()) {
        m_timer->setSingleShot(true);
        connect(m_timer, SIGNAL(timeout()), m_parent, SLOT(removePlayer()));
        m_timer->start(3000);

        m_playerPosition = m_parent->playersDistribution()
            .at(m_parent->playerList().count() - m_parent->freePlayerSlots());

        if (m_parent->playerList().at(m_playerPosition)->playerType() != PLAYER_IS_NONE) {
            for (int i = 0; i < m_parent->playerList().count(); i++, m_playerPosition = i) {
                if (m_parent->playerList().at(m_playerPosition)->playerType() == PLAYER_IS_NONE)
                    break;
            }
        }

        m_player->setPlayerPosition(m_playerPosition);
        m_player->setPlayerType(PLAYER_IS_REAL);
        
        qDebug() << "Admissible candidate:" << m_player->playerPseudo();

        emit newPlayer(m_player, m_playerKey, m_playerPosition);
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out << (quint16)0;
    out << (quint8)JOIN_GAME;
    out << m_playerPosition;
    out << m_playerKey;
    out << m_parent->playerKeys().count();
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    m_socket->write(block);
}

void ConnectionHandler::sendMap() {
    qDebug() << "Sending map";

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out << (quint16)0;
    out << (quint8)GAME_MAP;
    out << m_parent->map()->width();
    out << m_parent->map()->heigh();
    out << m_parent->map()->collisionsTab();
    out << m_parent->map()->unbreakableBlocks();
    out << m_parent->map()->breakableBlocks();
    out << m_parent->map()->bonusPositions();
    out << m_parent->map()->playersPositions();
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    m_socket->write(block);
}

void ConnectionHandler::sendPlayerInfo(int specificPlayer) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
   
    out << (quint16)0;

    // Remove own player
    QList<int> playersId;
    if (specificPlayer != -1) {
        qDebug() << "Sending specific player";
        playersId << specificPlayer;
        out << (quint8)GAME_PLAYER;
    }
    else {
        qDebug() << "Send player range";
        playersId << m_parent->playersDistribution();
        playersId.removeAll(m_parent->playerKeys().indexOf(m_playerKey));
        out << (quint8)GAME_PLAYERS;
    }

    for (int i = 0; i < playersId.count(); i++) {
        qDebug() << "Sending player info" << playersId.at(i);
        Player* currentPlayer = m_parent->playerList().at(playersId.at(i));

        out << currentPlayer->playerPseudo();
        out << currentPlayer->playerColor();
        out << currentPlayer->playerPosition();
        out << currentPlayer->playerType();
    }

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    m_socket->write(block);
}

void ConnectionHandler::sendPlayerMove(int playerIdx, int position) {
    qDebug() << "Player move to client";

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out << (quint16)0;
    out << (quint8)PLAYER_MOVE;
    out << playerIdx;
    out << position;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    m_socket->write(block);
}

void ConnectionHandler::notifyGameStart() {
    qDebug() << "Notify player game is starting";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out << (quint16)0;
    out << (quint8)GAME_START;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    m_socket->write(block);
}

bool ConnectionHandler::isKeyValid() {
    if (m_parent->playerKeys().indexOf(m_playerKey) == -1) {
            qDebug() << "Wrong player key";
            return false;
    }
    return true;
}

void ConnectionHandler::closeConnection() {
    if (m_socket->isOpen()) {
        qDebug() << "Closing connection with client";
        m_socket->close();
        m_socket->deleteLater();
        
        emit connectionEnded();
    }
}

void ConnectionHandler::comupteEvent(int event, int player) {
    if (m_parent->gameStarted() != 1)
        return;

    int playerPosition = m_parent->map()->playersPositions().indexOf(player+1);
    if (event == MOVE_LEFT) {
        if (!m_parent->map()->collisionsTab().at(playerPosition-1)) {
            playerPosition--;
        }
    }
    else if (event == MOVE_RIGHT) {
        if (!m_parent->map()->collisionsTab().at(playerPosition-1)) {
            playerPosition++;
        }
    }
    else if (event == MOVE_UP) {
        int newPos = playerPosition-m_parent->map()->width();
        if (newPos  >= 0 && !m_parent->map()->collisionsTab().at(newPos)) {
            playerPosition = newPos;
        }
    }
    else if (event == MOVE_DOWN) {
        int newPos = playerPosition+m_parent->map()->width();
        if (newPos < m_parent->map()->collisionsTab().count()
            && !m_parent->map()->collisionsTab().at(newPos)) {
            playerPosition = newPos;
        }
    }

    // TODO: Handle bomb and bonus
    emit playerMoved(player, playerPosition);
}

Player* ConnectionHandler::player() {
    return m_player;
}

int ConnectionHandler::playerKey() {
    return m_playerKey;
}

QTimer* ConnectionHandler::timer() {
    return m_timer;
}
