#include "gamefield.h"

Gamefield::Gamefield(QWidget* parent) {
    m_mainLayout = new QGridLayout;
    m_mapScene = new QGraphicsScene;
    m_mapView = new QGraphicsView(m_mapScene, this);
    m_mapView->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    m_mapView->setStyleSheet("background-color: #FFFFFF");

    m_playersInfoLayout = new QVBoxLayout;

    m_mainLayout->addWidget(m_mapView, 0, 0);
    m_mainLayout->setColumnStretch(0, 1);
    m_mainLayout->addLayout(m_playersInfoLayout, 0, 1);
    m_mainLayout->setColumnMinimumWidth(1, 120);

    setLayout(m_mainLayout);
}

/*! Determine the height from a position.
 *
 *  @param pos Position to compute
 *
 *  @return Height corresponding to the position
 *
 *  The height is determined from the top of the game scene.
 *  No validation is done on the accuracy of the position.
 *
 *  @see Gamefiled::widthFromPos
 */
int Gamefield::heightFromPos(int pos) {
    int row = pos / m_map->mapWidth();
    return row * (BLOCK_HEIGHT + BLOCK_SPACING);
}

/*! Determine the width from a position.
 *
 *  @param pos Position to compute
 *
 *  @return Width corresponding to the position
 *
 *  The width is determined from the left of the game scene.
 *  No validation is done on the accuracy of the position.
 *
 *  @see Gamefiled::heightFromPos
 */
int Gamefield::widthFromPos(int pos) {
    int row = pos / m_map->mapWidth();
    int col = pos - (row * m_map->mapWidth());

    return col * (BLOCK_WIDTH + BLOCK_SPACING);
}

/*! Draw the game map.
 *
 *  The map is drawn from the tables computed in the Map instance retrieved from
 *  the XML map file.
 */
void Gamefield::drawMap() {
    // Compute the map information before draw it
    if (m_map->collisionsTab().count() == 0)
        m_map->parseMapString();

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
        }
        else if (m_map->breakableBlocks().at(i)) {
            QGraphicsRectItem* breakableItem = new QGraphicsRectItem(0, 0,
                BLOCK_WIDTH, BLOCK_HEIGHT);
            breakableItem->setBrush(breakableBrush);
            breakableItem->setPen(breakablePen);
            breakableItem->moveBy(widthFromPos(i), heightFromPos(i));

            m_mapScene->addItem(breakableItem);
        }
    }
}

/*! Setter for the player list.
 *
 *  @param players Player list to set.
 *
 *  This method also draw the player on the game scene at there initial position.
 *  The old player are removed from the scene before the new one are set.
 *
 *  @see Gamefield::playerList
 */
void Gamefield::setPlayersList(QMap<int,Player*> players) {
    // Remove the old player before inserting new
    if (m_playersList.count() > 0) {
        QMapIterator<int, Player*> i(m_playersList);
        while (i.hasNext()) {
            i.next();
            m_mapScene->removeItem(i.value());
        }
        
        m_playersList.clear();
    }

    m_playersList.unite(players);

    // Make sure we have the player positions
    if (m_map->playersPositions().count() == 0)
        m_map->parseMapString();

    // Set the position of the players
    QList<int> playersIdx = m_playersList.keys();
    for (int i = 0; i < playersIdx.count(); i++) {
        m_playersList.value(playersIdx.at(i))->setPos(
            widthFromPos(m_map->playersPositions().indexOf(playersIdx.at(i)+1)),
            heightFromPos(m_map->playersPositions().indexOf(playersIdx.at(i)+1))
        );

        m_mapScene->addItem(m_playersList.value(playersIdx.at(i)));
    }
}


/*! Getter for the player list
 *
 *  @return The player list
 *
 *  @see Gamefield::setPlayerList
 */
QMap<int, Player*> Gamefield::playersList() {
    return m_playersList;
}

