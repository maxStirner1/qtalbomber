#include <QtGui>
#include <QPushButton>
#include <QMessageBox>
#include <QtWidgets>
#include "gamefield.h"

/*! Creates interface of the game.
 *
 *  This object holds all the graphics items of the game.
 */
GameField::GameField() {
    setWindowTitle(tr("QtalBomber - Bomberman game"));
    setWindowIcon(QIcon(":/qtal-icon"));
    setMinimumSize(670, 535);
    // Set the window as a main window to avoid problems on Windows and Mac.
    setWindowFlags(Qt::Window);

    m_scene = new QGraphicsScene;
    m_mapView = new QGraphicsView(m_scene);
    m_mapView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    m_mapView->setStyleSheet("background-color: #FFFFFF");

    m_gameInfoPanel = new QVBoxLayout;
    
    for (int i = 0; i < PLAYER_NUMBER_MAX; i++) {
        QPushButton* playerInfos = new QPushButton(tr("Player %1").arg(i + 1));
        playerInfos->setMinimumHeight(70);
        m_playersInfosDisplay.insert(i, playerInfos);
        m_gameInfoPanel->addWidget(playerInfos);
        connect(playerInfos, SIGNAL(clicked()), this, SLOT(onPlayerInfosClicked()));

        // Initilize players score
        m_playersScore.append(0);
    }

    m_mapList = new QComboBox;
    m_mapList->setMaximumWidth(120);
    m_gameInfoPanel->addWidget(m_mapList);
    connect(m_mapList, SIGNAL(activated(int)), this, SLOT(selectMap(int)));

    QHBoxLayout* roundsBox = new QHBoxLayout;
    roundsBox->addWidget(new QLabel("Rounds:"));
    m_gameRoundsChoice = new QSpinBox;
    m_gameRoundsChoice->setMinimum(ROUNDS_MIN);
    m_gameRoundsChoice->setMaximum(ROUNDS_MAX);
    m_gameRoundsChoice->setValue(ROUNDS_DEFAULT);
    roundsBox->addWidget(m_gameRoundsChoice);
    m_gameInfoPanel->addLayout(roundsBox);

    m_startGameButton = new QPushButton(tr("Start playing"));
    m_gameInfoPanel->addWidget(m_startGameButton);
    connect(m_startGameButton, SIGNAL(clicked()), this, SLOT(toggleGameState()));

    QPushButton* aboutGameButton = new QPushButton(tr("About"));
    connect(aboutGameButton, SIGNAL(clicked()), this, SLOT(toggleAboutDialog()));
    m_gameInfoPanel->addWidget(aboutGameButton);
    m_aboutDialog = new About;

    QPushButton* exitGameButton = new QPushButton(tr("Exit game"));
    m_gameInfoPanel->addWidget(exitGameButton);
    connect(exitGameButton, SIGNAL(clicked()), this, SLOT(exitGame()));

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(m_mapView, 0, 0);
    mainLayout->addLayout(m_gameInfoPanel, 0, 1);
    setLayout(mainLayout);

    m_music = new QSound(tr("%1/Music.wav").arg(SOUNDS_DIR), this);
    m_music->setLoops(-1);
    m_musicPlaying = false;

    m_gameState = GAME_PENDING;
    m_currentRound = 1;

    QSound* intro = new QSound(tr("%1/Intro.wav").arg(SOUNDS_DIR), this);
    intro->setLoops(1);
    intro->play();

    loadMaps();
    if (m_availableMaps.length() == 0) {
        QMessageBox::critical(this, tr("Game maps error"),
            tr("No valid maps or no <em>%1</em> directory found").arg(MAPS_DIR));
        // Hide the main window when no map are found
        setVisible(false);
    }
    else {
        // Load the first map
        selectMap(0);
    }
}

/*! Set the selected map.
 *
 *  @param index Index of the selected map.
 */
void GameField::selectMap(int index) {
    if (m_gameState != GAME_PENDING)
        return;

    m_level = m_availableMaps.at(index);

    drawArena();
    m_scene->setSceneRect(0, 0, m_level.width()*(BLOCK_WIDTH + BLOCK_SPACING), m_level.height()*(BLOCK_HEIGHT + BLOCK_SPACING));
    m_gamefieldText = new QGraphicsSimpleTextItem;
    m_scene->addItem(m_gamefieldText);
    drawCharacters();
}

