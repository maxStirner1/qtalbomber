/*
 *  QtalBomber - A Qt BomberMan game <http://www.bombermanproject.com/>
 *  Copyright (C) 2010  QtalBomber team <team@bombermanproject.com>
 *
 *  Authors: Quentin Amoudruz
 *           Thomas Savineau
 *           Alexandre Joseph
 *           Lucas Courot
 *
 *  File:        GameFieldUI.cpp
 *  Description: Manage gamefield display.
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

#include "ui/GameFieldUI.h"
#include "ui/GameUI.h"
#include "ui/ClientUI.h"

GameFieldUI::GameFieldUI(GameUI* parent) : QWidget(parent), m_parent(parent) {
    m_map = new Map;
    m_mainPanel = new QGridLayout;
    m_mapScene = new QGraphicsScene;
    m_mapText = new QGraphicsSimpleTextItem;

    m_mapView = new QGraphicsView(m_mapScene, this);
    m_mapView->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    m_mapView->setStyleSheet("background-color: #FFFFFF");

    m_mapRight = new QVBoxLayout;

    m_mainPanel->addWidget(m_mapView, 0, 0);
    m_mainPanel->setColumnStretch(0, 1);
    m_mainPanel->addLayout(m_mapRight, 0, 1);
    m_mainPanel->setColumnMinimumWidth(1, 120);

    setLayout(m_mainPanel);

    m_inGame = PLAYER_NOT_CONNECTED;
    m_gameStarted = false;

    m_startText.append("60");
    m_startText.append(tr("Ready?"));

    m_startText.append("80");
    m_startText.append("3");

    m_startText.append("80");
    m_startText.append("2");

    m_startText.append("80");
    m_startText.append("1");

    m_startText.append("70");
    m_startText.append(tr("Go!"));
}

QString GameFieldUI::layoutTitle() {
    return tr("Bomberman Game");
}

void GameFieldUI::setClientCom(CommunicationHandler* clientCom) {
    qDebug() << "Client communication in gamefield set";
    
    m_clientCom = clientCom;

    connect(m_clientCom, SIGNAL(gameJoined(int)), this, SLOT(onGameJoined(int)));
    connect(m_clientCom, SIGNAL(playerRefused()), this, SLOT(onPlayerRefused()));
    connect(m_clientCom, SIGNAL(newGameMap(int, int, QList<int>, QList<int>,
        QList<int>, QList<int>, QList<int>)), this, SLOT(setMapInfo(int, int,
        QList<int>, QList<int>, QList<int>, QList<int>, QList<int>)));
    connect(m_clientCom, SIGNAL(playerDisconnected()), this, SLOT(onPlayerDisconnected()));
    connect(m_clientCom, SIGNAL(initPlayers(int)), this, SLOT(onInitPlayers(int)));
    connect(m_clientCom, SIGNAL(playerUpdated(Player*)), this, SLOT(onPlayerUpdated(Player*)));
    connect(m_clientCom, SIGNAL(gameStart()), this, SLOT(onGameStart()));
    connect(m_clientCom, SIGNAL(rivalMoved(int, int)), this, SLOT(moveRival(int, int)));
}

void GameFieldUI::onGameJoined(int position) {
    m_playerPosition = position;
    m_inGame = PLAYER_IN_GAME;
    m_clientCom->setAction(GAME_MAP);
    m_clientCom->runAction();
    m_parent->displayGameFieldUI();

    connect(m_clientCom, SIGNAL(socketFree()), this, SLOT(requestPlayerList()));

    m_clientCom->player()->setPlayerType(PLAYER_IS_SELF);
    m_clientCom->player()->setPlayerPosition(m_playerPosition);
}

int GameFieldUI::isInGame() {
    return m_inGame;
}

void GameFieldUI::onPlayerRefused() {
    m_inGame = PLAYER_REFUSED;

    QMessageBox::warning(0, tr("Game server information"), tr("Cannot join the "
    "server, it seems the game has already started. Please try another server."));

    m_parent->clientUI()->resetUI();
}

void GameFieldUI::onPlayerDisconnected() {
    if (m_inGame == PLAYER_IN_GAME) {
        m_mapView->setDisabled(true);
        m_mapView->setBackgroundBrush(QBrush(Qt::lightGray, Qt::Dense2Pattern));
        QMessageBox::critical(0, tr("Communication error"), tr("Connection with "
        "server has been lost. Click ok to return main menu"));

        m_inGame = PLAYER_NOT_CONNECTED;
        m_parent->displayStartUI();
    }
}

void GameFieldUI::onPlayerUpdated(Player* player) {
    qDebug() << "Recieved update for player" << player->playerPseudo();

    if (player->playerPosition() > m_playerList.count()) {
        qDebug() << "Player position may not have been set...";
        return;
    }

    Player* currentPlayer = m_playerList.at(player->playerPosition());
    currentPlayer->setPlayerPseudo(player->playerPseudo());
    currentPlayer->setPlayerColor(player->playerColor());
    currentPlayer->setPlayerType(player->playerType());
    updatePlayerFace(currentPlayer);
}

void GameFieldUI::setMapInfo(int width, int height, QList<int> collisionsTab, 
    QList<int> unbreakableBlocks, QList<int> breakableBlocks, 
    QList<int> bonusPositions, QList<int> playersPositions) {

    qDebug() << "Setting map new infos";

    m_playerLocation = playersPositions.indexOf(m_playerPosition+1);

    m_map->setMapWidth(width);
    m_map->setMapHeight(height);    
    m_map->setCollisionsTab(collisionsTab);
    m_map->setUnbreakablesBlocks(unbreakableBlocks);
    m_map->setBreakablesBlocks(breakableBlocks);
    m_map->setBonusPositions(bonusPositions);
    m_map->setPlayersPositions(playersPositions);

    drawMap();
    writeMapText("Waiting game to start...");

    for (int i = 0; i < m_playerList.count(); i++) {
        m_playerList.at(i)->setPos(widthFromPos(m_map->playersPositions().indexOf(i+1)),
            heightFromPos(m_map->playersPositions().indexOf(i+1)));
        m_mapScene->addItem(m_playerList.at(i));
    }

    // Set player now
    onPlayerUpdated(m_clientCom->player());
}

void GameFieldUI::requestPlayerList() {
    disconnect(m_clientCom, SIGNAL(socketFree()), this, SLOT(requestPlayerList()));
    m_clientCom->setAction(GAME_PLAYERS);
    m_clientCom->runAction();
}

void GameFieldUI::onInitPlayers(int playerNb) {
    qDebug() << "Initializing players";
    for (int i = 0; i < playerNb; i++) {
        Player* player = new Player;
        player->setPos(0, 0);
        player->setPlayerPosition(i);
        player->setPlayerType(PLAYER_IS_NONE);
        m_playerList.append(player);

        QLabel* playerFace = new QLabel;
        playerFace->setFixedSize(120, 120);

        m_playerDisplay.append(playerFace);
        m_mapRight->addWidget(playerFace);
        m_mapScene->addItem(player);
    }
}

void GameFieldUI::updatePlayerFace(Player* player) {
    qDebug() << "Updating player" << player->playerPosition();

    QPixmap playerPix;
    qDebug() << "Avatar size:" << player->playerAvatar().size();
    if (player->playerAvatar().size() != 0)
        playerPix.loadFromData(player->playerAvatar());
    else
        playerPix.load(":/qtal-unknow");
    playerPix.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QString playerToolTip = (player->playerType() == PLAYER_IS_REAL) ? tr("Player ")
        : (player->playerType() == PLAYER_IS_BOT) ? tr("Bot ")
        : (player->playerType() == PLAYER_IS_SELF) ? tr("You, ") : "";
    playerToolTip += player->playerPseudo();

    QLabel* currentPlayerFace = m_playerDisplay.at(player->playerPosition());
    currentPlayerFace->setPixmap(playerPix);
    currentPlayerFace->setToolTip(playerToolTip);

    QString style = "QLabel { border: 3px solid rgb(%1, %2, %3); }";
    currentPlayerFace->setStyleSheet(style.arg(player->playerColor().red())
        .arg(player->playerColor().green()).arg(player->playerColor().blue()));
}

void GameFieldUI::keyPressEvent(QKeyEvent* event) {
    if (!m_gameStarted)
        return;
    
    switch (event->key()) {
        case Qt::Key_Q:
            movePlayer(MOVE_LEFT);
            break;
        case Qt::Key_S:
            movePlayer(MOVE_DOWN);
            break;
        case Qt::Key_D:
            movePlayer(MOVE_RIGHT);
            break;
        case Qt::Key_Z:
            movePlayer(MOVE_UP);
            break;
        case Qt::Key_E:
//            dropBomb();
            break;
    }
}

void GameFieldUI::movePlayer(int position) {
    if (position == MOVE_LEFT) {
        if (!m_map->collisionsTab().at(m_playerLocation-1)) {
            m_playerLocation--;
            m_clientCom->sendEvent(position);
        }
    }
    else if (position == MOVE_RIGHT) {
        if (!m_map->collisionsTab().at(m_playerLocation+1)) {
            m_playerLocation++;
            m_clientCom->sendEvent(position);
        }
    }
    else if (position == MOVE_UP) {
        int newPos = m_playerLocation-m_map->width();
        if (newPos  >= 0 && !m_map->collisionsTab().at(newPos)) {
            m_playerLocation = newPos;
            m_clientCom->sendEvent(position);
        }
    }
    else if (position == MOVE_DOWN) {
        int newPos = m_playerLocation+m_map->width();
        if (newPos < m_map->collisionsTab().count()
            && !m_map->collisionsTab().at(newPos)) {
            m_playerLocation = newPos;
            m_clientCom->sendEvent(position);
        }
    }
    else {
        return;
    }

    m_map->setPlayerPosition(m_playerPosition, m_playerLocation);
    m_playerList.at(m_playerPosition)->setPos(widthFromPos(m_playerLocation),
        heightFromPos(m_playerLocation));
}

QList<QGraphicsRectItem*> GameFieldUI::mapItems() {
    return m_mapItems;
}


void GameFieldUI::onGameStart() {
    qDebug() << "Game is starting...";
    m_timer.start(1000);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(displayCountdown()));
}

void GameFieldUI::displayCountdown() {
    if (m_startText.isEmpty()) {
        qDebug() << "Countdown finished";
        m_timer.stop();
        removeMapText();
        m_gameStarted = true;
        return;
    }

    writeMapText(m_startText.takeFirst(), m_startText.takeFirst().toInt(), false, true);
}

void GameFieldUI::moveRival(int playerIdx, int playerPosition) {
    m_map->setPlayerPosition(playerIdx, playerPosition);

    m_playerList.at(playerIdx)->setPos(widthFromPos(playerPosition),
        heightFromPos(playerPosition));

    if (playerIdx == m_playerPosition)
        m_playerLocation = playerPosition;
}

/*
 * Map drawing functions
 */

