#ifndef _MULTIPLAYERHANDLER_H
#define	_MULTIPLAYERHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMap>
#include "eventhandler.h"

#include <QtCore/QDebug>

/*! Multiplayer event handler.
 *
 *  Event used for multiplayer game. All the events are recieved by this class
 *  and interpreted by it.
 */
class MultiplayerHandler : public EventHandler {
    Q_OBJECT

public:
    MultiplayerHandler(QMap<int, QMap<int, QString> > playersKeys);
    void newEvent(QKeyEvent* event);

signals:
    void playerMoved(int player, QString move);
    void bombDropped(int player);

public slots:
    void onPlayerMove(int player, int position);

private:
    /// Players move keys
    QMap<int, QMap<int, QString> > m_playersKeys;
};

#endif	/* _MULTIPLAYERHANDLER_H */

