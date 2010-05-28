#include "multiplayerui.h"
#include "gameui.h"
#include "gamefield.h"
#include "player.h"
#include "multiplayerhandler.h"


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
    QObject::connect(m_mapComboBox, SIGNAL(activated(int)), this,
        SLOT(selectMap(int)));

    // Append map to combo box
    GameUI* gameUI = qobject_cast<GameUI *>(m_parent);
    qDebug() << gameUI->mapList();
    for (int i=0; i < gameUI->mapList().count(); i++) {
        m_mapComboBox->addItem(gameUI->mapList().at(i)->mapName());
    }
    if (m_mapComboBox->currentIndex() > 0)
        m_selectedMap = 0;


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
    qobject_cast<GameUI *>(m_parent)->setVisibleUI("start");
}

/*! Set the default key for the players
 *
 *  The list generated by this method is used by the event handler.
 *  The keys and information are organized as follow:
 *      (key, (playerId, action))
 * 
 *  The 'key' is the event that will be catched.
 *  The 'playerId' is the Id of the player starting from 0.
 *  And the action is one of 'up', 'down', 'left', 'right
 *
 *  Remember to change this if the maximum player number is changed
 */
void MultiplayerUI::setDefaultKeys() {
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

    QMap<QString, QList<int> > keysActions;
    keysActions.insert("up", keysUp);
    keysActions.insert("down", keysDown);
    keysActions.insert("left", keysLeft);
    keysActions.insert("right", keysRight);
    keysActions.insert("drop", keysDrop);

    QMapIterator<QString, QList<int> > i(keysActions);
    while (i.hasNext()) {
        i.next();
        for (int j = 0; j < i.value().length(); j++) {
            QMap<int, QString> keyAssoc;
            keyAssoc.insert(j, i.key());
            m_playersKey.insert(i.value().at(j), keyAssoc);
        }
    }
}

/*! Do some check and start the game.
 * 
 *  Each player have to chose a differents color but pseudo can be the same. 
 */
void MultiplayerUI::startGame() {
    // Store error message
    QString errorMsg = "";
    // Count inactive players
    int nonePlayers = 0;
    // Register the players
    QMap<int, Player*> playerList;

    for (int i = 0; i < PLAYER_NUMBER; i++) {
        if (m_playersType.value(i)->currentIndex() == PLAYER_IS_NONE) {
            nonePlayers++;
            continue;
        }

        Player* player = new Player(m_playersPseudo.value(i)->text(),
            m_playersColor.value(i), m_playersType.value(i)->currentIndex());
        playerList[i] = player;

        // Check pseudo length
        if (m_playersPseudo.value(i)->text().trimmed().length() < 3) {
            errorMsg = tr("Pseudo for player %1 must be at least 3 character long").arg(i+1);
            break;
        }

        // Check that two players don't have the same colors (else, it's hard to identify it...)
        QList<int> playersWithSameColor = m_playersColor.keys(m_playersColor[i]);
        if (playersWithSameColor.length() > 1) {
            QString incriminedPlayers = QString::number(i+1);

            for (int j = 0; j < playersWithSameColor.length(); j++) {
                if (m_playersType.value(playersWithSameColor.at(j))->currentIndex() != PLAYER_IS_NONE
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

    // Check the minimum player requirement is met
    if (nonePlayers > (PLAYER_NUMBER - PLAYER_NUMBER_MIN))
        errorMsg = tr("You need at least %1 to play the game").arg(PLAYER_NUMBER_MIN);

    // Display an error if any one else launch the game
    if (errorMsg.length() > 0) {
        QMessageBox::warning(this, tr("Bomberman game error"), errorMsg);
    }
    else {
        // Draw the game field and switch to this layout
        GameUI* gameUI = qobject_cast<GameUI *>(m_parent);

        // Event handler of the gamefield
        MultiplayerHandler* eventHandler = new MultiplayerHandler(m_playersKey);

        // Set gamefield informations
        Gamefield* gamefield = qobject_cast<Gamefield *>(gameUI->getLayout("gamefield"));
        gamefield->setMap(qobject_cast<GameUI *>(m_parent)->mapList().at(m_selectedMap));
        gamefield->setPlayersList(playerList);
        gamefield->setGameRounds(m_gameRounds->value());
        gamefield->setEventHandler(eventHandler);
        gamefield->drawMap();

        // Show the gamefield
        gameUI->setVisibleUI("gamefield");
    }
}

/*! Set the selected map in the combo box
 *
 *  @param index The index of the selected map.
 */
void MultiplayerUI::selectMap(int index) {
    m_selectedMap = index;
}

/*! Define the title of the layout
 *
 *  @return Title of the layout
 */
QString MultiplayerUI::layoutTitle() {
    return tr("Multiplayer game - BomberMan");
}
