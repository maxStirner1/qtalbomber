/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        ServerUI.h
 *  Description: Header file for ServerUI class.
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

#ifndef _SERVERUI_H
#define	_SERVERUI_H

#include "constants.h"
#include "server/ConnectionHandler.h"
#include "server/GameMap.h"
#include "game/Map.h"
#include <QtGui>
#include <QDebug>

class ServerUI: public QWidget {
    Q_OBJECT

public:
    ServerUI(QWidget* parent);
    QString layoutTitle();
    QStringList serverInfos();
    Map* map();
    QList<Player*> playerList();
    QList<int> playerKeys();
    QList<int> playersDistribution();
    int freePlayerSlots();
    int gameStarted();

public slots:
    void checkServerReady();
    void dumpServerSettings();
    void addPlayer(Player* player, int key, int position);
    void removePlayer(ConnectionHandler* clientCon = 0);
    void notifyAllGameStart();

private slots:
    void hideUI();
    void showUI(QSystemTrayIcon::ActivationReason reason);
    void onBotNbChange(int bots);
    void onPlayerNbChange(int players);
    void changeServerStatus();
    void manageConnection();
    void stopServer();
    void manageClient();
    void displayMapInfos(QModelIndex index);
    void sendPlayerToOther(int playerPosition);
    void onGameStart();
    void playerMoved(int playerIdx, int position);

private:
    void loadServerSettings();
    void startServer();
    bool loadMaps();
    void distributePlayers();
    void initPlayerList();
    void reinitPlayer(int position);
    int m_gameStarted;
    QStringList m_botsName;
    QList<QColor> m_botsColor;
    int m_freePlayerSlots;
    Map* m_map;
    QWidget* m_parent;
    QFormLayout* m_serverPanelLeft;
    QFormLayout* m_serverPanelRight;
    QGridLayout* m_serverPanelMain;
    QPushButton* m_backButton;
    QPushButton* m_minimizeButton;
    QPushButton* m_startStopButton;
    QSystemTrayIcon* m_trayIcon;
    QLineEdit* m_gameName;
    QSpinBox* m_serverPort;
    QSpinBox* m_playerNb;
    QSpinBox* m_botNb;
    QGroupBox* m_mapInfos;
    QFormLayout* m_mapInfosLayout;
    QLabel* m_mapName;
    QLabel* m_mapAuthor;
    QLabel* m_mapDesc;
    QListView* m_gameMapView;
    QStandardItemModel* m_gameMapModel;
    QString m_selecteMapPath;
    QList<QStandardItem *> m_mapItem;
    QList<GameMap *> m_mapList;
    bool m_isServerRunning;
    QTcpServer* m_server;
    QList<ConnectionHandler*> m_clientList;
    QList<Player*> m_playerList;
    QList<int> m_playerKeys;
    QList<int> m_playerDistribution;
};

#endif	/* _SERVERUI_H */

