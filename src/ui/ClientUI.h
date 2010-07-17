/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        ClientUI.h
 *  Description: Header file for ClientUI class.
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

#ifndef _CLIENTUI_H
#define	_CLIENTUI_H

#include "constants.h"
#include "client/AddServerDialog.h"
#include "client/CommunicationHandler.h"
#include "game/Player.h"
#include "GameUI.h"
#include <QtGui>
#include <QtNetwork>
#include <QDebug>

class GameUI;

class ClientUI: public QWidget {
    Q_OBJECT
    
public:
    ClientUI(GameUI* parent);
    QString layoutTitle();
    void resetUI();

public slots:
    void dumpPlayerSettings();

private slots:
    void changePlayerAvatar(bool choose = true);
    void changePlayerColor(bool choose = true);
    void displayAddServerDialog();
    void addServer(QStringList serverProperties);
    void removeServer();
    void refreshServerInfos();
    void updateGameInfos(QStringList gameInfos);
    void errorGameInfos();
    void startGame();
    void onServerSelected(QModelIndex index);
    void serverOnline();
    
private:
    void loadPlayerSettings();
    void setPlayerColor(QColor color);
    void retrieveGameInfos(QStringList server, int identifier = -1);
    void manageServerListRow(int row, QStringList data);
    QStringList defaultServerData(QStringList serverProperties);
    GameUI* m_parent;
    QFormLayout* m_clientPanelRight;
    QFormLayout* m_clientPanelLeft;
    QGridLayout* m_clientPanelMain;
    QHBoxLayout* m_serverListActionBox;
    QPushButton* m_backButton;
    QPushButton* m_playButton;
    QLineEdit* m_playerName;
    QPushButton* m_playerAvatarButton;
    QPushButton* m_playerColorButton;
    QTreeView* m_serverTable;
    QStandardItemModel* m_serverTableItems;
    QList<QStringList> m_serverList;
    QPushButton* m_addServerButton;
    QPushButton* m_removeServerButton;
    QPushButton* m_refreshServerButton;
    QColor m_playerColor;
    QString m_playerAvatar;
    AddServerDialog* m_addServerDialog;
    QList<CommunicationHandler*> m_clientsCom;
    QList<bool> m_serverOnline;
    int m_gameCom;
};

#endif	/* _CLIENTUI_H */