/*! Setter for the number of rounds of the game.
 *
 *  @param rounds Number of rounds.
 *
 *  There is no limitation on the number of rounds of a game.
 *
 *  @see Gamefield::gameRounds
 */
void Gamefield::setGameRounds(int rounds) {
    m_gameRounds = rounds;
}

/*! Number of rounds of the game.
 *
 *  @return The number of rounds.
 *
 *  @see Gamefield::setGameRounds
 */
int Gamefield::gameRounds() {
    return m_gameRounds;
}

/*! Manage event of the layout.
 *
 *  @param event The recieved event
 *
 *  All the events are forwarded to the event handler.
 */
void Gamefield::keyPressEvent(QKeyEvent* event) {
    m_eventHandler->newEvent(event);
}

/*! Setter for the gamefield map.
 *
 *  @param map Map of the gamefield.
 *
 *  @see Gamefield::gameMap
 */
void Gamefield::setMap(GameMap* map) {
    m_map = map;
}

/*! Map of the gamefield.
 *
 *  @return Gamefield map.
 *
 *  @see Gamefield::setMap
 */
GameMap* Gamefield::gameMap() {
    return m_map;
}

/*! Set the gamefield event handler.
 *
 *  @param eventHandler Gamefield event handler.
 *
 *  @see Gamefield::eventHandler
 */
void Gamefield::setEventHandler(EventHandler* eventHandler) {
    m_eventHandler = eventHandler;

    QObject::connect(m_eventHandler, SIGNAL(playerMoved(int, QString)), this,
        SLOT(onPlayerMove(int, QString)));
    QObject::connect(m_eventHandler, SIGNAL(bombDropped(int)), this,
        SLOT(onBombDrop(int)));
    QObject::connect(this, SIGNAL(movePlayer(int, int)), m_eventHandler,
        SLOT(onPlayerMove(int, int)));
}

/*! Try to move a player to a requested position
 *
 *  @param player Player Id to move
 *  @param move Move action to do
 */
void Gamefield::onPlayerMove(int player, QString move) {
    int playerPosition = m_map->playersPositions().indexOf(player+1);

    if (move == "left") {
        if (!m_map->collisionsTab().at(playerPosition-1)) {
            playerPosition--;
        }
        else
            return;
    }
    else if (move == "right") {
        if (!m_map->collisionsTab().at(playerPosition+1)) {
            playerPosition++;
        }
        else
            return;
    }
    else if (move == "up") {
        int newPos = playerPosition-m_map->mapWidth();
        if (newPos  >= 0 && !m_map->collisionsTab().at(newPos)) {
            playerPosition = newPos;
        }
        else
            return;
    }
    else if (move == "down") {
        int newPos = playerPosition+m_map->mapWidth();
        if (newPos < m_map->collisionsTab().count()
            && !m_map->collisionsTab().at(newPos)) {
            playerPosition = newPos;
        }
        else
            return;
    }
    else
        return;

    // Set the new position on the map
    m_map->setPlayerPosition(player+1, playerPosition);
    
    // Move the player on the gamefield
    int playerIdxList = m_playersList.keys().indexOf(player);
    m_playersList.value(playerIdxList)->setPos(widthFromPos(playerPosition),
        heightFromPos(playerPosition));

    // Send infos that player has moved
    emit movePlayer(player, playerPosition);
}

/*!
 *
 *  @param player Player who drop the bomb
 */
void Gamefield::onBombDrop(int player) {
    // TODO: drop bomb
}

/*! Event handler of the gamefield.
 *
 *  @return The gamefield event handler.
 *
 *  The event handler manage and parse the event of the gamefield.
 *
 *  @see Gamefield::setEventHandler
 *  @see EventHandler
 */
EventHandler* Gamefield::eventHandler() {
    return m_eventHandler;
}

/*! Title of the layout
 *
 *  @return The layout title
 */
QString Gamefield::layoutTitle() {
    return tr("Bomberman - Ongoing game");
}