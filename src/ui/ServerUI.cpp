/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        ServerUI.cpp
 *  Description: Server management layout.
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

#include "ui/ServerUI.h"

ServerUI::ServerUI(QWidget* parent) {
    m_parent = parent;
    m_server = new QTcpServer(this);
    m_isServerRunning = false;

    m_botsName << "Lady Bomber" << "Romus Bomber" << "Ranger Bomber";
    m_botsColor << QColor(240, 44, 242) << QColor(174, 2, 0) << QColor(67, 99, 0);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/qtal-icon"));
    m_trayIcon->setToolTip(tr("BomberMan server"));

    m_serverPanelLeft = new QFormLayout;
    m_serverPanelLeft->setRowWrapPolicy(QFormLayout::WrapAllRows);

    m_gameName = new QLineEdit;
    m_serverPanelLeft->addRow(tr("Game name:"), m_gameName);

    m_playerNb = new QSpinBox;
    m_serverPanelLeft->addRow(tr("Player number:"), m_playerNb);
    m_playerNb->setRange(1, 4);
    m_playerNb->setValue(4);

    m_botNb = new QSpinBox;
    m_serverPanelLeft->addRow(tr("Bot number:"), m_botNb);
    m_botNb->setRange(0, 3);
    m_botNb->setValue(0);

    m_serverPort = new QSpinBox;
    m_serverPanelLeft->addRow(tr("Server port:"), m_serverPort);
    m_serverPort->setRange(0, 65535);
    m_serverPort->setValue(31415);

    m_serverPanelRight = new QFormLayout;
    m_serverPanelRight->setRowWrapPolicy(QFormLayout::WrapAllRows);

    m_gameMapView = new QListView;
    m_serverPanelRight->addRow(tr("Game map:"), m_gameMapView);
    m_gameMapView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_gameMapView->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_mapInfos = new QGroupBox(tr("Map information"));
    m_serverPanelRight->addRow(m_mapInfos);

    m_mapInfosLayout = new QFormLayout;
    m_mapName = new QLabel;
    m_mapAuthor = new QLabel;
    m_mapDesc = new QLabel;
    m_mapInfosLayout->addRow(tr("Name:"), m_mapName);
    m_mapInfosLayout->addRow(tr("Author:"), m_mapAuthor);
    m_mapInfosLayout->addRow(tr("Description:"), m_mapDesc);
    m_mapInfos->setLayout(m_mapInfosLayout);

    m_gameMapModel = new QStandardItemModel;
    m_gameMapView->setModel(m_gameMapModel);

    m_backButton = new QPushButton(tr("Back"));
    m_startStopButton = new QPushButton(tr("Start"));
    m_minimizeButton = new QPushButton(tr("Minimize"));
    m_minimizeButton->setDisabled(true);

    m_serverPanelMain = new QGridLayout;
    m_serverPanelMain->addLayout(m_serverPanelLeft, 0, 0);
    m_serverPanelMain->setColumnMinimumWidth(0, 200);
    m_serverPanelMain->addLayout(m_serverPanelRight, 0, 1, 1, 2);
    m_serverPanelMain->setColumnStretch(1, 1);
    m_serverPanelMain->addWidget(m_backButton, 1, 0, Qt::AlignLeft);
    m_serverPanelMain->addWidget(m_minimizeButton, 1, 1, Qt::AlignLeft);
    m_serverPanelMain->addWidget(m_startStopButton, 1, 2, Qt::AlignRight);

    setLayout(m_serverPanelMain);
    loadMaps();
    loadServerSettings();

    connect(m_playerNb, SIGNAL(valueChanged(int)), this, SLOT(onPlayerNbChange(int)));
    connect(m_botNb, SIGNAL(valueChanged(int)), this, SLOT(onBotNbChange(int)));
    connect(m_backButton, SIGNAL(clicked()), m_parent, SLOT(displayStartUI()));
    connect(m_backButton, SIGNAL(clicked()), this, SLOT(dumpServerSettings()));
    connect(m_minimizeButton, SIGNAL(clicked()), this, SLOT(hideUI()));
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(showUI(QSystemTrayIcon::ActivationReason)));
    connect(m_gameMapView, SIGNAL(clicked(QModelIndex)), this,
        SLOT(displayMapInfos(QModelIndex)));
}

