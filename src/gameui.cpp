#include "gameui.h"
#include "startui.h"
#include "multiplayerui.h"
#include "gamefield.h"

GameUI::GameUI() {
    m_layoutStack = new QStackedLayout;
    setLayout(m_layoutStack);

    // Load the game map
    loadMaps();

    // Start UI
    StartUI* startLayout = new StartUI(this);
    m_layoutList.insert("start", startLayout);
    m_layoutStack->addWidget(startLayout);

    // Multiplayer UI
    MultiplayerUI* multiLayout = new MultiplayerUI(this);
    m_layoutList.insert("multiplayer", multiLayout);
    m_layoutStack->addWidget(multiLayout);

    // Gamefield
    Gamefield* gamefieldLayout = new Gamefield(this);
    m_layoutList.insert("gamefield", gamefieldLayout);
    m_layoutStack->addWidget(gamefieldLayout);

    // Set the window as a main window to avoid problems on Windows and Mac.
    setWindowFlags(Qt::Window);
    // Fix the size of the window
    setFixedSize(640, 515);
    // Qt resource system is used for the window icon
    setWindowIcon(QIcon(":/qtal-icon"));

    connect(this, SIGNAL(quitGame()), qApp, SLOT(quit()));
}

/*! Display requested interface
 *
 *  @param layoutName Name of the layout to display
 *
 *  This method set the visible layout. Window title is also set.
 *  It is implied that the layout have already been instanciate. If not the
 *  case the method will do nothing and will still display the current layout.
 */
void GameUI::setVisibleUI(QString layoutName) {
    if (!m_layoutList.contains(layoutName))
        return;

    BaseUI* currentWidget = m_layoutList.value(layoutName);
    setWindowTitle(currentWidget->layoutTitle());
    m_layoutStack->setCurrentWidget(currentWidget);
}

/*! Load and set map in the map list.
 *
 *  For each map XML file, an instance of the Map class is created.
 *
 *  @see GameUI::mapList
 */
void GameUI::loadMaps() {
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
        Map* gameMap = new Map(mapInfo);
        if (!gameMap->isMapValid())
            continue;

        m_mapList.append(gameMap);
    }
}

/*! Available map of the game
 *
 *  @return List of the map
 *
 *  @see GameUI::loadMaps
 */
QList<Map *> GameUI::mapList() {
    return m_mapList;
}


/*! Return a defined layout
 *
 *  @param layoutName Name of the layout to return
 *  @return Pointer to requested layout
 *
 *  Method used to get the instance of layout. Can be used to execute action
 *  from a layout in another.
 *
 *  If the requested layout is not found the start user interface is returned.
 */
BaseUI* GameUI::getLayout(QString layoutName) {
    if (!m_layoutList.contains(layoutName))
        return m_layoutList["start"];

    return m_layoutList.value(layoutName);
}


/*! Run some action and emit que quit signal.
 *
 *  @see  GameUI::exitGame
 */
void GameUI::exitGame() {
    emit quitGame();
}