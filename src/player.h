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
    void setState(int status);
    int state();
    void setBombCapacity(int capacity);
    int bombCapacity();
    void toggleKickBonus();
    bool hasKick();
    void setLifes(int lifes);
    int lifes();
    int bombsRange();
    void setBombsRange(int range);

private:
    /// Identifier of the player
    int m_playerId;
    /// Player status
    int m_status;
    /// Number of bomb which can be dropped
    int m_bombCapacity;
    /// Bombs range
    int m_bombsRange;
    /// Kick bonus
    bool m_kickBonus;
    /// Number of life of the player
    int m_lifes;
};

#endif	/* _PLAYER_H */
