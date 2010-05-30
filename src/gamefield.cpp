#include "gamefield.h"
#include <QtCore/QDebug>

Gamefield::Gamefield() {
    setWindowTitle(tr("QtalBomber - Bomberman game"));
    setWindowIcon(QIcon(":/qtal-icon"));
    setFixedSize(640, 515);
    // Set the window as a main window to avoid problems on Windows and Mac.
    setWindowFlags(Qt::Window);

    // Initialize the graphics view
    m_mapView = new QGraphicsView;
    m_mapView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    m_mapView->setStyleSheet("background-color: #FFFFFF");

    // Initialize the game informations panel
    m_gameInfoPanel = new QVBoxLayout;

    // Initialize players info
    for (int i = 0; i < PLAYER_NUMBER_MAX; i++) {
        QPushButton* playerInfos = new QPushButton(tr("Player %1").arg(i + 1));
        m_playersInfosDisplay.insert(i, playerInfos);
        m_gameInfoPanel->addWidget(playerInfos);
        connect(playerInfos, SIGNAL(clicked()), this, SLOT(onPlayerInfosClicked()));
    }

    m_mapList = new QComboBox;
    m_mapList->setMaximumWidth(120);
    m_gameInfoPanel->addWidget(m_mapList);
    connect(m_mapList, SIGNAL(activated(int)), this, SLOT(selectMap(int)));

    m_gameRoundsChoice = new QSpinBox;
    m_gameRoundsChoice->setMinimum(1);
    m_gameRoundsChoice->setMaximum(10);
    m_gameRoundsChoice->setValue(3);
    m_gameInfoPanel->addWidget(m_gameRoundsChoice);


    QPushButton* startGameButton = new QPushButton(tr("Start playing"));
    m_gameInfoPanel->addWidget(startGameButton);
    connect(startGameButton, SIGNAL(clicked()), this, SLOT(toggleGameState()));

    QPushButton* aboutGameButton = new QPushButton(tr("About"));
    m_gameInfoPanel->addWidget(aboutGameButton);
    // TODO: Create about dialog and connect it to the button

    QPushButton* exitGameButton = new QPushButton(tr("Exit game"));
    m_gameInfoPanel->addWidget(exitGameButton);
    connect(exitGameButton, SIGNAL(clicked()), this, SLOT(exitGame()));

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(m_mapView, 0, 0);
    mainLayout->addLayout(m_gameInfoPanel, 0, 1);
    setLayout(mainLayout);

    m_gameState = GAME_PENDING;
    m_currentRound = 1;
    initPlayersControlKeys();

    loadMaps();
    if (m_availableMaps.length() == 0) {
        QMessageBox::critical(this, tr("Game maps error"),
            tr("No valid maps or no <em>%1</em> directory found").arg(MAPS_DIRECTORY));
        startGameButton->setDisabled(true);
    }
    else {
        // Load the first map
        selectMap(0);
    }
}




/******************************************************************************/
/******************************** Game management *****************************/
/******************************************************************************/


/*! Recieve the exit game signal and handle it.
 *
 *  If the game is running, a warning message is displayed.
 */
void Gamefield::exitGame() {
    if (m_gameState == GAME_RUNNING) {
        int ret = QMessageBox::warning(this, tr("Exit game confirmation"),
            tr("Do you really want to exit the game?"),
            QMessageBox::Ok | QMessageBox::Cancel);

        if (ret != QMessageBox::Ok)
            return;
    }

    QApplication::quit();
}

/*! Toggle game state.
 *
 *  This methods also performs some action as desactivate some elements of the
 *  interface.
 */
void Gamefield::toggleGameState() {
    QPushButton* startGameButton = qobject_cast<QPushButton *>(sender());

    if (!startGameButton)
        return;

    if (m_gameState == GAME_PENDING) {
        startGameButton->setText("Stop playing");
        m_mapList->setDisabled(true);
        m_gameRoundsChoice->setDisabled(true);
        displayeCurrentRound(m_currentRound);
    }
    else {
        // Don't display a warning if game is finished
        if (m_gameState == GAME_RUNNING) {
            int ret = QMessageBox::warning(this, tr("Stop game confirmation"),
                tr("Do you really want to stop the game?"),
                QMessageBox::Ok | QMessageBox::Cancel);

            if (ret == QMessageBox::Cancel)
                return;
        }
        m_gameState = GAME_PENDING;
        startGameButton->setText("Start playing");
        m_mapList->setEnabled(true);
        m_gameRoundsChoice->setEnabled(true);

        // Re-init game
        // TODO: debug game relaunch
        selectMap(m_availableMaps.indexOf(m_currentMap));
    }
}