QString ServerUI::layoutTitle() {
    return tr("BomberMan Game - Host a game");
}

QStringList ServerUI::serverInfos() {
    QStringList infos;
    infos << m_gameName->text() << m_mapName->text();
    infos << QString::number(m_playerList.count() - m_freePlayerSlots - m_botNb->value());
    infos << QString::number(m_playerNb->value());
    infos << QString::number(m_botNb->value());
    return infos;
}

QList<Player*> ServerUI::playerList() {
    return m_playerList;
}

QList<int> ServerUI::playerKeys() {
    return m_playerKeys;
}

/************************************************
 * SLOTS
 ***********************************************/

void ServerUI::hideUI() {
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, tr("Systray"),
            tr("Couldn't detect any system tray on this system."));
    }
    else {
        m_trayIcon->setVisible(true);
        m_parent->setVisible(false);
    }
}

void ServerUI::showUI(QSystemTrayIcon::ActivationReason) {
    m_parent->setVisible(true);
    m_trayIcon->setVisible(false);
}

void ServerUI::onPlayerNbChange(int players) {
    if ((m_botNb->value() + players) > 4) {
        m_botNb->setValue(4 - players);
    }
    else if (players && !m_botNb->value()) {
        m_botNb->setValue(1);
    }
}

void ServerUI::onBotNbChange(int bots) {
    if ((m_playerNb->value() + bots) > 4) {
        m_playerNb->setValue(4 - bots);
    }
}

void ServerUI::checkServerReady() {
    if (!m_isServerRunning) {
        bool serverReady = false;

        if (m_mapList.size() > 0)
            serverReady = true;

        if (!serverReady) {
            m_startStopButton->setText(tr("Refresh"));
            m_minimizeButton->setDisabled(true);
            m_gameName->setDisabled(true);
            m_serverPort->setDisabled(true);
            m_gameMapView->setDisabled(true);
            m_playerNb->setDisabled(true);
            m_botNb->setDisabled(true);

            connect(m_startStopButton, SIGNAL(clicked()), this, SLOT(checkServerReady()));

            QMessageBox::warning(0, tr("Server warning"),
                tr("No <em>'maps'</em> directory or no valid map found. Please download and install maps, "
                "then click refresh or restart the game"));
        }
        else {
            m_startStopButton->setText(tr("Start"));
            m_minimizeButton->setEnabled(false);
            m_gameName->setEnabled(true);
            m_gameMapView->setEnabled(true);
            m_serverPort->setEnabled(true);
            m_playerNb->setEnabled(true);
            m_botNb->setEnabled(true);

            connect(m_startStopButton, SIGNAL(clicked()), this, SLOT(changeServerStatus()));
        }
    }
}

bool ServerUI::loadMaps() {
    QDir mapsDir;

    if (!mapsDir.cd(QDir::currentPath() + MAPS_DIR))
        return false;

    QStringList mapsExt("*.xml");
    mapsDir.setFilter(QDir::Files);
    mapsDir.setNameFilters(mapsExt);

    QFileInfoList mapsList = mapsDir.entryInfoList();

    m_mapList.clear();
    m_mapItem.clear();
    m_gameMapModel->clear();
    foreach (QFileInfo map, mapsList) {
        if (!map.isReadable())
            continue;

        GameMap* gameMap = new GameMap(map);
        if (!gameMap->isValid())
            continue;
        
        m_mapList.append(gameMap);

        QStandardItem* mapName = new QStandardItem(gameMap->mapName());
        mapName->setEditable(false);
        m_mapItem.append(mapName);
        m_gameMapModel->insertRow(m_gameMapModel->rowCount(), mapName);
    }

    return true;
}