int GameFieldUI::heightFromPos(int pos) {
    int row = pos / m_map->width();
    return row * (BLOCK_HEIGHT + BLOCK_SPACING);
}

int GameFieldUI::widthFromPos(int pos) {
    int row = pos / m_map->width();
    int col = pos - (row * m_map->width());

    return col * (BLOCK_WIDTH + BLOCK_SPACING);
}

void GameFieldUI::writeMapText(QString text, int size, bool add, bool center) {
    m_mapText->setText(text);

    QBrush brush(Qt::white);
    QPen pen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QFont font;
    font.setBold(QFont::Black);
    font.setStretch(QFont::Condensed);
    font.setCapitalization(QFont::SmallCaps);
    font.setPointSize(size);

    if (center) {
        m_mapText->setPos(heightFromPos(m_map->heigh()/2), widthFromPos(m_map->width()/1.8));
    }
    else {
        m_mapText->setPos(heightFromPos(m_map->heigh()/2), widthFromPos(m_map->width()/2.5));
    }

    m_mapText->setBrush(brush);
    m_mapText->setPen(pen);
    m_mapText->setFont(font);

    if (add)
        m_mapScene->addItem(m_mapText);
}

void GameFieldUI::removeMapText() {
    m_mapScene->removeItem(m_mapText);
}