/*! Set the game map and draw the graphics are
 *
 *  @param map Map index to be set.
 */
void Gamefield::selectMap(int index) {
    if (m_gameState != GAME_PENDING)
        return;

    m_currentMap = m_availableMaps.at(index);

    // Release the old scene before create a new on
    drawMap();
    m_gamefieldText = new QGraphicsSimpleTextItem;
    m_mapScene->addItem(m_gamefieldText);
    initPlayers();
}

/*! Manage event of the gamefield.
 *
 *  @param event Emitted event.
 */
void Gamefield::keyPressEvent(QKeyEvent* event) {
    if (m_gameState != GAME_RUNNING)
        return;

    if (!m_playersControlKeys.contains(event->key()))
        return;

    int player = m_playersControlKeys.value(event->key()).keys().at(0);
    int action = m_playersControlKeys.value(event->key()).values().at(0);

    if (m_players.at(player)->state() != PLAYER_ALIVE)
        return;

    if (action == DROP_BOMB)
        dropBomb(player);
    else
        movePlayer(player, action);
}






/******************************************************************************/
/******************************** Map management ******************************/
/******************************************************************************/



/*! Load and set map in the map list.
 *
 *   For each map XML file, an instance of the Map class is created.
 */
void Gamefield::loadMaps() {
    QDir mapsDir;

    if (!mapsDir.cd(QObject::tr("%1/%2").arg(QApplication::applicationDirPath()).arg(MAPS_DIRECTORY)))
        return;

    // Maps must have an .xml extension
    QStringList mapsExt("*.xml");
    mapsDir.setFilter(QDir::Files);
    mapsDir.setNameFilters(mapsExt);

    QFileInfoList mapsList = mapsDir.entryInfoList();

    foreach(QFileInfo mapInfo, mapsList) {
        if (!mapInfo.isReadable())
            continue;

        // Discard the map if not valid
        GameMap* gameMap = new GameMap(mapInfo);
        if (!gameMap->isValid())
            continue;

        m_availableMaps.append(gameMap);
        m_mapList->addItem(gameMap->name());
    }
}


/*! Draw the game map.
 *
 *  The map is drawn from the tables computed in the Map instance retrieved from
 *  the XML map file.
 */
void Gamefield::drawMap() {
    // Compute the map information before draw it
    if (m_currentMap->collisionsTab().count() == 0)
        m_currentMap->parseMapRepr();

    // Delete all the existing item to avoid conflict
    m_gamefieldBomb.clear();
    m_gamefieldFlam.clear();
    m_breakableBlocks.clear();

    if (m_mapView->scene() != 0)
        m_mapScene->~QGraphicsScene();

    // TODO: fix map reset

    m_mapScene = new QGraphicsScene;
    m_mapView->setScene(m_mapScene);

    QBrush unbreakableBrush(Qt::darkGray);
    QPen unbreakablePen(Qt::black);
    QBrush breakableBrush(Qt::gray, Qt::Dense4Pattern);
    QPen breakablePen(Qt::gray);

    for (int i = 0; i < m_currentMap->collisionsTab().count(); i++) {
        if (m_currentMap->unbreakableBlocks().at(i)) {
            QGraphicsRectItem* unbreakableItem = new QGraphicsRectItem(0, 0,
                MAP_BLOCK_WIDTH, MAP_BLOCK_HEIGHT);
            unbreakableItem->setBrush(unbreakableBrush);
            unbreakableItem->setPen(unbreakablePen);
            unbreakableItem->moveBy(widthFromPos(i), heightFromPos(i));

            m_mapScene->addItem(unbreakableItem);
        }
        else if (m_currentMap->breakableBlocks().at(i)) {
            QGraphicsRectItem* breakableItem = new QGraphicsRectItem(0, 0,
                MAP_BLOCK_WIDTH, MAP_BLOCK_HEIGHT);
            breakableItem->setBrush(breakableBrush);
            breakableItem->setPen(breakablePen);
            breakableItem->moveBy(widthFromPos(i), heightFromPos(i));

            m_mapScene->addItem(breakableItem);
            m_breakableBlocks.insert(i, breakableItem);
        }
    }
}

