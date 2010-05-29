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
    m_mapView->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    m_mapView->setStyleSheet("background-color: #FFFFFF");


    // Initialize the game informations panel
    m_gamePanel = new QVBoxLayout;

    // Initialize players info
    for (int i = 0; i < PLAYER_NUMBER_MAX; i++) {
        QPushButton* playerInfos = new QPushButton(tr("Player %1").arg(i+1));
        m_playerInfosList.insert(i, playerInfos);
        m_gamePanel->addWidget(playerInfos);
        connect(playerInfos, SIGNAL(clicked()), this, SLOT(onPlayerInfosClicked()));
    }

    m_gameMapListBox = new QComboBox;
    m_gameMapListBox->setMaximumWidth(120);
    m_gamePanel->addWidget(m_gameMapListBox);
    connect(m_gameMapListBox, SIGNAL(activated(int)), this, SLOT(onMapSelected(int)));

    m_gameRoundsField = new QSpinBox;
    m_gameRoundsField->setMinimum(1);
    m_gameRoundsField->setMaximum(10);
    m_gameRoundsField->setValue(3);
    m_gamePanel->addWidget(m_gameRoundsField);
    

    QPushButton* startGameButton = new QPushButton(tr("Start playing"));
    m_gamePanel->addWidget(startGameButton);
    connect(startGameButton, SIGNAL(clicked()), this, SLOT(onGameStart()));

    QPushButton* aboutGameButton = new QPushButton(tr("About"));
    m_gamePanel->addWidget(aboutGameButton);
    // TODO: Create about dialog and connect it to the button

    QPushButton* exitGameButton = new QPushButton(tr("Exit game"));
    m_gamePanel->addWidget(exitGameButton);
    connect(exitGameButton, SIGNAL(clicked()), this, SLOT(onExitGame()));

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(m_mapView, 0, 0);
    mainLayout->addLayout(m_gamePanel, 0, 1);
    setLayout(mainLayout);

    m_gameState = GAME_PENDING;

    loadMaps();
    if (m_gameMapList.length() == 0) {
        QMessageBox::critical(this, tr("Game maps error"),
            tr("No valid maps or no <em>%1</em> directory found").arg(MAPS_DIRECTORY));
        startGameButton->setDisabled(true);
    }
    else {
        // Load the first map
        onMapSelected(0);
    }
}

/*! Recieve the exit game signal and handle it.
 *
 *  If the game is running, a warning message is displayed.
 */
void Gamefield::onExitGame() {
    if (m_gameState == GAME_RUNNING) {
        int ret = QMessageBox::warning(this, tr("Exit game confirmation"),
            tr("Do you really want to exit the game?"),
            QMessageBox::Ok | QMessageBox::Cancel);

        if (ret != QMessageBox::Ok)
            return;
    }

    QApplication::quit();
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
    int row = pos / m_gameMap->mapWidth();
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
    int row = pos / m_gameMap->mapWidth();
    int col = pos - (row * m_gameMap->mapWidth());

    return col * (BLOCK_WIDTH + BLOCK_SPACING);
}

/*! Game players initialization.
 *
 *  Initialize players and set it on the gamefield.
 */
void Gamefield::initPlayers() {
    m_playersList.clear();
    m_playerNumber = PLAYER_NUMBER_MIN;
    
    for (int i = 0; i < PLAYER_NUMBER_MAX; i++) {
        Player* player = new Player(i);

        m_playersList.insert(i, player);
        m_playerInfosList.at(i)->setStyleSheet("border: 3px solid gray");

        // Position of the player in the players position table
        int playerTabPos = m_gameMap->playersPositions().indexOf(i+1);

        if (i < PLAYER_NUMBER_MIN) {
            player->setPlayerStatus(PLAYER_ALIVE);
            m_gameMap->setCollisionsTab(playerTabPos, 1);
            m_mapScene->addItem(m_playersList.at(i));
            m_playerInfosList.at(i)->setStyleSheet("border: 3px solid blue");
        }

        // Add and move the player on the scene
        m_playersList.at(i)->setPos(widthFromPos(playerTabPos), heightFromPos(playerTabPos));
    }
}