/*! Removes players who are not in the game
 *
 */
void GameField::removeUndesiredPlayers() {
    for (int i = 0; i < PLAYER_NUMBER_MAX; i++) {
        if (m_players.at(i)->status() == PLAYER_OFF)
            m_level.setPlayerPosition(i + 1, 0); //Remove undesired players
    }

}

/*! Rounds countdown message management.
 *
 *  Define the text to display depending of the elements remaining in the startText.
 */
void GameField::displayCountdown() {
    if (m_startText.isEmpty()) {
        QTimer* countdown = qobject_cast<QTimer *>(sender());

        if (!countdown)
            return;

        countdown->stop();
        m_gamefieldText->setVisible(false);
        m_gameState = GAME_RUNNING;

        if (!m_musicPlaying) {
            m_music->play();
            m_musicPlaying = true;
        }

        return;
    }

    m_gameState = GAME_PAUSED;
    writeMapText(m_startText.takeFirst(), m_startText.takeFirst().toInt());
}

/*! Insert text on the gamefield scene.
 *
 *  @param text Text to display
 *  @param size Font size
 */
void GameField::writeMapText(QString text, int size) {
    m_gamefieldText->setText(text);
    m_gamefieldText->setVisible(true);

    QBrush brush(Qt::white);
    QPen pen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QFont font;
    font.setBold(QFont::Black);
    font.setStretch(QFont::Condensed);
    font.setCapitalization(QFont::SmallCaps);
    font.setPointSize(size);

    m_gamefieldText->setPos((m_scene->width()-(text.length()*25)) / 2, m_scene->height() / 3);

    m_gamefieldText->setBrush(brush);
    m_gamefieldText->setPen(pen);
    m_gamefieldText->setFont(font);
}

/*! Display the about dialog window
 *
 */
void GameField::toggleAboutDialog() {
    if (m_aboutDialog->isVisible())
        m_aboutDialog->setVisible(false);
    else
        m_aboutDialog->setVisible(true);
}

/*! Loads all the maps in the maps directory.
 *
 *  If the a map is invalid, it is discarded.
 */
void GameField::loadMaps() {
    QDir mapsDir;

    if (!mapsDir.cd(QObject::tr("%1/%2").arg(QApplication::applicationDirPath()).arg(MAPS_DIR)))
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
        Level gameMap(mapInfo.absoluteFilePath());
        if (!gameMap.isValid())
            continue;

        m_availableMaps.append(gameMap);
        m_mapList->addItem(gameMap.name());
    }
}

/*! Exit game with confirmation
 *
 *  The confirmation is display only if the game is running.
 */
void GameField::exitGame() {
    if (m_gameState == GAME_RUNNING) {
        int ret = QMessageBox::warning(this, tr("Exit game confirmation"),
            tr("Do you really want to exit the game?"),
            QMessageBox::Ok | QMessageBox::Cancel);

        if (ret != QMessageBox::Ok)
            return;
    }

    QApplication::quit();
}

/*! Stops and starts the current game
 *
 *  @param notify Disable confirmation on stop game.
 */
void GameField::toggleGameState(bool notify) {
    if (m_gameState == GAME_PENDING) {
        m_startGameButton->setText(tr("Stop playing"));
        m_mapList->setDisabled(true);
        m_gameRoundsChoice->setDisabled(true);

        grabKeyboard();
        focusNextChild();
        removeUndesiredPlayers();
        startCountdown();
    }
    else {
        // Don't display a warning if game is finished
        if (m_gameState == GAME_RUNNING && notify) {
            int ret = QMessageBox::warning(this, tr("Stop game confirmation"),
                tr("Do you really want to stop the game?"),
                QMessageBox::Ok | QMessageBox::Cancel);

            if (ret == QMessageBox::Cancel)
                return;
        }
        m_gameState = GAME_PENDING;
        m_startGameButton->setText(tr("Start playing"));
        m_mapList->setEnabled(true);
        m_gameRoundsChoice->setEnabled(true);
        m_music->stop();
        m_musicPlaying = false;
        m_playerInGame = 0;
        resetGamefield();
        m_currentRound = 1;

        for (int i = 0; i < m_players.size(); i++) {
            m_playersScore.replace(i, 0);
        }
    }
}