/*! Determine the height from a position.
 *
 *  The height is determined from the top of the game scene.
 *  No validation is done on the accuracy of the position.
 *
 *  @param pos Position to compute
 *
 *  @return Height corresponding to the position
 *
 *  @see Gamefiled::widthFromPos
 */
int Gamefield::heightFromPos(int pos) {
    int row = pos / m_currentMap->width();
    return row * (MAP_BLOCK_HEIGHT + MAP_BLOCK_SPACING);
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
    int row = pos / m_currentMap->width();
    int col = pos - (row * m_currentMap->width());

    return col * (MAP_BLOCK_WIDTH + MAP_BLOCK_SPACING);
}




/******************************************************************************/
/*************************** Player management ********************************/
/******************************************************************************/


/*! Game players initialization.
 *
 *  Initialize players and set it on the gamefield.
 */
void Gamefield::initPlayers() {
    m_players.clear();
    m_playerInGame = 0;

    for (int i = 0; i < PLAYER_NUMBER_MAX; i++) {
        Player* player = new Player(i);

        m_players.insert(i, player);

        if (i < PLAYER_NUMBER_MIN)
            setPlayerAlive(i);
        else
            setPlayerOff(i);

        // Set player score at zero
        m_playersScore.insert(i, 0);

        setPlayerDefaultPos(i);
    }
}

/*! Set the default key for the players
 *
 * The list generated by this method is used by the event handler.
 * The keys and information are organized as follow:
 * (key, (playerId, action))
 *
 * The 'key' is the event that will be catched.
 * The 'playerId' is the Id of the player starting from 0.
 * And the action is one of 'up', 'down', 'left', 'right
 *
 * Remember to change this if the maximum player number is changed
 */
void Gamefield::initPlayersControlKeys() {
    QList<int> keysUp;
    QList<int> keysDown;
    QList<int> keysLeft;
    QList<int> keysRight;
    QList<int> keysDrop;

    // Define the default keys
    keysUp << Qt::Key_Up << Qt::Key_Z << Qt::Key_I << Qt::Key_G;
    keysDown << Qt::Key_Down << Qt::Key_S << Qt::Key_K << Qt::Key_B;
    keysLeft << Qt::Key_Left << Qt::Key_Q << Qt::Key_J << Qt::Key_V;
    keysRight << Qt::Key_Right << Qt::Key_D << Qt::Key_L << Qt::Key_N;
    keysDrop << Qt::Key_Space << Qt::Key_E << Qt::Key_O << Qt::Key_H;

    QMap<int, QList<int> > keysActions;
    keysActions.insert(MOVE_UP, keysUp);
    keysActions.insert(MOVE_DOWN, keysDown);
    keysActions.insert(MOVE_LEFT, keysLeft);
    keysActions.insert(MOVE_RIGHT, keysRight);
    keysActions.insert(DROP_BOMB, keysDrop);

    QMapIterator<int, QList<int> > i(keysActions);
    while (i.hasNext()) {
        i.next();
        for (int j = 0; j < i.value().length(); j++) {
            QMap<int, int> keyAssoc;
            keyAssoc.insert(j, i.key());
            m_playersControlKeys.insert(i.value().at(j), keyAssoc);
        }
    }
}

void Gamefield::onPlayerInfosClicked() {
    if (m_gameState != GAME_PENDING)
        return;

    // Label which send the signal
    QPushButton* playerInfosLabel = qobject_cast<QPushButton *>(sender());

    if (!playerInfosLabel)
        return;

    // Retrieve the player id
    int playerId = m_playersInfosDisplay.indexOf(playerInfosLabel);

    if (m_players.at(playerId)->state() == PLAYER_OFF)
        setPlayerAlive(playerId);
    else {
        if (m_playerInGame > PLAYER_NUMBER_MIN)
            setPlayerOff(playerId);
    }
}

/*! Set a player as dead.
 *
 *  @param playerId Player identifier
 *
 *  @see Gamefield::playerAlive
 *  @see Gamefield::playerNone
 */