void ServerUI::displayMapInfos(QModelIndex index) {
    qDebug() << "Display map infos at" << index.row();
    GameMap* gameMap = m_mapList.at(index.row());
    m_selecteMapPath = gameMap->mapLocation();
    m_mapAuthor->setText(gameMap->mapAuthor());
    m_mapName->setText(gameMap->mapName());
    m_mapDesc->setText(gameMap->mapDescription());
}

void ServerUI::loadServerSettings() {
    qDebug() << "Loading server settings";

    QFile settings(QDir::currentPath() + "/server.qtal");

    if (!settings.exists())
        return;

    settings.open(QIODevice::ReadOnly);
    QDataStream in(&settings);
    in.setVersion(QDataStream::Qt_4_6);

    quint32 magic;
    in >> magic;

    if (magic != SERVER_SETTINGS_MAGIC)
        return;

    QString gameName;
    in >> gameName;
    m_gameName->setText(gameName);

    int playersNb;
    in >> playersNb;
    m_playerNb->setValue(playersNb);

    int botsNb;
    in >> botsNb;
    m_botNb->setValue(botsNb);

    int serverPort;
    in >> serverPort;
    m_serverPort->setValue(serverPort);

    QString selectedMapPath;
    in >> selectedMapPath;

    for (int i = 0; i < m_mapList.length(); i++) {
        qDebug() << m_mapList.at(i)->mapLocation();
        if (m_mapList.at(i)->mapLocation() == selectedMapPath) {
            QModelIndex mapIdx = m_gameMapModel->index(i, 0);
            m_gameMapView->setCurrentIndex(mapIdx);
            displayMapInfos(mapIdx);
            break;
        }
    }
}

void ServerUI::dumpServerSettings() {
    qDebug() << "Dumping server settings";

    QFile settings(QDir::currentPath() + "/server.qtal");

    if (!settings.open(QIODevice::WriteOnly))
        return;

    QDataStream out(&settings);
    out.setVersion(QDataStream::Qt_4_6);

    out << (quint32)SERVER_SETTINGS_MAGIC;
    out << m_gameName->text();
    out << m_playerNb->value();
    out << m_botNb->value();
    out << m_serverPort->value();
    out << m_selecteMapPath;
}

void ServerUI::addPlayer(Player* player, int key, int position) {
    qDebug() << "Adding a new player at" << position;

    m_freePlayerSlots--;
    m_playerList.replace(position, player);
    m_playerKeys.replace(position, key);
}

void ServerUI::removePlayer(ConnectionHandler* clientCon) {
    qDebug() << "Removing player";

    if (!clientCon) {
        qDebug() << "Time exceeded";
        QTimer* timer = qobject_cast<QTimer *>(sender());
        clientCon = qobject_cast<ConnectionHandler*>(timer->parent());
    }
    m_playerList.removeAll(clientCon->player());
    m_playerKeys.removeAll(clientCon->playerKey());
    m_freePlayerSlots++;
    clientCon->closeConnection();
}

void ServerUI::distributePlayers() {
        QMap<int, int> unsortedDistribution;
        // Player distribution
        for (int i = 0; i < (m_playerNb->value() + m_botNb->value()); i++) {
            unsortedDistribution.insert(qrand(), i);
        }

        m_playerDistribution.clear();
        for (int i = 0; i < (m_playerNb->value() + m_botNb->value()); i++) {
            m_playerDistribution.insert(i, unsortedDistribution.lowerBound(0).value());
            unsortedDistribution.remove(unsortedDistribution.lowerBound(0).key());
        }

        qDebug() << "Player distribution:" << m_playerDistribution;
}

