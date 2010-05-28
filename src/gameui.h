#ifndef _GAMEUI_H
#define	_GAMEUI_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtGui/QStackedLayout>
#include <QtCore/QCoreApplication>
#include "baseui.h"

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
};

#endif	/* _GAMEUI_H */