void Gamefield::setPlayerDead(int playerId) {
    m_players.at(playerId)->setState(PLAYER_DEAD);
    m_mapScene->removeItem(m_players.at(playerId));
    m_playersInfosDisplay.at(playerId)->setStyleSheet("border: 3px solid red");
    m_currentMap->removePlayer(playerId + 1);
    m_playerInGame--;

    // Update player score
    m_playersScore[playerId] = m_playersScore[playerId] + m_playerInGame;

    manageRounds();
}

/*! Set the players at here default position on the map.
 *
 *  @see Gamefield::initPlayers
 */
void Gamefield::setPlayerDefaultPos(int playerId) {
    int playerTabPos = m_currentMap->playersPositions().indexOf(playerId+1);
    m_players.at(playerId)->setPos(widthFromPos(playerTabPos), heightFromPos(playerTabPos));
}

/*! Set player as alive.
 *
 *  @param playerId Player identifier
 *
 *  @see Gamefield::playerDead
 *  @see Gamefield::playerNone
 */
void Gamefield::setPlayerAlive(int playerId) {
    m_players.at(playerId)->setState(PLAYER_ALIVE);
    m_mapScene->addItem(m_players.at(playerId));
    m_playersInfosDisplay.at(playerId)->setStyleSheet("border: 3px solid blue");
    m_currentMap->addCollision(m_currentMap->playersPositions().indexOf(playerId + 1));
    m_playerInGame++;
}

/*! Set a player as not used
 *
 *  @param playerId Player identifier
 *
 *  @see Gamefield::playerAlive
 *  @see Gamefield::playerDead
 */
void Gamefield::setPlayerOff(int playerId) {
    if (m_players.at(playerId)->state() == PLAYER_ALIVE) {
        m_currentMap->removeCollision(m_currentMap->playersPositions().indexOf(playerId + 1));
        m_mapScene->removeItem(m_players.at(playerId));
    }

    if (m_players.at(playerId)->state() != PLAYER_OFF)
        m_playerInGame--;
        
    m_players.at(playerId)->setState(PLAYER_OFF);
    m_playersInfosDisplay.at(playerId)->setStyleSheet("border: 3px solid gray");
}

/*! Try to move a player to a requested position
 *
 * @param player Player Id to move
 * @param move Move action to do
 */
void Gamefield::movePlayer(int playerId, int direction) {
    int playerPosition = m_currentMap->playersPositions().indexOf(playerId + 1);

    if (direction == MOVE_LEFT) {
        if (m_currentMap->collisionsTab().at(playerPosition - 1) == 0) {
            playerPosition--;
        }
        else
            return;
    }
    else if (direction == MOVE_RIGHT) {
        if (m_currentMap->collisionsTab().at(playerPosition + 1) == 0) {
            playerPosition++;
        }
        else
            return;
    }
    else if (direction == MOVE_UP) {
        int newPos = playerPosition - m_currentMap->width();
        if (newPos >= 0 && m_currentMap->collisionsTab().at(newPos) == 0) {
            playerPosition = newPos;
        }
        else
            return;
    }
    else if (direction == MOVE_DOWN) {
        int newPos = playerPosition + m_currentMap->width();
        if (newPos < m_currentMap->collisionsTab().count()
            && m_currentMap->collisionsTab().at(newPos) == 0) {
            playerPosition = newPos;
        }
        else
            return;
    }
    else
        return;

    // Set the new position on the map
    m_currentMap->setPlayerPosition(playerId + 1, playerPosition);

    // Move the player on the gamefield
    m_players.at(playerId)->setPos(widthFromPos(playerPosition),
        heightFromPos(playerPosition));
}



/******************************************************************************/
/*************************** Bomb and flam management *************************/
/******************************************************************************/


/*! Drop a bomb on the gamefield
 *
 *  The bomb is dropped at the player position.
 *
 *  @param playerId Identifier of the player who drop the bomb
 *
 *  @see Gamefield::onBombExplode
 */