/*! Initializes the countdown between rounds.
 *
 *  m_startText contains alternatively the size and the text.
 */
void GameField::startCountdown() {
    m_startText.append("60");
    m_startText.append(tr("Round %1").arg(m_currentRound));

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

    QTimer* countdown = new QTimer(this);
    countdown->setInterval(HUD_TIME);
    connect(countdown, SIGNAL(timeout()), this, SLOT(displayCountdown()));
    countdown->start();
}

/*! Converts coordinates x and y to position in the map table.
 *
 *  @param x item graphic abscissa
 *  @param y item graphic ordinate
 */
int GameField::indexFromCoord(int x, int y) {
    int ax = x / elementSize();
    int ay = y / elementSize();
    return ax + ay * m_level.height();
}

/*!
 * Places the bomb on the players coordinates
 * Sets a collision
 * Creates flams for 4 directions, flames are invisibles and not dangerous
 * @param player Player who drop the bomb
 */
void GameField::dropBomb(Player* player) {
    if (player->currentBombCapacity() >= player->bombCapacity() || !player->isAlive())
        return;

    Bomb* bomb = new Bomb(player->playerID());
    player->addCurrentBombCapacity();
    Flam* flamUp = new Flam(MOVE_UP, player->bombRange());
    Flam* flamDown = new Flam(MOVE_DOWN, player->bombRange());
    Flam* flamRight = new Flam(MOVE_RIGHT, player->bombRange());
    Flam* flamLeft = new Flam(MOVE_LEFT, player->bombRange());

    connect(bomb, SIGNAL(disappear()), this, SLOT(eraseBomb()));
    connect(bomb, SIGNAL(blowUp()), flamUp, SLOT(triggerFlam()));
    connect(bomb, SIGNAL(blowUp()), flamDown, SLOT(triggerFlam()));
    connect(bomb, SIGNAL(blowUp()), flamRight, SLOT(triggerFlam()));
    connect(bomb, SIGNAL(blowUp()), flamLeft, SLOT(triggerFlam()));

    connect(flamUp, SIGNAL(disappear()), this, SLOT(eraseFlam()));
    connect(flamDown, SIGNAL(disappear()), this, SLOT(eraseFlam()));
    connect(flamRight, SIGNAL(disappear()), this, SLOT(eraseFlam()));
    connect(flamLeft, SIGNAL(disappear()), this, SLOT(eraseFlam()));

    connect(flamUp, SIGNAL(collide()), this, SLOT(collisionChecking()));
    connect(flamDown, SIGNAL(collide()), this, SLOT(collisionChecking()));
    connect(flamRight, SIGNAL(collide()), this, SLOT(collisionChecking()));
    connect(flamLeft, SIGNAL(collide()), this, SLOT(collisionChecking()));

    m_scene->addItem(bomb);
    bomb->setPos(player->scenePos());

    m_scene->addItem(flamUp);
    m_scene->addItem(flamDown);
    m_scene->addItem(flamRight);
    m_scene->addItem(flamLeft);

    flamUp->setPos(player->scenePos());
    flamDown->setPos(player->scenePos());
    flamRight->setPos(player->scenePos());
    flamLeft->setPos(player->scenePos());

    m_bombs.append(bomb);
    m_level.setCollision(indexFromCoord(bomb->x(), bomb->y()), 1);
    bomb->dropBomb();
}

/*!
 * Refunds used bomb to the player
 * Removes bomb collision from the map
 */
void GameField::eraseBomb() {
    Bomb *bomb = qobject_cast<Bomb*>(sender());

    if (!bomb)
        return;

    for (int i = 0; i < 4; i++) {
        if (bomb->getOwnerId() == i) {
            m_players.at(i)->removeCurrentBombCapacity();
        }
    }

    m_level.setCollision(indexFromCoord(bomb->x(), bomb->y()), 0);
}

/*!
 * Hides flam
 */
