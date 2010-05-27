#include "ui/multiplayerui.h"
#include "ui/gameui.h"
#include "ui/startui.h"

MultiplayerUI::MultiplayerUI(QWidget* parent) : m_parent(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* playersLayout = new QHBoxLayout;

    /*
     * Create player settings UI
     */
    for (int currentPlayer = 0; currentPlayer < PLAYER_NUMBER; currentPlayer++) {
        QGroupBox* playerGroup = new QGroupBox;
        QFormLayout* playerOption = new QFormLayout;
        playerOption->setRowWrapPolicy(QFormLayout::WrapLongRows);

        QLineEdit* playerPseudo = new QLineEdit;
        playerPseudo->setMinimumWidth(70);
        playerOption->addRow(tr("Pseudo:"), playerPseudo);
        m_playersPseudo[currentPlayer] = playerPseudo;

        QComboBox* playerType = new QComboBox;
        playerType->addItem(tr("None"));
        playerType->addItem(tr("Player"));
//        playerType.addItem(tr("Bot"));
        playerOption->addRow(tr("Type:"), playerType);
        m_playersType[currentPlayer] = playerType;
        
        QPushButton* playerColorButton = new QPushButton;
        playerColorButton->setFixedSize(70, 70);
        playerOption->addRow(tr("Color:"), playerColorButton);
        m_playersColorButton[currentPlayer] = playerColorButton;
        setPlayerColor(currentPlayer, QColor(PLAYER_DEFAULT_COLOR));
        
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

    QLabel* gameRoundsLabel = new QLabel(tr("Rounds:"));
    gameLayout->addWidget(gameRoundsLabel, 0, 0, Qt::AlignLeft);
    
    m_gameRounds = new QSpinBox;
    m_gameRounds->setMaximum(10);
    m_gameRounds->setMinimum(1);
    gameLayout->addWidget(m_gameRounds, 0, 1, Qt::AlignLeft);

    QLabel* gameMapLabel = new QLabel(tr("Map:"));
    gameLayout->addWidget(gameMapLabel, 1, 0, Qt::AlignLeft);

    m_mapComboBox = new QComboBox;
    gameLayout->addWidget(m_mapComboBox, 1, 1, Qt::AlignLeft);

    QPushButton* moreInfoButton = new QPushButton(tr("Infos"));
    gameLayout->addWidget(moreInfoButton, 1, 2, Qt::AlignLeft);

    QPushButton* playButton = new QPushButton(tr("Play"));
    gameLayout->addWidget(playButton, 0, 1, 1, 2, Qt::AlignRight);

    QPushButton* backButton = new QPushButton(tr("Back"));
    gameLayout->addWidget(backButton, 1, 2, Qt::AlignRight);

    QObject::connect(backButton, SIGNAL(clicked()), this, SLOT(displayStartUI()));

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

    int playerId = m_playersColorButton.key(playerButton);
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

/*! Define the title of the layout
 *
 *  @return Title of the layout
 */
QString MultiplayerUI::layoutTitle() {
    return tr("Multiplayer game - BomberMan");
}