void Gamefield::dropBomb(int playerId) {
    if (m_players.at(playerId)->bombCapacity() < 1)
        return;

    int bombTabPosition = m_currentMap->playersPositions().indexOf(playerId + 1);
    Bomb* newBomb = new Bomb(playerId, bombTabPosition);
    newBomb->setPos(m_players.at(playerId)->scenePos());

    // Add the bomb to the list and add a possible collision
    m_gamefieldBomb.insert(bombTabPosition, newBomb);
    m_currentMap->addCollision(bombTabPosition);

    // Decrement player bomb capacity and add the bomb on the gamefield
    m_players.at(playerId)->setBombCapacity(m_players.at(playerId)->bombCapacity() - 1);
    m_mapScene->addItem(newBomb);

    connect(newBomb, SIGNAL(blowUp(int)), this, SLOT(onBombExplode(int)));
    newBomb->drop();
}

/*! Realize action when a bomb explode
 *
 *  @see Gamefield::dropBomb
 */
void Gamefield::onBombExplode(int position) {
    if (!m_gamefieldBomb.contains(position))
        return;

    Bomb* explodedBomb = m_gamefieldBomb[position];
    int playerId = explodedBomb->playerId();
    int bombPos = m_gamefieldBomb.key(explodedBomb);
    int flamLength = m_players.at(playerId)->bombsRange();

    // Refund bomb capacity to player
    m_players.at(playerId)->setBombCapacity(m_players.at(playerId)->bombCapacity() + 1);

    // Create the flam on the scene
    Flam* flamUp = new Flam(flamLength, MOVE_UP, bombPos, explodedBomb->scenePos());
    m_gamefieldFlam[bombPos] = flamUp;
    m_mapScene->addItem(flamUp);
    connect(flamUp, SIGNAL(flamGrow()), this, SLOT(stretchFlam()));

    Flam* flamDown = new Flam(flamLength, MOVE_DOWN, bombPos, explodedBomb->scenePos());
    m_gamefieldFlam[bombPos] = flamDown;
    m_mapScene->addItem(flamDown);
    connect(flamDown, SIGNAL(flamGrow()), this, SLOT(stretchFlam()));

    Flam* flamRight = new Flam(flamLength, MOVE_RIGHT, bombPos, explodedBomb->scenePos());
    m_gamefieldFlam[bombPos] = flamRight;
    m_mapScene->addItem(flamRight);
    connect(flamRight, SIGNAL(flamGrow()), this, SLOT(stretchFlam()));

    Flam* flamLeft = new Flam(flamLength, MOVE_LEFT, bombPos, explodedBomb->scenePos());
    m_gamefieldFlam[bombPos] = flamLeft;
    m_mapScene->addItem(flamLeft);
    connect(flamLeft, SIGNAL(flamGrow()), this, SLOT(stretchFlam()));

    // Remove the possible collision and the bomb from the scene
    m_currentMap->removeCollision(bombPos);
    m_mapScene->removeItem(explodedBomb);
    m_gamefieldBomb.remove(bombPos);
    explodedBomb->~QGraphicsEllipseItem();
}