void GameField::eraseFlam() {
    Flam *flam = qobject_cast<Flam*>(sender());

    if (!flam)
        return;

    flam->setVisible(false);
    //    flam->deleteLater();

}

/*!
 * Generates a bonus randomly when the block is destroyed
 * @param position Coordinate of currently block
 */
void GameField::generateBonus(QPointF position) {
    if (qrand() % 10 > 7) {
        Bonus* randomBonus = new Bonus((qrand() % 3) + 1);
        m_scene->addItem(randomBonus);
        randomBonus->setPos(position);
        m_level.setBonus(indexFromCoord(position.x(), position.y()), randomBonus->type());
        m_bonuses.append(randomBonus);
    }
    else {
        return;
    }
}

/*!
 * Manage flams collision
 * There are 4 different ways to collide.
 * - If the flam collides with a block, the flam is hidden. If block is breakable, the block is destroyed.
 * - If the flam collides with a bomb, it exploses it.
 * - If the flam collides with a player, it removes a life to the player. If player has not lifes, he is killed.
 * - If the flam collides with a bonus, it is destroyed.
 */
void GameField::collisionChecking() {
    Flam *flamCopy = qobject_cast<Flam*>(sender());

    if (!flamCopy) {
        return;
    }

    if (!flamCopy->hasCollided()) {
        for (int i = 0; i < m_blocksArena.size(); i++) {
            if (flamCopy->collidesWithItem(m_blocksArena.at(i))) {
                if (m_blocksArena.at(i)->isBreakable()) {
                    flamCopy->setVisible(false);
                    flamCopy->setHasCollided(true);
                    m_blocksArena.at(i)->setVisible(false);
                    m_scene->removeItem(m_blocksArena.at(i));
                    int j = indexFromCoord(m_blocksArena.at(i)->x(), m_blocksArena.at(i)->y());
                    m_level.setCollision(j, 0);
                    m_level.setBreakableBlock(j, 0);

                    generateBonus(m_blocksArena.at(i)->pos());

                    m_blocksArena.at(i)->~Block();
                    m_blocksArena.removeAt(i);
                }
                else {
                    flamCopy->setVisible(false);
                    flamCopy->setHasCollided(true);
                }
            }
        }
        for (int i = 0; i < m_bombs.size(); i++) {
            if (flamCopy->collidesWithItem(m_bombs.at(i))) {
                m_bombs.at(i)->explode();
                //m_bombs.removeAt(i);
            }
        }
        for (int i = 0; i < m_bonuses.size(); i++) {
            if (flamCopy->collidesWithItem(m_bonuses.at(i))) {
                m_scene->removeItem(m_bonuses.at(i));
                m_bonuses.at(i)->setVisible(false);
                m_level.setBonus(indexFromCoord(m_bonuses.at(i)->x(), m_bonuses.at(i)->y()), 0);
            }
        }
        if (m_level.playerPosition().at(indexFromCoord(flamCopy->x(), flamCopy->y())) > 0) {
            int playerId = m_level.playerPosition().at(indexFromCoord(flamCopy->x(), flamCopy->y())) - 1; //Remember a QList starts at 0 or thr first player got the ID 1
            if (!m_players.at(playerId)->isAlive())
                return;

            m_players.at(playerId)->removeLifes();
            if (m_players.at(playerId)->lifes() < 1) {
                setPlayerDead(playerId);
            }
        }
    }
}

/*!
 * Catchs key press event and executes appropriate function
 * In this for player controls
 * @param event Keyboard action
 */
