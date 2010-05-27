#ifndef _EVENTHANDLER_H
#define	_EVENTHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QKeyEvent>

/*! Base class for every event handler
 *
 *  Every event handler must implements the defined methods.
 */
class EventHandler : public QObject {
    Q_OBJECT

public:
    virtual void newEvent(QKeyEvent* event) = 0;

signals:
    /// A player want to move
    void playerMoved(int player, QString move);
    /// A bomb have been dropped by a player
    void bombDropped(int player);

public slots:
    virtual void onPlayerMove(int player, int position) = 0;
};


#endif	/* _EVENTHANDLER_H */

