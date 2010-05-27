#include "ui/multiplayerui.h"
#include "ui/gameui.h"
#include "ui/startui.h"

MultiplayerUI::MultiplayerUI(QWidget* parent) : m_parent(parent) {
    // TODO: disable field when player is none (or a bot)
    // TODO: save players settings
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* playersLayout = new QHBoxLayout;
    setDefaultKeys();

    /*
     * Create player settings UI
     */
    for (int currentPlayer = 0; currentPlayer < PLAYER_NUMBER; currentPlayer++) {
        QGroupBox* playerGroup = new QGroupBox(tr("Player %1").arg(currentPlayer+1));
        QFormLayout* playerOption = new QFormLayout;
        playerOption->setRowWrapPolicy(QFormLayout::WrapLongRows);

        // Player pseudo
        QLineEdit* playerPseudo = new QLineEdit;
        playerPseudo->setMinimumWidth(70);
        playerOption->addRow(tr("Pseudo:"), playerPseudo);
        m_playersPseudo[currentPlayer] = playerPseudo;

        // Player type (None, player, bot)
        QComboBox* playerType = new QComboBox;
        playerType->addItem(tr("None"));
        playerType->addItem(tr("Player"));
//        playerType.addItem(tr("Bot"));
        playerOption->addRow(tr("Type:"), playerType);
        m_playersType[currentPlayer] = playerType;

        // Player color
        QPushButton* playerColorButton = new QPushButton;
        playerColorButton->setFixedSize(70, 70);
        playerOption->addRow(tr("Color:"), playerColorButton);
        m_playersColorButton[currentPlayer] = playerColorButton;
        setPlayerColor(currentPlayer, QColor(PLAYER_DEFAULT_COLOR));

        // Player keys
        QLineEdit* playerKeyUp = new QLineEdit;
        playerOption->addRow(tr("Up key:"), playerKeyUp);
        QLineEdit* playerKeyDown = new QLineEdit;
        playerOption->addRow(tr("Down key:"), playerKeyDown);
        QLineEdit* playerKeyLeft = new QLineEdit;
        playerOption->addRow(tr("Left key:"), playerKeyLeft);
        QLineEdit* playerKeyRigt = new QLineEdit;
        playerOption->addRow(tr("Right key:"), playerKeyRigt);
        QLineEdit* playerKeyDrop = new QLineEdit;
        playerOption->addRow(tr("Drop key:"), playerKeyDrop);

        playerGroup->setLayout(playerOption);
        playersLayout->addWidget(playerGroup);

        QObject::connect(playerColorButton, SIGNAL(clicked()), this,
            SLOT(changePlayerColor()));
    }

    mainLayout->addLayout(playersLayout);

    /*
     * Create game settings UI 
     */

    QGroupBox* gameGroupbox = new QGroupBox;
    QGridLayout* gameLayout = new QGridLayout;

    // Number of rounds
    QLabel* gameRoundsLabel = new QLabel(tr("Rounds:"));
    gameLayout->addWidget(gameRoundsLabel, 0, 0, Qt::AlignLeft);
    m_gameRounds = new QSpinBox;
    m_gameRounds->setMaximum(10);
    m_gameRounds->setMinimum(1);
    m_gameRounds->setValue(3);
    gameLayout->addWidget(m_gameRounds, 0, 1, Qt::AlignLeft);

    // Map selection and informations
    QLabel* gameMapLabel = new QLabel(tr("Map:"));
    gameLayout->addWidget(gameMapLabel, 1, 0, Qt::AlignLeft);
    m_mapComboBox = new QComboBox;
    gameLayout->addWidget(m_mapComboBox, 1, 1, Qt::AlignLeft);
    QPushButton* moreInfoButton = new QPushButton(tr("Infos"));
    gameLayout->addWidget(moreInfoButton, 1, 2, Qt::AlignLeft);

    // Action buttons
    QPushButton* playButton = new QPushButton(tr("Play"));
    gameLayout->addWidget(playButton, 0, 1, 1, 2, Qt::AlignRight);
    QPushButton* backButton = new QPushButton(tr("Back"));
    gameLayout->addWidget(backButton, 1, 2, Qt::AlignRight);

    QObject::connect(backButton, SIGNAL(clicked()), this, SLOT(displayStartUI()));
    QObject::connect(playButton, SIGNAL(clicked()), this, SLOT(startGame()));

    gameGroupbox->setLayout(gameLayout);
    mainLayout->addWidget(gameGroupbox);

    setLayout(mainLayout);

    // Loads the maps
    loadMaps();
}

/*! Display a color dialog to let user choose is player color
 *
 *  If no color is selected the old one is keeped
 */
void MultiplayerUI::changePlayerColor() {
    QPushButton* playerButton = qobject_cast<QPushButton *>(sender());
    if (!playerButton)
        return;

    // Retrieve the ID of the player to change the color.
    int playerId = m_playersColorButton.key(playerButton);
    // The old color is set by default in the dialog.
    QColor playerColor = m_playersColor[playerId];
    QColor newPlayerColor;

    newPlayerColor = QColorDialog::getColor(playerColor, this, tr("Player color"));

    if (newPlayerColor.isValid())
        setPlayerColor(playerId, newPlayerColor);
}

/*! Set the color of a player
 *
 *  @param playerId ID of the player to set the color on
 *  @param color The color to set to the player
 */