void GameField::keyPressEvent(QKeyEvent *event) {
    if (m_gameState != GAME_RUNNING)
        return;

    switch (event->key()) {
        case Qt::Key_Up:
            move(MOVE_UP, m_players.at(0));
            break;
        case Qt::Key_Down:
            move(MOVE_DOWN, m_players.at(0));
            break;
        case Qt::Key_Right:
            move(MOVE_RIGHT, m_players.at(0));
            break;
        case Qt::Key_Left:
            move(MOVE_LEFT, m_players.at(0));
            break;
        case Qt::Key_Control:
            dropBomb(m_players.at(0));
            break;
        case Qt::Key_Z:
            move(MOVE_UP, m_players.at(1));
            break;
        case Qt::Key_S:
            move(MOVE_DOWN, m_players.at(1));
            break;
        case Qt::Key_D:
            move(MOVE_RIGHT, m_players.at(1));
            break;
        case Qt::Key_Q:
            move(MOVE_LEFT, m_players.at(1));
            break;
        case Qt::Key_A:
            dropBomb(m_players.at(1));
            break;
        case Qt::Key_U:
            move(MOVE_UP, m_players.at(2));
            break;
        case Qt::Key_J:
            move(MOVE_DOWN, m_players.at(2));
            break;
        case Qt::Key_K:
            move(MOVE_RIGHT, m_players.at(2));
            break;
        case Qt::Key_H:
            move(MOVE_LEFT, m_players.at(2));
            break;
        case Qt::Key_Y:
            dropBomb(m_players.at(2));
            break;
        case Qt::Key_G:
            move(MOVE_UP, m_players.at(3));
            break;
        case Qt::Key_B:
            move(MOVE_DOWN, m_players.at(3));
            break;
        case Qt::Key_N:
            move(MOVE_RIGHT, m_players.at(3));
            break;
        case Qt::Key_V:
            move(MOVE_LEFT, m_players.at(3));
            break;
        case Qt::Key_Space:
            dropBomb(m_players.at(3));
            break;
    }
}

/*!
 * Manages player deplacement
 * Manages collisions between player and collision items
 * @param direction Direction in which the player moves
 * @param item Current player
 */
void GameField::move(int direction, Player *item) {
    if (!item->isAlive())
        return;

    qreal dx = 0;
    qreal dy = 0;

    switch (direction) {
        case MOVE_UP:
            dx = 0;
            dy = -(BLOCK_SPACING + BLOCK_HEIGHT);
            item->setPix(MOVE_UP);
            break;
        case MOVE_DOWN:
            dx = 0;
            dy = (BLOCK_SPACING + BLOCK_HEIGHT);
            item->setPix(MOVE_DOWN);
            break;
        case MOVE_RIGHT:
            dx = (BLOCK_SPACING + BLOCK_WIDTH);
            dy = 0;
            item->setPix(MOVE_RIGHT);
            break;
        case MOVE_LEFT:
            dx = -(BLOCK_SPACING + BLOCK_WIDTH);
            dy = 0;
            item->setPix(MOVE_LEFT);
            break;
    }

    int x = item->x() + dx;
    int y = item->y() + dy;
    int i = indexFromCoord(x, y);

    if (m_level.collisions().at(i) == 0) {
        m_level.setPlayerPosition(indexFromCoord(item->x(), item->y()), 0);
        item->moveBy(dx, dy);
        m_level.setPlayerPosition(i, item->playerID() + 1);

        if (m_level.bonuses().at(i) > 0) {
            for (int i = 0; i < m_bonuses.size(); i++) {
                if (m_bonuses.at(i)->x() == item->x() && m_bonuses.at(i)->y() == item->y()) {
                    item->setPlayerAttributes(m_bonuses.at(i)->attributes());
                    m_scene->removeItem(m_bonuses.at(i));
                    m_bonuses.removeAt(i);
                    m_level.setBonus(i, 0);
                }
            }
        }
    }
}

/*!
 * Creates new players and places them on the map
 * There must be at least a minimum number of players defined by PLAYER_NUMBER_MAX
 */
void GameField::drawCharacters() {
    m_players.clear();
    m_playerInGame = 0;

    for (int i = 0; i < PLAYER_NUMBER_MAX; i++) {
        Player* player = new Player(i);

        m_players.insert(i, player);

        if (i < PLAYER_NUMBER_MIN)
            setPlayerAlive(i);
        else
            setPlayerOff(i);

        setPlayerDefaultPos(i);
    }
}

/*!
 * Activates and desactivates players.
 * There are a minimum number of players who can be activated, this number is defined in a constant.
 * There are a maximum number of players who can be activated, this number is defined in a constant.
 */