void ServerUI::initPlayerList() {
    qDebug() << "Initializing player list";
    m_playerList.clear();
    m_playerKeys.clear();

    for (int i = 0; i < m_playerDistribution.count(); i++) {
        Player* player = new Player;
        m_playerList.append(player);
        m_playerKeys.append(qrand());
    }

    for (int i = 0; i < m_playerDistribution.count(); i++) {
        int playerPos = m_playerDistribution.at(i);

        QByteArray* avatar;
        if (playerPos < m_botNb->value()) {
            avatar = new QByteArray(QFile(tr(":/qtal-bot-%1").arg(playerPos)).readAll());
            m_playerList.at(playerPos)->setPlayerColor(m_botsColor.at(playerPos));
            m_playerList.at(playerPos)->setPlayerPseudo(m_botsName.at(playerPos));
            m_playerList.at(playerPos)->setPlayerType(PLAYER_IS_BOT);
            qDebug() << "Bot" << m_botsName.at(playerPos) << "at" << playerPos;
        }
        else {
            avatar = new QByteArray(QFile(":/qtal-wait").readAll());
            m_playerList.at(playerPos)->setPlayerPseudo(tr("Waiting..."));
            m_playerList.at(playerPos)->setPlayerColor(Qt::black);
            m_playerList.at(playerPos)->setPlayerType(PLAYER_IS_NONE);
            qDebug() << "Player"  << playerPos << "initialized";
        }

        m_playerList.at(playerPos)->setPlayerAvatar(avatar);
        m_playerList.at(playerPos)->setPlayerPosition(playerPos);
    }

    qDebug() << "Key list:" << m_playerKeys;
}

void ServerUI::reinitPlayer(int position) {
    qDebug() << "Re-initializing player at" << position;
    Player* player = m_playerList.at(position);
    player->setPlayerPseudo(tr("Waiting..."));
    player->setPlayerColor(Qt::black);
    QByteArray* avatar = new QByteArray(QFile(":/qtal-wait").readAll());
    player->setPlayerAvatar(avatar);
    player->setPlayerType(PLAYER_IS_NONE);
    m_playerKeys.replace(position, qrand());
}

int ServerUI::freePlayerSlots() {
    return m_freePlayerSlots;
}

int ServerUI::gameStarted() {
    return m_gameStarted;
}

QList<int> ServerUI::playersDistribution() {
    return m_playerDistribution;
}

void ServerUI::sendPlayerToOther(int playerPosition) {
    qDebug() << "Sending update for player at" << playerPosition;
    for (int i = 0; i < m_clientList.count(); i++) {
        int currentPlayerIdx = m_playerList.indexOf(m_clientList.at(i)->player());
        if (currentPlayerIdx == -1 || currentPlayerIdx == playerPosition)
            continue;
        else if (m_playerList.at(currentPlayerIdx)->playerType() == PLAYER_IS_REAL) {
            qDebug() << "Update send at" << m_playerList.at(currentPlayerIdx)->playerPseudo();
            m_clientList.at(i)->sendPlayerInfo(playerPosition);
        }
    }
}

void ServerUI::notifyAllGameStart() {
    qDebug() << "Starting game";

    QTimer::singleShot(6000, this, SLOT(onGameStart()));
    m_gameStarted = GAME_COUNTDOWN;

    for (int i = 0; i < m_clientList.count(); i++) {
        int currentPlayerIdx = m_playerList.indexOf(m_clientList.at(i)->player());
        if (currentPlayerIdx == -1)
            continue;
        else if (m_playerList.at(currentPlayerIdx)->playerType() == PLAYER_IS_REAL) {
            qDebug() << "Start message send at" << m_playerList.at(currentPlayerIdx)->playerPseudo();
            m_clientList.at(i)->notifyGameStart();
        }
    }
}

void ServerUI::onGameStart() {
    qDebug() << "Game is started!";
    m_gameStarted = GAME_STARTED;
}

void ServerUI::playerMoved(int playerIdx, int position) {
    qDebug() << "Telling other a player moved";
    m_map->setPlayerPosition(playerIdx+1, position);

    for (int i = 0; i < m_clientList.count(); i++) {
        int currentPlayerIdx = m_playerList.indexOf(m_clientList.at(i)->player());
        if (currentPlayerIdx == -1)
            continue;

        m_clientList.at(i)->sendPlayerMove(playerIdx, position);
    }
    
}

/*
 * Server management
 */

