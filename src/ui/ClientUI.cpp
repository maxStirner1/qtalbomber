/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        ClientUI.cpp
 *  Description: Client layout management.
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

#include "ui/ClientUI.h"
#include "ui/GameUI.h"

ClientUI::ClientUI(GameUI* parent) : QWidget(parent) {
    m_parent = parent;
    m_addServerDialog = new AddServerDialog(this);

    m_clientPanelLeft = new QFormLayout;
    m_clientPanelLeft->setRowWrapPolicy(QFormLayout::WrapAllRows);
    m_playerName = new QLineEdit;
    m_playerAvatarButton = new QPushButton(tr("No avatar"));
    m_playerAvatarButton->setToolTip(tr("Click to modify your avatar"));
    m_playerAvatarButton->setFixedSize(120, 120);
    m_playerColorButton = new QPushButton;
    m_playerColorButton->setFixedSize(70, 70);
    m_backButton = new QPushButton(tr("Back"));

    m_clientPanelLeft->addRow(tr("Pseudo:"), m_playerName);
    m_clientPanelLeft->addRow(tr("Avatar:"), m_playerAvatarButton);
    m_clientPanelLeft->addRow(tr("Character color:"), m_playerColorButton);

    m_clientPanelRight = new QFormLayout;
    m_clientPanelRight->setRowWrapPolicy(QFormLayout::WrapAllRows);

    m_serverTable = new QTreeView;
    m_serverTable->setRootIsDecorated(false);
    m_serverTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_serverTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_serverTableItems = new QStandardItemModel;
    m_serverTable->setModel(m_serverTableItems);

    QStringList serverListHeader;
    serverListHeader << "Game name" << "Map" << "Players" << "Bots";
    m_serverTableItems->setHorizontalHeaderLabels(serverListHeader);

    m_serverListActionBox = new QHBoxLayout;
    m_addServerButton = new QPushButton(tr("Add"));
    m_refreshServerButton = new QPushButton(tr("Refresh"));
    m_removeServerButton = new QPushButton(tr("Remove"));
    m_playButton = new QPushButton(tr("Play!"));
    m_playButton->setDisabled(true);

    m_clientPanelRight->addRow(tr("Avalaible servers:"), m_serverTable);
    m_serverListActionBox->addWidget(m_addServerButton);
    m_serverListActionBox->addWidget(m_refreshServerButton);
    m_serverListActionBox->addWidget(m_removeServerButton);
    m_clientPanelRight->addRow(m_serverListActionBox);

    m_clientPanelMain = new QGridLayout;
    m_clientPanelMain->addLayout(m_clientPanelLeft, 0, 0);
    m_clientPanelMain->setColumnMinimumWidth(0, 180);
    m_clientPanelMain->addLayout(m_clientPanelRight, 0, 1);
    m_clientPanelMain->setColumnStretch(1, 1);
    m_clientPanelMain->addWidget(m_backButton, 1, 0, Qt::AlignLeft);
    m_clientPanelMain->addWidget(m_playButton, 1, 1, Qt::AlignRight);

    setLayout(m_clientPanelMain);

    m_playerColor = Qt::green;
    setPlayerColor(m_playerColor);
    loadPlayerSettings();

    connect(m_playerAvatarButton, SIGNAL(clicked()), this, SLOT(changePlayerAvatar()));
    connect(m_playerColorButton, SIGNAL(clicked()), this, SLOT(changePlayerColor()));
    connect(m_addServerButton, SIGNAL(clicked()), this, SLOT(displayAddServerDialog()));
    connect(m_addServerDialog, SIGNAL(serverAdded(QStringList)), this, SLOT(addServer(QStringList)));
    connect(m_removeServerButton, SIGNAL(clicked()), this, SLOT(removeServer()));
    connect(m_playButton, SIGNAL(clicked()), this, SLOT(startGame()));
    connect(m_backButton, SIGNAL(clicked()), this, SLOT(dumpPlayerSettings()));
    connect(m_backButton, SIGNAL(clicked()), m_parent, SLOT(displayStartUI()));
    connect(m_refreshServerButton, SIGNAL(clicked()), this, SLOT(refreshServerInfos()));
    connect(m_serverTable, SIGNAL(clicked(QModelIndex)), this, SLOT(onServerSelected(QModelIndex)));
}

QString ClientUI::layoutTitle() {
    return tr("BomberMan Game - Join a game");
}