void GameField::onPlayerInfosClicked() {
    if (m_gameState != GAME_PENDING)
        return;

    // Label which send the signal
    QPushButton* playerInfosLabel = qobject_cast<QPushButton *>(sender());

    if (!playerInfosLabel)
        return;

    // Retrieve the player id
    int playerId = m_playersInfosDisplay.indexOf(playerInfosLabel);

    if (m_players.at(playerId)->status() == PLAYER_OFF)
        setPlayerAlive(playerId);
    else {
        if (m_playerInGame > PLAYER_NUMBER_MIN)
            setPlayerOff(playerId);
    }
}

/*!
 * Return after counting, the number of dead players
 */
int GameField::countDeadPlayer() {
    int deadPlayers = 0;
    for (int i = 0; i < m_players.size(); i++) {
        if (!m_players.at(i)->isAlive() && m_players.at(i)->status() == PLAYER_IN)
            deadPlayers++;
    }

    return deadPlayers;
}

/*!
 * Calls function to change players status
 * Sets StyleSheet to the player button
 * Triggers death animation to the player
 * Decrements the number of alive players
 * Adds player score
 * Calls manageRounds function to check if there are players in games
 * @param playerID Current player ID
 */
void GameField::setPlayerDead(int playerId) {
    // Add player score
    m_playersScore.replace(playerId, m_playersScore.at(playerId) + countDeadPlayer());
    m_players.at(playerId)->setPlayerAlive(false);
    //    m_scene->removeItem(m_players.at(playerId));
    m_playersInfosDisplay.at(playerId)->setStyleSheet("border: 3px solid red");
    m_playersInfosDisplay.at(playerId)->setIcon(QIcon(":/player-off"));
    m_players.at(playerId)->startDeathAnimation();
    m_level.setPlayerPosition(playerId + 1, 0);
    m_playerInGame--;

    manageRounds();
}

/*!
 * Adds player to the scene
 * Changes player status to PLAYER_IN
 * Increments the numbre of alive players
 * @param playerID Current player ID
 */
void GameField::setPlayerAlive(int playerId) {
    m_players.at(playerId)->setStatus(PLAYER_IN);
    m_players.at(playerId)->setPlayerAlive(true);
    if (m_players.at(playerId)->scene() == 0)
        m_scene->addItem(m_players.at(playerId));
    m_playersInfosDisplay.at(playerId)->setStyleSheet("border: 3px solid blue");
    m_playersInfosDisplay.at(playerId)->setIcon(QIcon(tr(":/player-%1-2-0").arg(playerId)));
    m_playerInGame++;
}

/*!
 * Removes player from the scene
 * Changes player status to PLAYER_OFF
 * Sets StyleSheet to the player button
 * Decrements the numbre of player in the game if the player is in the scene
 * @param playerID Current player ID
 */
void GameField::setPlayerOff(int playerId) {
    if (m_players.at(playerId)->status() == PLAYER_IN) {
        m_playerInGame--;

        if (m_players.at(playerId)->isAlive()) {
            m_scene->removeItem(m_players.at(playerId));
        }
    }

    m_players.at(playerId)->setStatus(PLAYER_OFF);
    m_players.at(playerId)->setPlayerAlive(false);
    m_playersInfosDisplay.at(playerId)->setStyleSheet("border: 3px solid gray");
    m_playersInfosDisplay.at(playerId)->setIcon(QIcon(":/player-off"));
}

/*!
 * Places player depending his ID on the map
 * @param playerID Current player ID
 */
void GameField::setPlayerDefaultPos(int playerId) {
    int playerTabPos = m_level.playerPosition().indexOf(playerId + 1);
    m_players.at(playerId)->setPos(widthFromPos(playerTabPos), heightFromPos(playerTabPos));
}

/*!
 * Calculates the height relative to the origin of the map
 * @param pos Position in the string map
 */
int GameField::heightFromPos(int pos) {
    int row = pos / m_level.width();
    return row * (BLOCK_HEIGHT + BLOCK_SPACING);
}

/*!
 * Calculates the width relative to the origin of the map
 * @param pos Position in the string map
 */
int GameField::widthFromPos(int pos) {
    int row = pos / m_level.width();
    int col = pos - (row * m_level.width());

    return col * (BLOCK_WIDTH + BLOCK_SPACING);
}

/*!
 * Deletes items on the map
 * Sets items positions to the map depending the map architecture
 */