void ServerUI::changeServerStatus() {
    // TODO: verify that a map is selected
    m_gameName->setText(m_gameName->text().trimmed());
    if (m_gameName->text().length() < 3) {
        QMessageBox::information(this, tr("Server warning"),
            tr("The game name have to be at least three letter long."));
        return;
    }

    m_isServerRunning ? stopServer() : startServer();
}

void ServerUI::startServer() {
    if (!m_isServerRunning) {
        if (!m_server->listen(QHostAddress::Any, m_serverPort->value())) {
            QMessageBox::critical(this, tr("Unable to start the server:\n%1").arg(m_server->errorString()),
                    tr("Server error"));
            return;
        }

        qDebug() << "Starting server";

        m_isServerRunning = true;
        m_startStopButton->setText(tr("Stop"));
        m_minimizeButton->setDisabled(false);
        m_gameName->setDisabled(true);
        m_serverPort->setDisabled(true);
        m_gameMapView->setDisabled(true);
        m_playerNb->setDisabled(true);
        m_botNb->setDisabled(true);

        m_freePlayerSlots = m_playerNb->value();
        m_gameStarted = GAME_PENDING;
        m_playerList.clear();
        m_playerKeys.clear();
        m_playerDistribution.clear();
        m_clientList.clear();

        dumpServerSettings();
        distributePlayers();
        initPlayerList();

        QModelIndexList selectedMap = m_gameMapView->selectionModel()->selectedIndexes();
        GameMap* currentMap = m_mapList.at(selectedMap.at(0).row());
        m_map = new Map(currentMap->mapContent(), currentMap->heigh(), currentMap->width());
        
        connect(m_server, SIGNAL(newConnection()), this, SLOT(manageConnection()));
    }
}

void ServerUI::stopServer() {
    if (m_isServerRunning) {
        qDebug() << "Stopping server";

        if (m_server->isListening()) {
            for (int i = 0; i < m_clientList.size(); i++) {
                m_clientList.at(i)->closeConnection();
            }
            m_server->close();
        }

        m_isServerRunning = false;
        m_gameStarted = GAME_PENDING;
        m_playerList.clear();
        m_playerKeys.clear();
        m_playerDistribution.clear();
        m_clientList.clear();
        m_startStopButton->setText(tr("Start"));
        m_minimizeButton->setEnabled(false);
        m_gameName->setEnabled(true);
        m_gameMapView->setEnabled(true);
        m_serverPort->setEnabled(true);
        m_playerNb->setEnabled(true);
        m_botNb->setEnabled(true);

        disconnect(m_server, SIGNAL(newConnection()), this, SLOT(manageConnection()));
    }
}

void ServerUI::manageConnection() {
    qDebug() << "New client connected";
    QTcpSocket* socket = m_server->nextPendingConnection();

    if (!socket)
        return;

    ConnectionHandler* client = new ConnectionHandler(socket, this);
    m_clientList << client;

    connect(client, SIGNAL(connectionEnded()), this, SLOT(manageClient()));
    connect(client, SIGNAL(newPlayer(Player*, int, int)), this, SLOT(addPlayer(Player*, int, int)));
    connect(client, SIGNAL(sendPlayerUpdate(int)), this, SLOT(sendPlayerToOther(int)));
    connect(client, SIGNAL(playerMoved(int, int)), this, SLOT(playerMoved(int, int)));
}

void ServerUI::manageClient() {
    ConnectionHandler* client = qobject_cast<ConnectionHandler *>(sender());

    if (!client)
        return;

    qDebug() << "Remove client from list";

    client->disconnect();

    if (client->timer() != NULL && client->timer()->isActive())
        client->timer()->stop();

    if (m_playerList.indexOf(client->player()) != -1) {
        qDebug() << "Client was a player";
        if (!m_gameStarted) {
            qDebug() << "Re-initializing place";
            int playerIdx = m_playerList.indexOf(client->player());
            reinitPlayer(playerIdx);
            sendPlayerToOther(playerIdx);
        }
        // TODO: Set player offline, change the key and send status to other

        m_freePlayerSlots++;
    }
    m_clientList.removeAll(client);
}

Map* ServerUI::map() {
    return m_map;
}
