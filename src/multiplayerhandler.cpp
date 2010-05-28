#include "multiplayerhandler.h"

MultiplayerHandler::MultiplayerHandler(QMap<int,QMap<int,QString> > playersKeys) {
    m_playersKeys = playersKeys;
}

/*! Recieve and handle the events of the gamefield.
 *
 *  @param event Event sended by the gamefield.
 *
 *  If an event is reconised, the corresponding signal is send which is catched
 *  by the gamefield.
 */
void MultiplayerHandler::newEvent(QKeyEvent* event) {
    if (m_playersKeys.contains(event->key())) {
        int player = m_playersKeys.value(event->key()).keys().at(0);
        QString action = m_playersKeys.value(event->key()).values().at(0);

        if (action == "drop")
            emit bombDropped(player);
        else
            emit playerMoved(player, action);
    }
}

/*! Send the information if necessary that a player has moved
 *
 *  @param player The player who moved
 *  @param position The new position of the player
 *
 *  As we are in local, nothing has to be done
 */
void MultiplayerHandler::onPlayerMove(int player, int position) {
    // Get ride of warnings
    player = player;
    position = position;
}