void Gamefield::stretchFlam() {
    Flam* flam = qobject_cast<Flam *>(sender());

    if (!flam)
        return;

    // Remove the flam if grow finished
    if (flam->currentRange() > flam->range()) {
        m_mapScene->removeItem(flam);
        flam->~QGraphicsEllipseItem();
        return;
    }

    int newPos = 0;
    bool collides = false;

    if (flam->direction() == MOVE_LEFT) {
        newPos = flam->position() - 1;
        if (m_currentMap->collisionsTab().at(newPos) == 0)
            //            flam->update(0, 0, flam->rect().x() - BLOCK_WIDTH - BLOCK_SPACING, flam->rect().y());
            flam->moveBy(-MAP_BLOCK_WIDTH - MAP_BLOCK_SPACING, 0);
        else
            collides = true;
    }
    else if (flam->direction() == MOVE_RIGHT) {
        newPos = flam->position() + 1;
        if (m_currentMap->collisionsTab().at(newPos) == 0)
            //            flam->update(0, 0, flam->rect().x() + BLOCK_WIDTH + BLOCK_SPACING, flam->rect().y());
            flam->moveBy(MAP_BLOCK_WIDTH + MAP_BLOCK_SPACING, 0);
        else
            collides = true;
    }
    else if (flam->direction() == MOVE_UP) {
        newPos = flam->position() - m_currentMap->width();
        if (newPos < m_currentMap->collisionsTab().count() && m_currentMap->collisionsTab().at(newPos) == 0)
            //            flam->update(0, 0, flam->rect().x(), flam->rect().y() - BLOCK_WIDTH - BLOCK_SPACING);
            flam->moveBy(0, -MAP_BLOCK_WIDTH - MAP_BLOCK_SPACING);
        else
            collides = true;
    }
    else if (flam->direction() == MOVE_DOWN) {
        newPos = flam->position() + m_currentMap->width();
        if (newPos < m_currentMap->collisionsTab().count() && m_currentMap->collisionsTab().at(newPos) == 0)
            //            flam->update(0, 0, flam->rect().x(), flam->rect().y() + BLOCK_WIDTH + BLOCK_SPACING);
            flam->moveBy(0, MAP_BLOCK_WIDTH + MAP_BLOCK_SPACING);
        else
            collides = true;
    }
    else
        return;

    if (collides) {
        if (m_currentMap->playersPositions().at(newPos))
            setPlayerDead(m_currentMap->playersPositions().at(newPos) - 1);
        else if (m_currentMap->breakableBlocks().at(newPos)) {
            m_currentMap->removeBreakableBlock(newPos);
            m_mapScene->removeItem(m_breakableBlocks[newPos]);
            m_breakableBlocks.remove(newPos);
        }

        m_mapScene->removeItem(flam);

        // Remove flam from the flam list
        QList<int> flamPos = m_gamefieldFlam.keys(flam);
        for (int i = 0; i < flamPos.length(); i++) {
            m_gamefieldFlam.remove(flamPos.at(i));
        }
        flam->~QGraphicsEllipseItem();

        if (m_gamefieldBomb.contains(newPos))
            onBombExplode(newPos);

        return;
    }

    flam->setPosition(newPos);
    m_gamefieldFlam[flam->position()] = flam;
}




/******************************************************************************/
/****************************** Rounds management *****************************/
/******************************************************************************/


void Gamefield::manageRounds() {
    // If there is no more player, switch to the next round.
    if (m_playerInGame < 2) {
        for (int i = 0; i < m_players.length(); i++) {
            if (m_players.at(i)->state() == PLAYER_DEAD)
                m_players.at(i)->setState(PLAYER_ALIVE);
        }

        m_currentRound++;
        if (m_currentRound > m_gameRoundsChoice->value()) {
            qDebug() << "game finished";
            m_gameState = GAME_FINISHED;
        }
        else {
            startNewRound();
        }
    }
}

void Gamefield::startNewRound() {
    for (int i = 0; i < PLAYER_NUMBER_MAX; i++) {
        if (m_players.at(i)->state() == PLAYER_DEAD) {
            m_players.at(i)->setState(PLAYER_ALIVE);
            setPlayerDefaultPos(i);
        }

        // Remove player from scene to avoid it to be deleted
        if (m_players.at(i)->scene() != 0)
            m_mapScene->removeItem(m_players.at(i));
    }

    m_mapScene->removeItem(m_gamefieldText);
    m_currentMap->parseMapRepr();
    drawMap();
    m_mapScene->addItem(m_gamefieldText);

    for (int i = 0; i < PLAYER_NUMBER_MAX; i++) {
        m_mapScene->addItem(m_players.at(i));
        m_currentMap->addCollision(m_currentMap->playersPositions().indexOf(i+1));
    }

    displayeCurrentRound(m_currentRound);
}

void Gamefield::displayeCurrentRound(int round) {
    if (round == 0) {
        m_gamefieldText->setVisible(false);
        m_gameState = GAME_RUNNING;
        return;
    }

    m_gameState = GAME_WAITING;
    QBrush brush(Qt::white);
    QPen pen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QFont font;
    font.setBold(QFont::Black);
    font.setStretch(QFont::Condensed);
    font.setCapitalization(QFont::SmallCaps);
    font.setPointSize(40);

    m_gamefieldText->setBrush(brush);
    m_gamefieldText->setPen(pen);
    m_gamefieldText->setFont(font);
    m_gamefieldText->setPos(m_mapScene->height() / 3, m_mapScene->width() / 3);

    m_gamefieldText->setText(tr("Round %1").arg(round));
    m_gamefieldText->setVisible(true);

    QTimer::singleShot(DISPLAY_CURRENT_ROUND_TIME, this, SLOT(displayeCurrentRound()));
}
