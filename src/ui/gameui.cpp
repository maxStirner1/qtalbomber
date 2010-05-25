#include "ui/gameui.h"
#include "ui/startui.h"

GameUI::GameUI() {
    m_layoutStack = new QStackedLayout;

    // StartUI management
    StartUI* startLayout = new StartUI(this);
    m_layoutList.insert("start", startLayout);
    m_layoutStack->addWidget(startLayout);

    setLayout(m_layoutStack);

    // Set the window as a main window to avoid problems on Windows and Mac.
    setWindowFlags(Qt::Window);
    // Fix the size of the window
    setFixedSize(640, 515);
    // Qt resource system is used for the window icon
    setWindowIcon(QIcon(":/qtal-icon"));

    connect(startLayout, SIGNAL(displayUI(QString)), this, SLOT(setVisibleUI(QString)));
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