void MultiplayerUI::setPlayerColor(int playerId, QColor color) {
    if (!m_playersColorButton.contains(playerId))
        return;

    // Set the button style color and the value in the player color table.
    QString style = "background-color: rgb(%1, %2, %3);";
    m_playersColorButton[playerId]->setStyleSheet(style.arg(color.red()).arg(color.green()).arg(color.blue()));
    m_playersColor[playerId] = color;
}

/*! Go back to the start interface
 */
void MultiplayerUI::displayStartUI() {
    GameUI* gameUI = qobject_cast<GameUI *>(m_parent);
    StartUI* startUI = qobject_cast<StartUI *>(gameUI->getLayout("start"));
    startUI->displayLayout("start");
}

/*! Set the default key for the players
 *
 *  Remember to change this if the maximum player number is changed
 */
void MultiplayerUI::setDefaultKeys() {
//    QMap<QString, int> playerkeys1;
//    playerkeys1["up"] = Qt::Key_Up;
//    playerkeys1["down"] = Qt::Key_Down;
//    playerkeys1["left"] = Qt::Key_Left;
//    playerkeys1["right"] = Qt::Key_Right;
//    playerkeys1["drop"] = Qt::Key_Space;
//    m_playersKey.insertMulti(0, playerkeys1);
//
//    QMap<QString, int> playerkeys2;
//    playerkeys2["up"] = Qt::Key_Z;
//    playerkeys2["down"] = Qt::Key_S;
//    playerkeys2["left"] = Qt::Key_Q;
//    playerkeys2["right"] = Qt::Key_D;
//    playerkeys2["drop"] = Qt::Key_E;
//    m_playersKey.insert(1, playerkeys2);
//
//    QMap<QString, int> playerkeys3;
//    playerkeys3["up"] = Qt::Key_I;
//    playerkeys3["down"] = Qt::Key_K;
//    playerkeys3["left"] = Qt::Key_J;
//    playerkeys3["right"] = Qt::Key_L;
//    playerkeys3["drop"] = Qt::Key_O;
//    m_playersKey.insert(2, playerkeys3);
//
//    QMap<QString, int> playerkeys4;
//    playerkeys4["up"] = Qt::Key_G;
//    playerkeys4["down"] = Qt::Key_B;
//    playerkeys4["left"] = Qt::Key_V;
//    playerkeys4["right"] = Qt::Key_N;
//    playerkeys4["drop"] = Qt::Key_H;
//    m_playersKey.insert(3, playerkeys4);
}

/*! Load and set map in the map list.
 *
 *  For each map XML file, an instance of the Map class is created.
 */
void MultiplayerUI::loadMaps() {
    QDir mapsDir;

    if (!mapsDir.cd(QObject::tr("%1/%2").arg(QApplication::applicationDirPath()).arg(MAPS_DIRECTORY)))
        return;

    QStringList mapsExt("*.xml");
    mapsDir.setFilter(QDir::Files);
    mapsDir.setNameFilters(mapsExt);

    QFileInfoList mapsList = mapsDir.entryInfoList();

    foreach (QFileInfo mapInfo, mapsList) {
        if (!mapInfo.isReadable())
            continue;

        Map* gameMap = new Map(mapInfo);
        if (!gameMap->isValid())
            continue;

        m_mapComboBox->addItem(gameMap->mapName());
        m_mapList[m_mapComboBox->currentIndex()] = gameMap;
    }
}

/*! Do some check and start the game.
 * 
 *  Each player have to chose a differents color but pseudo can be the same. 
 */
void MultiplayerUI::startGame() {
    QString errorMsg = "";
    int nonePlayers = 0;

    for (int i = 0; i < PLAYER_NUMBER; i++) {
        if (m_playersType[i]->currentText() == tr("None")) {
            nonePlayers++;
            continue;
        }
        
        if (m_playersPseudo[i]->text().trimmed().length() < 3) {
            errorMsg = tr("Pseudo for player %1 must be at least 3 character long").arg(i+1);
            break;
        }

        QList<int> playersWithSameColor = m_playersColor.keys(m_playersColor[i]);
        if (playersWithSameColor.length() > 1) {
            QString incriminedPlayers = QString::number(i+1);

            for (int j = 0; j < playersWithSameColor.length(); j++) {
                if (m_playersType[playersWithSameColor.at(j)]->currentText() != tr("None")
                    && playersWithSameColor.at(j) != i) {
                    incriminedPlayers += tr(" and %1").arg(playersWithSameColor.at(j)+1);
                }
            }

            if (incriminedPlayers.length() > 1) {
                errorMsg = tr("Player %1 have the same color").arg(incriminedPlayers);
                break;
            }
        }
    }

    if (nonePlayers > (PLAYER_NUMBER - PLAYER_NUMBER_MIN))
        errorMsg = tr("You need at least %1 to play the game").arg(PLAYER_NUMBER_MIN);

    if (errorMsg.length() > 0) {
        QMessageBox::warning(this, tr("Bomberman game error"), errorMsg);
    }
    else {
        qDebug() << "Launching game";
    }
}

/*! Define the title of the layout
 *
 *  @return Title of the layout
 */
QString MultiplayerUI::layoutTitle() {
    return tr("Multiplayer game - BomberMan");
}
