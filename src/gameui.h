#ifndef _GAMEUI_H
#define	_GAMEUI_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtGui/QStackedLayout>
#include <QtCore/QCoreApplication>
#include "baseui.h"
#include "map.h"

/*! Main window of the game
 *
 *  Construct the main interface of the game. The differents user interface are
 *  instanciate in this class and stored in a layout stack.
 *  The layout are displayed on demand.
 *
 *  By default the constructor show the start interface.
 *
 *  @see StartUI
 */
class GameUI : public QWidget {
    Q_OBJECT

public:
    GameUI();
    BaseUI* getLayout(QString layoutName);
    QList<Map *> mapList();

signals:
    /*! Signal emitted when want to quit the game. Catched by exitGame slot.
     * 
     *  @see GameUI::exitGame
     */
    void quitGame();

public slots:
    void setVisibleUI(QString layoutName);
    void exitGame();

private:
    /// Stack containing all the differents layout
    QStackedLayout* m_layoutStack;
    /// List of the differents instance of the user interface
    QMap<QString, BaseUI*> m_layoutList;
    /// List of the availbale maps
    QList<Map *> m_mapList;

    void loadMaps();
};

#endif	/* _GAMEUI_H */