void GameField::drawArena() {
    m_blocksArena.clear();
    m_bonuses.clear();
    m_bombs.clear();

    qDeleteAll(m_scene->items());

    for (int i = 0; i < m_level.unbreakableBlocks().size(); i++) {
        Block* newBlock;
        if (m_level.unbreakableBlocks().at(i) == 1)
            newBlock = new Block(false);
        else if (m_level.breakableBlocks().at(i) == 1)
            newBlock = new Block(true);
        else
            continue;

        int y = i / m_level.height();
        int x = i - (y * m_level.height());
        newBlock->setPos(x * elementSize(), y * elementSize());
        m_blocksArena.append(newBlock);
        m_scene->addItem(newBlock);
    }
}

/*!
 * Adds block height with its spacing
 */
int GameField::elementSize() {
    return BLOCK_HEIGHT + BLOCK_SPACING;
}

/*!
 * When only one player remains alive, it :
 * - When it's the last round :
 * -- Calls displayFinalScores function to display scores
 * -- Sets game state to finished
 * - else :
 * -- Inscrements current round
 * -- Set the score of the winner
 */
void GameField::manageRounds() {
    // If there is no more player, switch to the next round.
    if (m_playerInGame < 2) {
        m_currentRound++;

        int winner = -1;
        for (int i = 0; i < m_players.size(); i++) {
            if (m_players.at(i)->isAlive()) {
                winner = i;
                m_playersScore.replace(i, m_playersScore.at(i) + countDeadPlayer());
                break;
            }
        }

        if (m_gameRoundsChoice->value() < m_currentRound) {
            displayFinalScores();
            m_gameState = GAME_FINISHED;
        }
        else {

            if (winner == -1)
                writeMapText(tr("Draw round"), 40);
            else
                writeMapText(tr("Player %1 has won").arg(winner + 1), 60);

            QTimer::singleShot(ON_ROUND_TIMER, this, SLOT(resetGamefield()));
            QTimer::singleShot(ON_ROUND_TIMER, this, SLOT(removeUndesiredPlayers()));
            QTimer::singleShot(ON_ROUND_TIMER, this, SLOT(startCountdown()));
        }
    }
}

/*!
 * Creates text items to display the final score
 * Display final scores of all players
 */
void GameField::displayFinalScores() {
    QGraphicsSimpleTextItem* scoreLabel = new QGraphicsSimpleTextItem(tr("Scores :"));
    QFont font;
    font.setBold(QFont::Black);
    font.setStretch(QFont::Condensed);
    font.setCapitalization(QFont::SmallCaps);
    font.setPointSize(70);

    scoreLabel->setPos(100, 50);
    scoreLabel->setBrush(QBrush(Qt::white));
    scoreLabel->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    scoreLabel->setFont(font);
    m_scene->addItem(scoreLabel);

    for (int i = 0; i < m_playersScore.size(); i++) {
        QString value;
        if (m_players.at(i)->status() == PLAYER_OFF)
            value = tr("none");
        else
            value = QString::number(m_playersScore.at(i));

        QGraphicsSimpleTextItem* score = new QGraphicsSimpleTextItem(tr("Player %1: %2").arg(i + 1).arg(value));

        QFont font;
        font.setBold(QFont::Black);
        font.setStretch(QFont::Condensed);
        font.setPointSize(50);

        score->setPos(100, 140 + (60 * i));
        score->setBrush(QBrush(Qt::white));
        score->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        score->setFont(font);
        m_scene->addItem(score);
    }
}

/*!
 * Recreates map and players
 * @see GameField::manageRounds
 */
void GameField::resetGamefield() {
    QList<int> playersEngaged;
    for (int i = 0; i < PLAYER_NUMBER_MAX; i++) {
        playersEngaged.append(m_players.at(i)->status() == PLAYER_IN);
    }

    m_level.parseMapRepr();
    drawArena();
    m_gamefieldText = new QGraphicsSimpleTextItem;
    m_scene->addItem(m_gamefieldText);

    m_playerInGame = 0;
    for (int i = 0; i < playersEngaged.length(); i++) {
        Player* player = new Player(i);
        m_players.replace(i, player);
        if (playersEngaged.at(i)) {
            setPlayerAlive(i);
        }
        setPlayerDefaultPos(i);
    }
}