void ClientUI::setPlayerColor(QColor color) {
    QString style = "background-color: rgb(%1, %2, %3);";
    m_playerColorButton->setStyleSheet(style.arg(color.red()).arg(color.green()).arg(color.blue()));
}

/************************************************
 * SLOTS
 ***********************************************/

void ClientUI::changePlayerAvatar(bool choose) {
    if (choose) {
        m_playerAvatar = QFileDialog::getOpenFileName(this, tr("Avatar selection"),
            QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));
    }
    
    QFile avatar(m_playerAvatar);
    if (avatar.exists()) {
        m_playerAvatarButton->setIcon(QIcon(m_playerAvatar));
        m_playerAvatarButton->setIconSize(QSize(115, 115));
    }
    
    if (m_playerAvatar.length() == 0) {
        m_playerAvatarButton->setText(tr("No avatar"));
        m_playerAvatarButton->setIcon(QIcon(""));
    }
    else
        m_playerAvatarButton->setText("");
}

void ClientUI::changePlayerColor(bool choose) {
    QColor playerColor_back = m_playerColor;
    if (choose)
        m_playerColor = QColorDialog::getColor(m_playerColor, this, tr("Character color"));

    if (m_playerColor.isValid())
        setPlayerColor(m_playerColor);
    else
        m_playerColor = playerColor_back;
}

void ClientUI::displayAddServerDialog() {
    m_addServerDialog->setVisible(true);
}

void ClientUI::addServer(QStringList serverProperties) {
    qDebug() << "Adding new server";
    
    m_serverList.append(serverProperties);
    m_serverOnline.append(false);

    retrieveGameInfos(serverProperties, m_serverTableItems->rowCount());
    manageServerListRow(m_serverTableItems->rowCount(), defaultServerData(serverProperties));
}

void ClientUI::removeServer() {
    QModelIndexList selectedList = m_serverTable->selectionModel()->selectedIndexes();

    if (!selectedList.length()) {
        QMessageBox::warning(this, tr("Server removing"), tr("Please select a server to remove."));
        return;
    }
    
    m_serverTableItems->removeRow(selectedList.at(0).row());
    m_serverList.removeAt(selectedList.at(0).row());
    m_serverOnline.removeAt(selectedList.at(0).row());
}

void ClientUI::refreshServerInfos() {
    QModelIndexList selectedList = m_serverTable->selectionModel()->selectedIndexes();

    if (!selectedList.length()) {
        int ret = QMessageBox::question(this, tr("Refresh server information"), tr("No server"
        " selected, click OK to refresh all the server or Cancel to select one"), QMessageBox::Ok | QMessageBox::Cancel);
        
        if (ret == QMessageBox::Cancel)
            return;

        qDebug() << "Updating all server infos";
        for (int i = 0; i < m_serverList.count(); i++) {
            manageServerListRow(i, defaultServerData(m_serverList.at(i)));
            retrieveGameInfos(m_serverList.at(i), i);
        }
    }
    else {
        qDebug() << "Update line" << selectedList.at(0).row();
        manageServerListRow(selectedList.at(0).row(), defaultServerData(m_serverList.at(selectedList.at(0).row())));
        retrieveGameInfos(m_serverList.at(selectedList.at(0).row()), selectedList.at(0).row());
    }
}

void ClientUI::retrieveGameInfos(QStringList server, int identifier) {
    qDebug() << "Retrieving game infos";
    
    CommunicationHandler* clientCom = new CommunicationHandler(server.at(0), 
        server.at(1).toInt(), GAME_INFOS, identifier);

    m_clientsCom << clientCom;

    connect(clientCom, SIGNAL(gameInfos(QStringList)), this,
        SLOT(updateGameInfos(QStringList)));
    connect(clientCom, SIGNAL(comError()), this, SLOT(errorGameInfos()));
}

void ClientUI::updateGameInfos(QStringList gameInfos) {
    qDebug() << "Updating game infos";
    
    CommunicationHandler* clientCom = qobject_cast<CommunicationHandler *>(sender());
    
    if (!clientCom)
        return;
    else if (clientCom->identifier() == -1)
        return;

    m_serverOnline.replace(clientCom->identifier(), true);

    manageServerListRow(clientCom->identifier(), gameInfos);
    m_clientsCom.removeAll(clientCom);
}

void ClientUI::errorGameInfos() {
    qDebug() << "Error recieved";
    
    CommunicationHandler* clientCom = qobject_cast<CommunicationHandler *>(sender());
    
    if (!clientCom)
        return;

    clientCom->disconnect();
    clientCom->closeConnection();
    m_clientsCom.removeAll(clientCom);
}