void GameFieldUI::drawMap() {
    QBrush unbreakableBrush(Qt::darkGray);
    QPen unbreakablePen(Qt::black);
    QBrush breakableBrush(Qt::gray, Qt::Dense4Pattern);
    QPen breakablePen(Qt::gray);

    for (int i = 0; i < m_map->collisionsTab().count(); i++) {
        if (m_map->unbreakableBlocks().at(i)) {
            QGraphicsRectItem* unbreakableItem = new QGraphicsRectItem(0, 0,
                BLOCK_WIDTH, BLOCK_HEIGHT);
            unbreakableItem->setBrush(unbreakableBrush);
            unbreakableItem->setPen(unbreakablePen);
            unbreakableItem->moveBy(widthFromPos(i), heightFromPos(i));

            m_mapScene->addItem(unbreakableItem);
            m_mapItems.append(unbreakableItem);
        }
        else if (m_map->breakableBlocks().at(i)) {
            QGraphicsRectItem* breakableItem = new QGraphicsRectItem(0, 0,
                BLOCK_WIDTH, BLOCK_HEIGHT);
            breakableItem->setBrush(breakableBrush);
            breakableItem->setPen(breakablePen);
            breakableItem->moveBy(widthFromPos(i), heightFromPos(i));

            m_mapScene->addItem(breakableItem);
            m_mapItems.append(breakableItem);
        }
    }
}


