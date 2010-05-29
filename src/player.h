#ifndef _PLAYER_H
#define	_PLAYER_H

#include "qtalbomber.h"
#include <QGraphicsEllipseItem>
#include <QString>
#include <QColor>
#include <QBrush>
#include <QLinearGradient>

class Player : public QGraphicsEllipseItem {
public:
    Player(int playerId);
    int playerId();
    void setPlayerStatus(int status);
    int playerStatus();
    void setBombCapacity(int capacity);
    int bombCapacity();
    void togglePlayerKick();
    bool hasKick();
    void setPlayerLifes(int lifes);
    int playerLifes();

private:
    /// Identifier of the player
    int m_playerId;
    /// Player status
    int m_playerStatus;
    /// Number of bomb which can be dropped
    int m_bombCapacity;
    /// Bombs range
    int m_bombRange;
    /// Kick bonus
    bool m_playerKick;
    /// Number of life of the player
    int m_playerLifes;

    void initPlayer();
};

#endif	/* _PLAYER_H */