void ClientUI::manageServerListRow(int row, QStringList data) {
    // Game name
    QStandardItem* nameItem = new QStandardItem(data.at(0));
    nameItem->setEditable(false);
    m_serverTableItems->setItem(row, 0, nameItem);

    // Map name;
    QStandardItem* mapItem = new QStandardItem(data.at(1));
    mapItem->setEditable(false);
    m_serverTableItems->setItem(row, 1, mapItem);
    
    // Player number
    QStandardItem* playerItem = new QStandardItem(tr("%1/%2").arg(data.at(2)).arg(data.at(3)));
    playerItem->setEditable(false);
    m_serverTableItems->setItem(row, 2, playerItem);

    // Bot number
    QStandardItem* botItem = new QStandardItem(data.at(4));
    botItem->setEditable(false);
    m_serverTableItems->setItem(row, 3, botItem);
}

void ClientUI::loadPlayerSettings() {
    qDebug() << "Loading player settings";

    QFile settings(QDir::currentPath() + "/player.qtal");

    if (!settings.exists())
        return;

    settings.open(QIODevice::ReadOnly);
    QDataStream in(&settings);
    in.setVersion(QDataStream::Qt_4_6);

    quint32 magic;
    in >> magic;

    if (magic != PLAYER_SETTINGS_MAGIC)
        return;

    QString playerName;
    in >> playerName;
    m_playerName->setText(playerName);

    in >> m_playerAvatar;
    changePlayerAvatar(false);

    in >> m_playerColor;
    changePlayerColor(false);

    QList<QStringList> serverList;
    in >> serverList;

    foreach (QStringList server, serverList) {
        addServer(server);
    }
}

void ClientUI::dumpPlayerSettings() {
    qDebug() << "Dumping player settings";

    QFile settings(QDir::currentPath() + "/player.qtal");
    
    if (!settings.open(QIODevice::WriteOnly))
        return;

    QDataStream out(&settings);
    out.setVersion(QDataStream::Qt_4_6);

    out << (quint32)PLAYER_SETTINGS_MAGIC;
    out << m_playerName->text();
    out << m_playerAvatar;
    out << m_playerColor;
    out << m_serverList;
}

QStringList ClientUI::defaultServerData(QStringList serverProperties) {
    QString dash = "-";
    QStringList serverData;

    serverData << tr("%1:%2").arg(serverProperties.at(0)).arg(serverProperties.at(1));
    serverData << dash << dash << dash << dash;

    return serverData;
}

void ClientUI::startGame() {
    QModelIndexList selectedServer = m_serverTable->selectionModel()->selectedIndexes();

    if (!selectedServer.length()) {
        QMessageBox::information(this, tr("Game information"),
            tr("Please select a server to play on"));
        return;
    }

    qDebug() << "Starting game";

    QStringList server = m_serverList.at(selectedServer.at(0).row());
    CommunicationHandler* clientCom = new CommunicationHandler();
        
    m_clientsCom << clientCom;
    m_gameCom = m_clientsCom.size()-1;

    // TODO: Resize image before sending
    QByteArray* avatar = new QByteArray;
    avatar->append(QFile(m_playerAvatar).readAll());
    Player* playerInfos  = new Player(m_playerName->text(), m_playerColor, avatar);

    clientCom->setAction(JOIN_GAME);
    clientCom->setHost(server.at(0), server.at(1).toInt());
    clientCom->setPlayer(playerInfos);
    clientCom->beginTransaction();

    m_playButton->setDisabled(true);
    m_playButton->setDown(true);
    m_parent->gameFieldUI()->setClientCom(clientCom);

    QTimer::singleShot(2000, this, SLOT(serverOnline()));
}

void ClientUI::onServerSelected(QModelIndex index) {
    if (m_serverOnline.at(index.row()))
        m_playButton->setEnabled(true);
    else
        m_playButton->setDisabled(true);
}

void ClientUI::serverOnline() {
    if (m_parent->gameFieldUI()->isInGame() == PLAYER_NOT_CONNECTED) {
        QMessageBox::warning(this, tr("Game server offline"), tr("The game server "
        "you try to attempt seems to be offline. Please refresh your server list."));

        QModelIndexList selectedServer = m_serverTable->selectionModel()->selectedIndexes();
        m_serverOnline.replace(selectedServer.at(0).row(), false);
    }

    resetUI();
}


void ClientUI::resetUI() {
    m_playButton->setDown(false);
}