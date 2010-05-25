#include "startui.h"

/*! Create the differents buttons of the interface and set the splash image.
 *
 *  @param parent QWidget parent
 *
 *  As the Qt resource system enable it, the splash is stored as binary in the
 *  program to avoid path problems on the differents platforms.
 */
StartUI::StartUI(QWidget* parent) {
    // Layout initialization
    m_startPanel = new QGridLayout;
    setLayout(m_startPanel);

    // Multyplayer button
    m_multiplayerGameButton = new QPushButton(tr("Multiplayer Game"));
    m_multiplayerGameButton->setObjectName("multiplayer");
    m_startPanel->addWidget(m_multiplayerGameButton, 1, 1);

    // About game button
    m_aboutGameButton = new QPushButton(tr("About the game"));
    m_aboutGameButton->setObjectName("about");
    m_startPanel->addWidget(m_aboutGameButton, 2, 1);

    // Exit game button
    m_exitGameButton = new QPushButton(tr("Exit game"));
    m_startPanel->addWidget(m_exitGameButton, 3, 1);

    // Game splash
    m_gameSplash = new QLabel();
    m_gameSplash->setPixmap(QPixmap(":/qtal-splash"));
    m_startPanel->addWidget(m_gameSplash, 0, 0, 3, 1);

    // Connect button event to actions
    QObject::connect(m_multiplayerGameButton, SIGNAL(clicked()), this, SLOT(displayLayout()));
    QObject::connect(m_aboutGameButton, SIGNAL(clicked()), this, SLOT(displayLayout()));
    QObject::connect(m_exitGameButton, SIGNAL(clicked()), parent, SLOT(exitGame()));
}

/*! Define the title of the layout
 *
 *  @return Layout title
 */
QString StartUI::layoutTitle() {
    return tr("BomberMan Game");
}

/*! Set the corresponding layout from the signal sender.
 *
 *  This slot is called when a button of the start interface is clicked.
 *  The method will try to determine the sender of the object and display the
 *  proper interface.
 *
 *  If no sender or no corresponding layout can be found, the method will do
 *  nothing.
 */
void StartUI::displayLayout() {
    QString layoutName = sender()->objectName();

    if (layoutName.length() != 0)
        emit displayUI(layoutName);
}