/*! Draw the game map.
 *
 *  The map is drawn from the tables computed in the Map instance retrieved from
 *  the XML map file.
 */
void Gamefield::drawMap() {
    // Compute the map information before draw it
    if (m_gameMap->collisionsTab().count() == 0)
        m_gameMap->parseMapString();

    QBrush unbreakableBrush(Qt::darkGray);
    QPen unbreakablePen(Qt::black);
    QBrush breakableBrush(Qt::gray, Qt::Dense4Pattern);
    QPen breakablePen(Qt::gray);

    for (int i = 0; i < m_gameMap->collisionsTab().count(); i++) {
        if (m_gameMap->unbreakableBlocks().at(i)) {
            QGraphicsRectItem* unbreakableItem = new QGraphicsRectItem(0, 0,
                BLOCK_WIDTH, BLOCK_HEIGHT);
            unbreakableItem->setBrush(unbreakableBrush);
            unbreakableItem->setPen(unbreakablePen);
            unbreakableItem->moveBy(widthFromPos(i), heightFromPos(i));

            m_mapScene->addItem(unbreakableItem);
        }
        else if (m_gameMap->breakableBlocks().at(i)) {
            QGraphicsRectItem* breakableItem = new QGraphicsRectItem(0, 0,
                BLOCK_WIDTH, BLOCK_HEIGHT);
            breakableItem->setBrush(breakableBrush);
            breakableItem->setPen(breakablePen);
            breakableItem->moveBy(widthFromPos(i), heightFromPos(i));

            m_mapScene->addItem(breakableItem);
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
    int playerId = m_playerInfosList.indexOf(playerInfosLabel);

    if (m_playersList.at(playerId)->playerStatus() == PLAYER_NONE) {
        m_playersList.at(playerId)->setPlayerStatus(PLAYER_ALIVE);
        m_gameMap->setCollisionsTab(m_gameMap->playersPositions().indexOf(playerId+1), 1);
        m_mapScene->addItem(m_playersList.at(playerId));
        playerInfosLabel->setStyleSheet("border: 3px solid blue");
        m_playerNumber++;
    }
    else {
        if (m_playerNumber <= PLAYER_NUMBER_MIN)
            return;

        m_playersList.at(playerId)->setPlayerStatus(PLAYER_NONE);
        m_gameMap->setCollisionsTab(m_gameMap->playersPositions().indexOf(playerId+1), 0);
        m_mapScene->removeItem(m_playersList.at(playerId));
        playerInfosLabel->setStyleSheet("border: 3px solid gray");
        m_playerNumber--;
    }
}

void Gamefield::keyPressEvent(QKeyEvent* event) {

}

/*! Toggle game state.
 *
 *  This methods also performs some action as desactivate some elements of the
 *  interface.
 */
void Gamefield::onGameStart() {
    QPushButton* startGameButton = qobject_cast<QPushButton *>(sender());

    if (!startGameButton)
        return;

    if (m_gameState == GAME_PENDING) {
        m_gameState = GAME_RUNNING;
        startGameButton->setText("Stop playing");
        m_gameMapListBox->setDisabled(true);
        m_gameRoundsField->setDisabled(true);
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
        m_gameMapListBox->setEnabled(true);
        m_gameRoundsField->setEnabled(true);
    }
}

/*! Set the game map and draw the graphics are
 *
 *  @param map Map index to be set.
 */
void Gamefield::onMapSelected(int index) {
    if (m_gameState != GAME_PENDING)
        return;

    m_gameMap = m_gameMapList.at(index);

    // Release the old scene before create a new on
    if (m_mapView->scene() != 0)
        m_mapScene->~QGraphicsScene();

    m_mapScene = new QGraphicsScene;
    m_mapView->setScene(m_mapScene);
    drawMap();
    initPlayers();
}

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

    foreach (QFileInfo mapInfo, mapsList) {
        if (!mapInfo.isReadable())
            continue;

        // Discard the map if not valid
        GameMap* gameMap = new GameMap(mapInfo);
        if (!gameMap->isMapValid())
            continue;

        m_gameMapList.append(gameMap);
        m_gameMapListBox->addItem(gameMap->mapName());
    }
}
